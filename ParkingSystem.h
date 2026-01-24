#ifndef PARKINGSYSTEM_H
#define PARKINGSYSTEM_H

#include "Zone.h"
#include "AllocationEngine.h"
#include "RequestManager.h"
#include "RollbackManager.h"
#include "RequestQueue.h"
#include "VehicleBST.h"
#include <string>
using namespace std;

class ParkingSystem {
private:
    Zone** zones;
    AllocationEngine* allocationEngine;
    RequestManager* requestManager;
    RollbackManager* rollbackManager;
    RequestQueue* requestQueue;
    VehicleBST* vehicleBST;
    
    int zoneCount;
    int maxZones;
    
    // Counters for ID generation
    int nextVehicleId;
    int nextRequestId;
    
public:
    ParkingSystem(int maxZones = 10);
    ~ParkingSystem();
    
    // Zone management
    bool addZone(const string& zoneId, const string& zoneName, int maxAreas);
    bool addAreaToZone(const string& zoneId, const string& areaId, int maxSlots);
    bool addSlotToArea(const string& zoneId, const string& areaId, const string& slotId);
    Zone* findZone(const string& zoneId) const;
    
    // Vehicle management (using BST)
    bool addVehicle(const string& vehicleType, const string& preferredZone);
    Vehicle* findVehicle(const string& vehicleId) const;
    void displayAllVehicles() const;
    
    // Request management (with Queue)
    string createParkingRequest(const string& vehicleId, const string& requestedZone);
    bool processNextRequest();  // Process from queue
    bool allocateSlotToRequest(const string& requestId);
    bool markAsOccupied(const string& requestId);
    bool markAsReleased(const string& requestId);
    bool cancelRequest(const string& requestId);
    
    // Queue operations
    void displayPendingRequests() const;
    int getPendingRequestCount() const;
    
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
    
    // Testing
    void runTestSuite();
    
private:
    void initializeDefaultZones();
    string generateVehicleId();
    string generateRequestId();
};

#endif