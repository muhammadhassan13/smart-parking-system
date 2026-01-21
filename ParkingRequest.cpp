#include "ParkingRequest.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

ParkingRequest::ParkingRequest() 
    : vehicle(nullptr), allocatedSlot(nullptr), crossZoneAllocation(false),
      requestTime(0), allocationTime(0), releaseTime(0),
      currentState(RequestState::REQUESTED) {}

ParkingRequest::ParkingRequest(const string& requestId, Vehicle* vehicle, const string& zoneId)
    : requestId(requestId), vehicle(vehicle), requestedZoneId(zoneId),
      allocatedSlot(nullptr), crossZoneAllocation(false),
      currentState(RequestState::REQUESTED) {
    requestTime = time(0);
    allocationTime = 0;
    releaseTime = 0;
}

string ParkingRequest::getRequestId() const {
    return requestId;
}

Vehicle* ParkingRequest::getVehicle() const {
    return vehicle;
}

string ParkingRequest::getRequestedZoneId() const {
    return requestedZoneId;
}

time_t ParkingRequest::getRequestTime() const {
    return requestTime;
}

time_t ParkingRequest::getAllocationTime() const {
    return allocationTime;
}

time_t ParkingRequest::getReleaseTime() const {
    return releaseTime;
}

RequestState ParkingRequest::getCurrentState() const {
    return currentState;
}

ParkingSlot* ParkingRequest::getAllocatedSlot() const {
    return allocatedSlot;
}

bool ParkingRequest::isCrossZoneAllocation() const {
    return crossZoneAllocation;
}

bool ParkingRequest::allocateSlot(ParkingSlot* slot, bool crossZone) {
    if (currentState != RequestState::REQUESTED) {
        cout << "Error: Cannot allocate slot. Request is not in REQUESTED state." << endl;
        return false;
    }
    
    if (slot == nullptr) {
        cout << "Error: Cannot allocate null slot." << endl;
        return false;
    }
    
    if (!slot->getAvailability()) {
        cout << "Error: Slot is not available." << endl;
        return false;
    }
    
    allocatedSlot = slot;
    crossZoneAllocation = crossZone;
    currentState = RequestState::ALLOCATED;
    allocationTime = time(0);
    
    // Mark slot as occupied
    slot->setAvailability(false);
    slot->setVehicleId(vehicle->getVehicleId());
    
    return true;
}

bool ParkingRequest::markAsOccupied() {
    if (currentState != RequestState::ALLOCATED) {
        cout << "Error: Cannot mark as occupied. Request is not in ALLOCATED state." << endl;
        return false;
    }
    
    currentState = RequestState::OCCUPIED;
    return true;
}

bool ParkingRequest::markAsReleased() {
    if (currentState != RequestState::OCCUPIED) {
        cout << "Error: Cannot release. Request is not in OCCUPIED state." << endl;
        return false;
    }
    
    currentState = RequestState::RELEASED;
    releaseTime = time(0);
    
    // Free the slot
    if (allocatedSlot != nullptr) {
        allocatedSlot->setAvailability(true);
        allocatedSlot->setVehicleId("");
    }
    
    return true;
}

bool ParkingRequest::cancelRequest() {
    if (currentState == RequestState::RELEASED || currentState == RequestState::CANCELLED) {
        cout << "Error: Cannot cancel. Request is already completed or cancelled." << endl;
        return false;
    }
    
    // Free the slot if it was allocated
    if (allocatedSlot != nullptr && currentState == RequestState::ALLOCATED) {
        allocatedSlot->setAvailability(true);
        allocatedSlot->setVehicleId("");
    }
    
    currentState = RequestState::CANCELLED;
    return true;
}

void ParkingRequest::displayRequestInfo() const {
    cout << "\n=== Parking Request Details ===" << endl;
    cout << "Request ID: " << requestId << endl;
    cout << "Vehicle ID: " << (vehicle ? vehicle->getVehicleId() : "None") << endl;
    cout << "Requested Zone: " << requestedZoneId << endl;
    cout << "Current State: " << stateToString() << endl;
    cout << "Cross Zone Allocation: " << (crossZoneAllocation ? "Yes" : "No") << endl;
    
    if (allocatedSlot != nullptr) {
        cout << "Allocated Slot: " << allocatedSlot->getSlotId() 
             << " (Zone: " << allocatedSlot->getZoneId() << ")" << endl;
    } else {
        cout << "Allocated Slot: None" << endl;
    }
    
    // Format and display times
    struct tm* timeinfo;
    
    if (requestTime > 0) {
        timeinfo = localtime(&requestTime);
        cout << "Request Time: " << put_time(timeinfo, "%Y-%m-%d %H:%M:%S") << endl;
    }
    
    if (allocationTime > 0) {
        timeinfo = localtime(&allocationTime);
        cout << "Allocation Time: " << put_time(timeinfo, "%Y-%m-%d %H:%M:%S") << endl;
    }
    
    if (releaseTime > 0) {
        timeinfo = localtime(&releaseTime);
        cout << "Release Time: " << put_time(timeinfo, "%Y-%m-%d %H:%M:%S") << endl;
    }
    
    if (currentState == RequestState::RELEASED) {
        cout << "Duration: " << fixed << setprecision(2) << calculateDuration() << " minutes" << endl;
    }
}

string ParkingRequest::stateToString() const {
    switch(currentState) {
        case RequestState::REQUESTED: return "REQUESTED";
        case RequestState::ALLOCATED: return "ALLOCATED";
        case RequestState::OCCUPIED: return "OCCUPIED";
        case RequestState::RELEASED: return "RELEASED";
        case RequestState::CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}

double ParkingRequest::calculateDuration() const {
    if (currentState != RequestState::RELEASED || releaseTime == 0) {
        return 0.0;
    }
    
    time_t endTime = releaseTime;
    time_t startTime = allocationTime > 0 ? allocationTime : requestTime;
    
    return difftime(endTime, startTime) / 60.0; // Convert to minutes
}

bool ParkingRequest::isActive() const {
    return (currentState == RequestState::REQUESTED || 
            currentState == RequestState::ALLOCATED || 
            currentState == RequestState::OCCUPIED);
}