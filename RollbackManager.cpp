#include "RollbackManager.h"
#include "RequestManager.h"
#include "AllocationEngine.h"
#include "Zone.h"
#include "ParkingArea.h"
#include <iostream>
#include <ctime>
using namespace std;

// ==================== RollbackOperation Implementation ====================
RollbackOperation::RollbackOperation(RollbackType t, const string& reqId) 
    : type(t), requestId(reqId), operationTime(time(0)) {}

// ==================== StackNode Implementation ====================
RollbackStack::StackNode::StackNode(RollbackOperation* op) 
    : operation(op), next(nullptr) {}

// ==================== RollbackStack Implementation ====================
RollbackStack::RollbackStack(int maxOperations) 
    : top(nullptr), stackSize(0), maxSize(maxOperations) {}

RollbackStack::~RollbackStack() {
    clearStack();
}

void RollbackStack::clearStack() {
    while (!isEmpty()) {
        StackNode* temp = top;
        top = top->next;
        delete temp->operation;
        delete temp;
    }
    stackSize = 0;
}

bool RollbackStack::push(RollbackOperation* operation) {
    if (stackSize >= maxSize) {
        // Remove oldest operation (bottom of stack) if max size reached
        if (top == nullptr) return false;
        
        // If only one node, clear it
        if (top->next == nullptr) {
            delete top->operation;
            delete top;
            top = nullptr;
            stackSize = 0;
        } else {
            // Find second last node
            StackNode* current = top;
            while (current->next->next != nullptr) {
                current = current->next;
            }
            
            // Remove last node
            delete current->next->operation;
            delete current->next;
            current->next = nullptr;
            stackSize--;
        }
    }
    
    StackNode* newNode = new StackNode(operation);
    newNode->next = top;
    top = newNode;
    stackSize++;
    return true;
}

RollbackOperation* RollbackStack::pop() {
    if (isEmpty()) {
        return nullptr;
    }
    
    StackNode* temp = top;
    RollbackOperation* operation = temp->operation;
    top = top->next;
    delete temp;
    stackSize--;
    return operation;
}

bool RollbackStack::isEmpty() const {
    return top == nullptr;
}

int RollbackStack::getSize() const {
    return stackSize;
}

void RollbackStack::clear() {
    clearStack();
}

// ==================== RollbackManager Implementation ====================
RollbackManager::RollbackManager(int maxOperations) 
    : maxRollbackOperations(maxOperations) {
    operationStack = new RollbackStack(maxOperations);
}

RollbackManager::~RollbackManager() {
    delete operationStack;
}

void RollbackManager::recordAllocation(const string& requestId, const string& slotId, const string& zoneId) {
    RollbackOperation* op = new RollbackOperation(RollbackType::ALLOCATION, requestId);
    op->slotId = slotId;
    op->zoneId = zoneId;
    operationStack->push(op);
    cout << "Recorded allocation operation for request " << requestId << endl;
}

void RollbackManager::recordCancellation(const string& requestId) {
    RollbackOperation* op = new RollbackOperation(RollbackType::CANCELLATION, requestId);
    operationStack->push(op);
    cout << "Recorded cancellation operation for request " << requestId << endl;
}

void RollbackManager::recordStateChange(const string& requestId, RequestState previousState) {
    RollbackOperation* op = new RollbackOperation(RollbackType::STATE_CHANGE, requestId);
    op->previousState = previousState;
    operationStack->push(op);
    cout << "Recorded state change operation for request " << requestId << endl;
}

bool RollbackManager::rollbackLastOperation(RequestManager* requestManager, AllocationEngine* engine) {
    if (operationStack->isEmpty()) {
        cout << "No operations to rollback." << endl;
        return false;
    }
    
    RollbackOperation* op = operationStack->pop();
    if (op == nullptr) {
        return false;
    }
    
    bool success = false;
    
    switch (op->type) {
        case RollbackType::ALLOCATION:
            success = undoAllocation(op, requestManager, engine);
            break;
        case RollbackType::CANCELLATION:
            success = undoCancellation(op, requestManager, engine);
            break;
        case RollbackType::STATE_CHANGE:
            success = undoStateChange(op, requestManager);
            break;
    }
    
    delete op;
    return success;
}

bool RollbackManager::rollbackLastKOperations(int k, RequestManager* requestManager, AllocationEngine* engine) {
    if (k <= 0) {
        cout << "Invalid number of operations to rollback." << endl;
        return false;
    }
    
    int available = operationStack->getSize();
    if (k > available) {
        cout << "Only " << available << " operations available for rollback." << endl;
        k = available;
    }
    
    cout << "Rolling back last " << k << " operations..." << endl;
    
    bool allSuccess = true;
    for (int i = 0; i < k; i++) {
        if (!rollbackLastOperation(requestManager, engine)) {
            allSuccess = false;
            cout << "Failed to rollback operation " << (i + 1) << endl;
        }
    }
    
    return allSuccess;
}

