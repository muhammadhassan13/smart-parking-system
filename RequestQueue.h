#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include "ParkingRequest.h"
#include <string>
using namespace std;

class RequestQueue {
private:
    struct QueueNode {
        ParkingRequest* request;
        QueueNode* next;
        
        QueueNode(ParkingRequest* req) : request(req), next(nullptr) {}
    };
    
    QueueNode* front;
    QueueNode* rear;
    int queueSize;
    int maxSize;
    
public:
    RequestQueue(int maxSize = 100);
    ~RequestQueue();
    
    // Queue operations
    bool enqueue(ParkingRequest* request);
    ParkingRequest* dequeue();
    ParkingRequest* peek() const;
    bool isEmpty() const;
    bool isFull() const;
    int getSize() const;
    
    // Utility
    void displayQueue() const;
    void clear();
    
private:
    void clearQueue();
};

#endif