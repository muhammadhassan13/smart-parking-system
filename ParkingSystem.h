#ifndef PARKINGSYSTEM_H
#define PARKINGSYSTEM_H

#include "Zone.h"
#include "Vehicle.h"
#include "AllocationEngine.h"
#include "RequestManager.h"
#include "RollbackManager.h"
#include <string>
using namespace std;

class ParkingSystem {
private:
    Zone** zones;
    Vehicle** vehicles;
    AllocationEngine* allocationEngine;
    RequestManager* requestManager;
    RollbackManager* rollbackManager;
    
    int zoneCount;
    int maxZones;
    int vehicleCount;
    int maxVehicles;
    
    // Counters for ID generation
    int nextVehicleId;
    int nextRequestId;
    
public:
    ParkingSystem(int maxZones = 10, int maxVehicles = 100);
    ~ParkingSystem();
    
    // Zone management
    bool addZone(const string& zoneId, const string& zoneName, int maxAreas);
    bool addAreaToZone(const string& zoneId, const string& areaId, int maxSlots);
    bool addSlotToArea(const string& zoneId, const string& areaId, const string& slotId);
    Zone* findZone(const string& zoneId) const;
    
    // Vehicle management
    bool addVehicle(const string& vehicleType, const string& preferredZone);
    Vehicle* findVehicle(const string& vehicleId) const;
    void displayAllVehicles() const;
    
    // Request management
    string createParkingRequest(const string& vehicleId, const string& requestedZone);
    bool allocateSlotToRequest(const string& requestId);
    bool markAsOccupied(const string& requestId);
    bool markAsReleased(const string& requestId);
    bool cancelRequest(const string& requestId);
    
    // Rollback operations
    bool rollbackLastOperation();
    bool rollbackLastKOperations(int k);
    
    // Analytics
    void displaySystemStatus() const;
    void displayZoneAnalytics() const;
    void displayRequestAnalytics() const;
    void displayPeakUsage() const;
    
    // Utility
    int getTotalSlots() const;
    int getAvailableSlots() const;
    int getTotalRequests() const;
    int getActiveRequests() const;
    
private:
    void initializeDefaultZones();
    string generateVehicleId();
    string generateRequestId();
};

#endif