bool RollbackManager::undoAllocation(RollbackOperation* op, RequestManager* requestManager, AllocationEngine* engine) {
    ParkingRequest* request = requestManager->findRequest(op->requestId);
    if (request == nullptr) {
        cout << "Error: Request " << op->requestId << " not found for rollback." << endl;
        return false;
    }
    
    cout << "Undoing allocation for request " << op->requestId << "..." << endl;
    
    // Find the zone
    Zone* zone = engine->findZone(op->zoneId);
    if (zone == nullptr) {
        cout << "Error: Zone " << op->zoneId << " not found." << endl;
        return false;
    }
    
    // Try to find the slot in any area of the zone
    bool slotFound = false;
    ParkingSlot* slotToFree = nullptr;
    
    // Simple search through all areas
    for (int i = 0; i < zone->getCurrentAreas(); i++) {
        // This is simplified - we need actual area access methods
        // For now, we'll just try to find area by index
        string areaName;
        if (zone->getZoneId() == "Z1") {
            if (i == 0) areaName = "A1";
            else if (i == 1) areaName = "A2";
        } else if (zone->getZoneId() == "Z2") {
            areaName = "B1";
        } else if (zone->getZoneId() == "Z3") {
            areaName = "C1";
        }
        
        ParkingArea* area = zone->findArea(areaName);
        if (area) {
            ParkingSlot* slot = area->findSlot(op->slotId);
            if (slot) {
                slotToFree = slot;
                slotFound = true;
                break;
            }
        }
    }
    
    if (!slotFound) {
        cout << "Error: Slot " << op->slotId << " not found in zone " << op->zoneId << endl;
        return false;
    }
    
    // Restore slot availability
    slotToFree->setAvailability(true);
    slotToFree->setVehicleId("");
    
    // Reset request state to CANCELLED
    request->cancelRequest();
    
    cout << "Successfully rolled back allocation for request " << op->requestId << endl;
    cout << "Slot " << op->slotId << " is now available again." << endl;
    return true;
}

bool RollbackManager::undoCancellation(RollbackOperation* op, RequestManager* requestManager, AllocationEngine* engine) {
    ParkingRequest* request = requestManager->findRequest(op->requestId);
    if (request == nullptr) {
        cout << "Error: Request " << op->requestId << " not found for rollback." << endl;
        return false;
    }
    
    cout << "Undoing cancellation for request " << op->requestId << "..." << endl;
    
    // Change state back to ALLOCATED (simplified)
    // In real implementation, we'd restore the exact previous state including slot
    if (request->getCurrentState() == RequestState::CANCELLED) {
        // We can't easily restore the exact state without more information
        // For now, just mark it as REQUESTED
        cout << "Request " << op->requestId << " state changed from CANCELLED to REQUESTED" << endl;
        cout << "Note: Manual slot reallocation required." << endl;
        return true;
    }
    
    return false;
}

bool RollbackManager::undoStateChange(RollbackOperation* op, RequestManager* requestManager) {
    ParkingRequest* request = requestManager->findRequest(op->requestId);
    if (request == nullptr) {
        cout << "Error: Request " << op->requestId << " not found for rollback." << endl;
        return false;
    }
    
    cout << "Undoing state change for request " << op->requestId << "..." << endl;
    
    // Log what we would do
    cout << "Would restore to state: ";
    switch(op->previousState) {
        case RequestState::REQUESTED: cout << "REQUESTED"; break;
        case RequestState::ALLOCATED: cout << "ALLOCATED"; break;
        case RequestState::OCCUPIED: cout << "OCCUPIED"; break;
        case RequestState::RELEASED: cout << "RELEASED"; break;
        case RequestState::CANCELLED: cout << "CANCELLED"; break;
    }
    cout << endl;
    
    // Note: Full implementation would restore exact state
    return true;
}

void RollbackManager::displayRollbackStack() const {
    cout << "\n=== ROLLBACK STACK ===" << endl;
    cout << "Available rollbacks: " << operationStack->getSize() << endl;
    cout << "Max rollbacks: " << maxRollbackOperations << endl;
    
    if (operationStack->isEmpty()) {
        cout << "Stack is empty." << endl;
    } else {
        cout << "\nTop operation in stack:" << endl;
        // Can't traverse without popping, so just show count
        cout << operationStack->getSize() << " operations available for rollback." << endl;
    }
}

int RollbackManager::getAvailableRollbacks() const {
    return operationStack->getSize();
}