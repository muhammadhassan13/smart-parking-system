#ifndef PARKINGREQUEST_H
#define PARKINGREQUEST_H

#include <string>
#include <ctime>
#include "Vehicle.h"
#include "ParkingSlot.h"
using namespace std;

// State Machine Enum
enum class RequestState {
    REQUESTED,
    ALLOCATED,
    OCCUPIED,
    RELEASED,
    CANCELLED
};

class ParkingRequest {
private:
    string requestId;
    Vehicle* vehicle;
    string requestedZoneId;
    time_t requestTime;
    time_t allocationTime;
    time_t releaseTime;
    RequestState currentState;
    ParkingSlot* allocatedSlot;
    bool crossZoneAllocation;
    
public:
    ParkingRequest();
    ParkingRequest(const string& requestId, Vehicle* vehicle, const string& zoneId);
    
    // Getters
    string getRequestId() const;
    Vehicle* getVehicle() const;
    string getRequestedZoneId() const;
    time_t getRequestTime() const;
    time_t getAllocationTime() const;
    time_t getReleaseTime() const;
    RequestState getCurrentState() const;
    ParkingSlot* getAllocatedSlot() const;
    bool isCrossZoneAllocation() const;
    
    // State management
    bool allocateSlot(ParkingSlot* slot, bool crossZone = false);
    bool markAsOccupied();
    bool markAsReleased();
    bool cancelRequest();
    
    // Utility
    void displayRequestInfo() const;
    string stateToString() const;
    double calculateDuration() const; // in minutes
    
    // Check if request is active
    bool isActive() const;
};

#endif