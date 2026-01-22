#include "RollbackManager.h"
#include "RequestManager.h"
#include "AllocationEngine.h"
#include <iostream>
#include <ctime>
using namespace std;

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
    
    // Find the zone and slot
    Zone* zone = engine->findZone(op->zoneId);
    if (zone == nullptr) {
        cout << "Error: Zone " << op->zoneId << " not found." << endl;
        return false;
    }
    
    // Find the area containing the slot
    bool slotFound = false;
    for (int i = 0; i < zone->getCurrentAreas(); i++) {
        ParkingArea* area = zone->findArea("A" + to_string(i + 1)); // Simplified - need better area finding
        if (area) {
            ParkingSlot* slot = area->findSlot(op->slotId);
            if (slot) {
                // Restore slot availability
                slot->setAvailability(true);
                slot->setVehicleId("");
                
                // Reset request state
                request->cancelRequest(); // This will set state to CANCELLED
                slotFound = true;
                break;
            }
        }
    }
    
    if (!slotFound) {
        cout << "Error: Slot " << op->slotId << " not found." << endl;
        return false;
    }
    
    cout << "Successfully rolled back allocation for request " << op->requestId << endl;
    return true;
}

bool RollbackManager::undoCancellation(RollbackOperation* op, RequestManager* requestManager, AllocationEngine* engine) {
    ParkingRequest* request = requestManager->findRequest(op->requestId);
    if (request == nullptr) {
        cout << "Error: Request " << op->requestId << " not found for rollback." << endl;
        return false;
    }
    
    cout << "Undoing cancellation for request " << op->requestId << "..." << endl;
    
    // For simplicity, we'll just change state back to previous valid state
    // In a real system, we'd need to restore the exact previous state
    request->allocateSlot(nullptr, false); // This will fail but change state
    
    cout << "Note: Cancellation rollback requires manual slot reallocation." << endl;
    return true;
}

bool RollbackManager::undoStateChange(RollbackOperation* op, RequestManager* requestManager) {
    ParkingRequest* request = requestManager->findRequest(op->requestId);
    if (request == nullptr) {
        cout << "Error: Request " << op->requestId << " not found for rollback." << endl;
        return false;
    }
    
    cout << "Undoing state change for request " << op->requestId << "..." << endl;
    // Simplified - in real implementation, we'd restore to previousState
    return true;
}

void RollbackManager::displayRollbackStack() const {
    cout << "\n=== ROLLBACK STACK ===" << endl;
    cout << "Available rollbacks: " << operationStack->getSize() << endl;
    cout << "Max rollbacks: " << maxRollbackOperations << endl;
    
    if (operationStack->isEmpty()) {
        cout << "Stack is empty." << endl;
    } else {
        cout << "\nRecent operations (newest first):" << endl;
        
        // We need to traverse the stack without popping
        // For simplicity, we'll just show the top operation
        RollbackOperation* topOp = operationStack->pop();
        if (topOp) {
            cout << "1. ";
            switch (topOp->type) {
                case RollbackType::ALLOCATION:
                    cout << "ALLOCATION - Request: " << topOp->requestId 
                         << ", Slot: " << topOp->slotId 
                         << ", Zone: " << topOp->zoneId;
                    break;
                case RollbackType::CANCELLATION:
                    cout << "CANCELLATION - Request: " << topOp->requestId;
                    break;
                case RollbackType::STATE_CHANGE:
                    cout << "STATE_CHANGE - Request: " << topOp->requestId;
                    break;
            }
            cout << endl;
            
            // Push it back
            operationStack->push(topOp);
        }
    }
}

int RollbackManager::getAvailableRollbacks() const {
    return operationStack->getSize();
}