#include "RequestManager.h"
#include <iostream>
#include <iomanip>
using namespace std;

// ==================== RequestNode Implementation ====================
RequestNode::RequestNode(ParkingRequest* req) 
    : request(req), next(nullptr) {}

// ==================== RequestManager Implementation ====================
RequestManager::RequestManager() : head(nullptr), tail(nullptr), requestCount(0) {}

RequestManager::~RequestManager() {
    clearList();
}

void RequestManager::clearList() {
    RequestNode* current = head;
    while (current != nullptr) {
        RequestNode* next = current->next;
        delete current->request; // Delete the request object
        delete current; // Delete the node
        current = next;
    }
    head = nullptr;
    tail = nullptr;
    requestCount = 0;
}

bool RequestManager::addRequest(ParkingRequest* request) {
    if (request == nullptr) {
        cout << "Error: Cannot add null request." << endl;
        return false;
    }
    
    RequestNode* newNode = new RequestNode(request);
    
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    
    requestCount++;
    return true;
}

ParkingRequest* RequestManager::findRequest(const string& requestId) {
    RequestNode* current = head;
    while (current != nullptr) {
        if (current->request->getRequestId() == requestId) {
            return current->request;
        }
        current = current->next;
    }
    return nullptr;
}

bool RequestManager::removeRequest(const string& requestId) {
    if (head == nullptr) {
        return false;
    }
    
    // Special case: head node
    if (head->request->getRequestId() == requestId) {
        RequestNode* temp = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete temp->request;
        delete temp;
        requestCount--;
        return true;
    }
    
    // Search for node to delete
    RequestNode* current = head;
    while (current->next != nullptr) {
        if (current->next->request->getRequestId() == requestId) {
            RequestNode* temp = current->next;
            current->next = temp->next;
            
            if (temp == tail) {
                tail = current;
            }
            
            delete temp->request;
            delete temp;
            requestCount--;
            return true;
        }
        current = current->next;
    }
    
    return false;
}

int RequestManager::getRequestCount() const {
    return requestCount;
}

void RequestManager::displayAllRequests() const {
    cout << "\n=== ALL PARKING REQUESTS (" << requestCount << ") ===" << endl;
    
    if (head == nullptr) {
        cout << "No requests found." << endl;
        return;
    }
    
    RequestNode* current = head;
    int counter = 1;
    
    while (current != nullptr) {
        cout << "\n" << counter << ". ";
        current->request->displayRequestInfo();
        current = current->next;
        counter++;
    }
}

void RequestManager::displayActiveRequests() const {
    cout << "\n=== ACTIVE PARKING REQUESTS ===" << endl;
    
    RequestNode* current = head;
    int activeCount = 0;
    int counter = 1;
    
    while (current != nullptr) {
        if (current->request->isActive()) {
            cout << "\n" << counter << ". ";
            current->request->displayRequestInfo();
            activeCount++;
            counter++;
        }
        current = current->next;
    }
    
    if (activeCount == 0) {
        cout << "No active requests found." << endl;
    } else {
        cout << "\nTotal active requests: " << activeCount << endl;
    }
}

void RequestManager::displayRequestHistory() const {
    cout << "\n=== REQUEST HISTORY ===" << endl;
    
    RequestNode* current = head;
    int completed = 0;
    int cancelled = 0;
    int active = 0;
    int counter = 1;
    
    // First pass: count statistics
    while (current != nullptr) {
        RequestState state = current->request->getCurrentState();
        if (state == RequestState::RELEASED) completed++;
        else if (state == RequestState::CANCELLED) cancelled++;
        else if (current->request->isActive()) active++;
        current = current->next;
    }
    
    cout << "Total Requests: " << requestCount << endl;
    cout << "Completed: " << completed << endl;
    cout << "Cancelled: " << cancelled << endl;
    cout << "Active: " << active << endl;
    cout << "Average Duration: " << fixed << setprecision(2) << getAverageDuration() << " minutes" << endl;
    
    // Display all requests
    current = head;
    cout << "\nDetailed History:" << endl;
    while (current != nullptr) {
        cout << counter << ". ";
        cout << "ID: " << current->request->getRequestId();
        cout << ", Vehicle: " << current->request->getVehicle()->getVehicleId();
        cout << ", State: " << current->request->stateToString();
        cout << ", Zone: " << current->request->getRequestedZoneId();
        
        if (current->request->getAllocatedSlot() != nullptr) {
            cout << ", Slot: " << current->request->getAllocatedSlot()->getSlotId();
        }
        
        cout << endl;
        current = current->next;
        counter++;
    }
}

int RequestManager::countByState(RequestState state) const {
    int count = 0;
    RequestNode* current = head;
    
    while (current != nullptr) {
        if (current->request->getCurrentState() == state) {
            count++;
        }
        current = current->next;
    }
    
    return count;
}

double RequestManager::getAverageDuration() const {
    RequestNode* current = head;
    double totalDuration = 0.0;
    int completedCount = 0;
    
    while (current != nullptr) {
        if (current->request->getCurrentState() == RequestState::RELEASED) {
            totalDuration += current->request->calculateDuration();
            completedCount++;
        }
        current = current->next;
    }
    
    if (completedCount == 0) {
        return 0.0;
    }
    
    return totalDuration / completedCount;
}