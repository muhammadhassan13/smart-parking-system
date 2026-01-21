#ifndef ALLOCATIONENGINE_H
#define ALLOCATIONENGINE_H

#include "Zone.h"
#include "ParkingRequest.h"

// Forward declaration
class ParkingRequest;

class AllocationEngine {
private:
    Zone** zones; // Array of zones
    int zoneCount;
    int maxZones;
    
public:
    AllocationEngine(int maxZones);
    ~AllocationEngine();
    
    // Zone management
    bool addZone(Zone* zone);
    Zone* findZone(const string& zoneId);
    
    // Allocation logic
    ParkingSlot* allocateSlot(ParkingRequest* request);
    ParkingSlot* allocateCrossZone(ParkingRequest* request);
    
    // Utility
    void displayAllZones() const;
    int getTotalAvailableSlots() const;
    
private:
    Zone* getNextAvailableZone(const string& currentZoneId);
};

#endif