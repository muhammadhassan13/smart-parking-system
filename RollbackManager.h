#ifndef ROLLBACKMANAGER_H
#define ROLLBACKMANAGER_H

#include "ParkingRequest.h"
#include <string>
using namespace std;

// Rollback operation types
enum class RollbackType {
    ALLOCATION,
    CANCELLATION,
    STATE_CHANGE
};

// Rollback operation data structure
struct RollbackOperation {
    RollbackType type;
    string requestId;
    string slotId;
    string zoneId;
    RequestState previousState;
    time_t operationTime;
    
    RollbackOperation(RollbackType t, const string& reqId) 
        : type(t), requestId(reqId), operationTime(time(0)) {}
};

// Stack implementation for rollback operations
class RollbackStack {
private:
    struct StackNode {
        RollbackOperation* operation;
        StackNode* next;
        
        StackNode(RollbackOperation* op) : operation(op), next(nullptr) {}
    };
    
    StackNode* top;
    int stackSize;
    int maxSize;
    
public:
    RollbackStack(int maxOperations = 100);
    ~RollbackStack();
    
    bool push(RollbackOperation* operation);
    RollbackOperation* pop();
    bool isEmpty() const;
    int getSize() const;
    void clear();
    
private:
    void clearStack();
};

class RollbackManager {
private:
    RollbackStack* operationStack;
    int maxRollbackOperations;
    
public:
    RollbackManager(int maxOperations = 10);
    ~RollbackManager();
    
    // Record operations
    void recordAllocation(const string& requestId, const string& slotId, const string& zoneId);
    void recordCancellation(const string& requestId);
    void recordStateChange(const string& requestId, RequestState previousState);
    
    // Rollback operations
    bool rollbackLastOperation(RequestManager* requestManager, AllocationEngine* engine);
    bool rollbackLastKOperations(int k, RequestManager* requestManager, AllocationEngine* engine);
    
    // Utility
    void displayRollbackStack() const;
    int getAvailableRollbacks() const;
    
private:
    bool undoAllocation(RollbackOperation* op, RequestManager* requestManager, AllocationEngine* engine);
    bool undoCancellation(RollbackOperation* op, RequestManager* requestManager, AllocationEngine* engine);
    bool undoStateChange(RollbackOperation* op, RequestManager* requestManager);
};

#endif