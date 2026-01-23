#include "RequestQueue.h"
#include <iostream>
using namespace std;

RequestQueue::RequestQueue(int maxSize) 
    : front(nullptr), rear(nullptr), queueSize(0), maxSize(maxSize) {}

RequestQueue::~RequestQueue() {
    clearQueue();
}

void RequestQueue::clearQueue() {
    while (!isEmpty()) {
        QueueNode* temp = front;
        front = front->next;
        // Note: We don't delete request object as it's managed elsewhere
        delete temp;
    }
    rear = nullptr;
    queueSize = 0;
}

bool RequestQueue::enqueue(ParkingRequest* request) {
    if (isFull()) {
        cout << "Error: Request queue is full. Cannot add more requests." << endl;
        return false;
    }
    
    QueueNode* newNode = new QueueNode(request);
    
    if (isEmpty()) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    
    queueSize++;
    return true;
}

ParkingRequest* RequestQueue::dequeue() {
    if (isEmpty()) {
        return nullptr;
    }
    
    QueueNode* temp = front;
    ParkingRequest* request = temp->request;
    
    front = front->next;
    if (front == nullptr) {
        rear = nullptr;
    }
    
    delete temp;
    queueSize--;
    return request;
}

ParkingRequest* RequestQueue::peek() const {
    if (isEmpty()) {
        return nullptr;
    }
    return front->request;
}

bool RequestQueue::isEmpty() const {
    return front == nullptr;
}

bool RequestQueue::isFull() const {
    return queueSize >= maxSize;
}

int RequestQueue::getSize() const {
    return queueSize;
}

void RequestQueue::displayQueue() const {
    cout << "\n=== PENDING REQUESTS QUEUE ===" << endl;
    cout << "Queue Size: " << queueSize << "/" << maxSize << endl;
    
    if (isEmpty()) {
        cout << "Queue is empty." << endl;
        return;
    }
    
    QueueNode* current = front;
    int counter = 1;
    
    while (current != nullptr) {
        cout << "\n" << counter << ". ";
        cout << "Request ID: " << current->request->getRequestId();
        cout << ", Vehicle: " << current->request->getVehicle()->getVehicleId();
        cout << ", Zone: " << current->request->getRequestedZoneId();
        cout << ", State: " << current->request->stateToString();
        current = current->next;
        counter++;
    }
    cout << endl;
}

void RequestQueue::clear() {
    clearQueue();
}