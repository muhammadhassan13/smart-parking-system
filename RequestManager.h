#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include "ParkingRequest.h"
#include <string>
using namespace std;

// Node for linked list
struct RequestNode {
    ParkingRequest* request;
    RequestNode* next;
    
    RequestNode(ParkingRequest* req) : request(req), next(nullptr) {}
};

class RequestManager {
private:
    RequestNode* head;
    RequestNode* tail;
    int requestCount;
    
public:
    RequestManager();
    ~RequestManager();
    
    // Request management
    bool addRequest(ParkingRequest* request);
    ParkingRequest* findRequest(const string& requestId);
    bool removeRequest(const string& requestId);
    
    // Getters
    int getRequestCount() const;
    
    // Display functions
    void displayAllRequests() const;
    void displayActiveRequests() const;
    void displayRequestHistory() const;
    
    // Statistics
    int countByState(RequestState state) const;
    double getAverageDuration() const;
    
private:
    void clearList();
};

#endif