#include "ParkingSystem.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

ParkingSystem::ParkingSystem(int maxZones) 
    : maxZones(maxZones), zoneCount(0),
      nextVehicleId(1000), nextRequestId(1000) {
    
    // Allocate zones array
    zones = new Zone*[maxZones];
    for (int i = 0; i < maxZones; i++) {
        zones[i] = nullptr;
    }
    
    // Create core components
    allocationEngine = new AllocationEngine(maxZones);
    requestManager = new RequestManager();
    rollbackManager = new RollbackManager();
    requestQueue = new RequestQueue();
    vehicleBST = new VehicleBST();
    
    // Initialize with default zones
    initializeDefaultZones();
    
    cout << "Parking System initialized with " << zoneCount << " zones." << endl;
}

ParkingSystem::~ParkingSystem() {
    // Delete zones
    for (int i = 0; i < zoneCount; i++) {
        delete zones[i];
    }
    delete[] zones;
    
    // Delete core components
    delete allocationEngine;
    delete requestManager;
    delete rollbackManager;
    delete requestQueue;
    delete vehicleBST;
    
    cout << "Parking System destroyed." << endl;
}

void ParkingSystem::initializeDefaultZones() {
    // Create default zones
    addZone("Z1", "Downtown", 3);
    addZone("Z2", "Uptown", 2);
    addZone("Z3", "Midtown", 2);
    
    // Add areas to zones
    addAreaToZone("Z1", "A1", 3);
    addAreaToZone("Z1", "A2", 2);
    addAreaToZone("Z2", "B1", 2);
    addAreaToZone("Z3", "C1", 3);
    
    // Add slots to areas
    addSlotToArea("Z1", "A1", "Z1-A1-S1");
    addSlotToArea("Z1", "A1", "Z1-A1-S2");
    addSlotToArea("Z1", "A1", "Z1-A1-S3");
    addSlotToArea("Z1", "A2", "Z1-A2-S1");
    addSlotToArea("Z1", "A2", "Z1-A2-S2");
    addSlotToArea("Z2", "B1", "Z2-B1-S1");
    addSlotToArea("Z2", "B1", "Z2-B1-S2");
    addSlotToArea("Z3", "C1", "Z3-C1-S1");
    addSlotToArea("Z3", "C1", "Z3-C1-S2");
    addSlotToArea("Z3", "C1", "Z3-C1-S3");
}

bool ParkingSystem::addZone(const string& zoneId, const string& zoneName, int maxAreas) {
    if (zoneCount >= maxZones) {
        cout << "Error: Cannot add more zones. Maximum " << maxZones << " reached." << endl;
        return false;
    }
    
    // Check if zone already exists
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i]->getZoneId() == zoneId) {
            cout << "Error: Zone " << zoneId << " already exists." << endl;
            return false;
        }
    }
    
    zones[zoneCount] = new Zone(zoneId, zoneName, maxAreas);
    allocationEngine->addZone(zones[zoneCount]);
    zoneCount++;
    
    cout << "Zone " << zoneId << " (" << zoneName << ") added successfully." << endl;
    return true;
}

bool ParkingSystem::addAreaToZone(const string& zoneId, const string& areaId, int maxSlots) {
    Zone* zone = findZone(zoneId);
    if (zone == nullptr) {
        cout << "Error: Zone " << zoneId << " not found." << endl;
        return false;
    }
    
    bool success = zone->addArea(areaId, maxSlots);
    if (success) {
        cout << "Area " << areaId << " added to zone " << zoneId << " with capacity " << maxSlots << " slots." << endl;
    }
    return success;
}

bool ParkingSystem::addSlotToArea(const string& zoneId, const string& areaId, const string& slotId) {
    Zone* zone = findZone(zoneId);
    if (zone == nullptr) {
        cout << "Error: Zone " << zoneId << " not found." << endl;
        return false;
    }
    
    ParkingArea* area = zone->findArea(areaId);
    if (area == nullptr) {
        cout << "Error: Area " << areaId << " not found in zone " << zoneId << endl;
        return false;
    }
    
    bool success = area->addSlot(slotId);
    if (success) {
        cout << "Slot " << slotId << " added to area " << areaId << " in zone " << zoneId << endl;
    }
    return success;
}

Zone* ParkingSystem::findZone(const string& zoneId) const {
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i]->getZoneId() == zoneId) {
            return zones[i];
        }
    }
    return nullptr;
}

string ParkingSystem::generateVehicleId() {
    stringstream ss;
    ss << "V" << nextVehicleId;
    nextVehicleId++;
    return ss.str();
}

string ParkingSystem::generateRequestId() {
    stringstream ss;
    ss << "R" << nextRequestId;
    nextRequestId++;
    return ss.str();
}

bool ParkingSystem::addVehicle(const string& vehicleType, const string& preferredZone) {
    string vehicleId = generateVehicleId();
    Vehicle* vehicle = new Vehicle(vehicleId, vehicleType, preferredZone);
    
    if (vehicleBST->insert(vehicle)) {
        cout << "Vehicle " << vehicleId << " (" << vehicleType << ") registered successfully." << endl;
        cout << "Preferred Zone: " << preferredZone << endl;
        return true;
    } else {
        delete vehicle;
        cout << "Error: Failed to register vehicle." << endl;
        return false;
    }
}

Vehicle* ParkingSystem::findVehicle(const string& vehicleId) const {
    return vehicleBST->search(vehicleId);
}

void ParkingSystem::displayAllVehicles() const {
    vehicleBST->displayInorder();
}

string ParkingSystem::createParkingRequest(const string& vehicleId, const string& requestedZone) {
    Vehicle* vehicle = findVehicle(vehicleId);
    if (vehicle == nullptr) {
        // If vehicle doesn't exist, create and register it
        cout << "Vehicle " << vehicleId << " not found. Auto-registering..." << endl;
        vehicle = new Vehicle(vehicleId, "Unknown", requestedZone);
        if (!vehicleBST->insert(vehicle)) {
            delete vehicle;
            cout << "Error: Failed to auto-register vehicle." << endl;
            return "";
        }
        cout << "Vehicle " << vehicleId << " auto-registered successfully." << endl;
    }
    
    string requestId = generateRequestId();
    ParkingRequest* request = new ParkingRequest(requestId, vehicle, requestedZone);
    
    // Add to queue first
    if (requestQueue->enqueue(request)) {
        cout << "Parking request " << requestId << " created successfully." << endl;
        cout << "Vehicle: " << vehicleId << " -> Zone: " << requestedZone << endl;
        cout << "Request added to queue. Use 'Process Next Request' to allocate." << endl;
        return requestId;
    } else {
        delete request;
        cout << "Error: Request queue is full. Cannot create new request." << endl;
        return "";
    }
}

bool ParkingSystem::processNextRequest() {
    ParkingRequest* request = requestQueue->dequeue();
    if (request == nullptr) {
        cout << "No pending requests in queue." << endl;
        return false;
    }
    
    // Add to request manager
    requestManager->addRequest(request);
    
    cout << "Processing request " << request->getRequestId() << "..." << endl;
    cout << "Vehicle: " << request->getVehicle()->getVehicleId() << endl;
    cout << "Requested Zone: " << request->getRequestedZoneId() << endl;
    
    // Try to allocate immediately
    bool allocated = allocateSlotToRequest(request->getRequestId());
    
    if (allocated) {
        cout << "Request " << request->getRequestId() << " processed and allocated successfully." << endl;
    } else {
        cout << "Request " << request->getRequestId() << " processed but could not allocate (no available slots)." << endl;
        cout << "Request remains in REQUESTED state. Try again later." << endl;
    }
    
    return allocated;
}

bool ParkingSystem::allocateSlotToRequest(const string& requestId) {
    ParkingRequest* request = requestManager->findRequest(requestId);
    if (request == nullptr) {
        // Check if request is still in queue
        cout << "Error: Request " << requestId << " not found in active requests." << endl;
        cout << "Note: Requests must be processed from queue first." << endl;
        return false;
    }
    
    if (request->getCurrentState() != RequestState::REQUESTED) {
        cout << "Error: Request " << requestId << " is not in REQUESTED state." << endl;
        cout << "Current state: " << request->stateToString() << endl;
        return false;
    }
    
    ParkingSlot* slot = allocationEngine->allocateSlot(request);
    if (slot == nullptr) {
        cout << "Error: No available slots in system." << endl;
        return false;
    }
    
    bool crossZone = (slot->getZoneId() != request->getRequestedZoneId());
    bool success = request->allocateSlot(slot, crossZone);
    
    if (success) {
        rollbackManager->recordAllocation(requestId, slot->getSlotId(), slot->getZoneId());
        cout << "Slot allocated successfully!" << endl;
        cout << "Allocated Slot: " << slot->getSlotId() << " in Zone " << slot->getZoneId() << endl;
        if (crossZone) {
            cout << "Cross-zone allocation. Extra cost applies." << endl;
        }
    } else {
        cout << "Error: Failed to allocate slot." << endl;
    }
    
    return success;
}

bool ParkingSystem::markAsOccupied(const string& requestId) {
    ParkingRequest* request = requestManager->findRequest(requestId);
    if (request == nullptr) {
        cout << "Error: Request " << requestId << " not found." << endl;
        return false;
    }
    
    RequestState previousState = request->getCurrentState();
    bool success = request->markAsOccupied();
    
    if (success) {
        rollbackManager->recordStateChange(requestId, previousState);
        cout << "Request " << requestId << " marked as OCCUPIED." << endl;
        cout << "Vehicle is now parked in slot " << request->getAllocatedSlot()->getSlotId() << endl;
    } else {
        cout << "Error: Cannot mark as OCCUPIED." << endl;
        cout << "Current state: " << request->stateToString() << endl;
        cout << "Required state: ALLOCATED" << endl;
    }
    
    return success;
}

bool ParkingSystem::markAsReleased(const string& requestId) {
    ParkingRequest* request = requestManager->findRequest(requestId);
    if (request == nullptr) {
        cout << "Error: Request " << requestId << " not found." << endl;
        return false;
    }
    
    RequestState previousState = request->getCurrentState();
    bool success = request->markAsReleased();
    
    if (success) {
        rollbackManager->recordStateChange(requestId, previousState);
        cout << "Request " << requestId << " marked as RELEASED." << endl;
        cout << "Slot " << request->getAllocatedSlot()->getSlotId() << " is now available." << endl;
        cout << "Parking Duration: " << fixed << setprecision(2) 
             << request->calculateDuration() << " minutes" << endl;
    } else {
        cout << "Error: Cannot mark as RELEASED." << endl;
        cout << "Current state: " << request->stateToString() << endl;
        cout << "Required state: OCCUPIED" << endl;
    }
    
    return success;
}

bool ParkingSystem::cancelRequest(const string& requestId) {
    ParkingRequest* request = requestManager->findRequest(requestId);
    if (request == nullptr) {
        cout << "Error: Request " << requestId << " not found." << endl;
        return false;
    }
    
    RequestState previousState = request->getCurrentState();
    bool success = request->cancelRequest();
    
    if (success) {
        rollbackManager->recordCancellation(requestId);
        cout << "Request " << requestId << " cancelled successfully." << endl;
        
        if (previousState == RequestState::ALLOCATED) {
            cout << "Slot " << request->getAllocatedSlot()->getSlotId() << " has been freed." << endl;
        }
    } else {
        cout << "Error: Cannot cancel request." << endl;
        cout << "Current state: " << request->stateToString() << endl;
        cout << "Cancellation only allowed from REQUESTED or ALLOCATED states." << endl;
    }
    
    return success;
}

void ParkingSystem::displayPendingRequests() const {
    requestQueue->displayQueue();
}

int ParkingSystem::getPendingRequestCount() const {
    return requestQueue->getSize();
}

bool ParkingSystem::rollbackLastOperation() {
    cout << "Attempting to rollback last operation..." << endl;
    bool success = rollbackManager->rollbackLastOperation(requestManager, allocationEngine);
    if (success) {
        cout << "Rollback successful!" << endl;
    } else {
        cout << "Rollback failed or no operations to rollback." << endl;
    }
    return success;
}

bool ParkingSystem::rollbackLastKOperations(int k) {
    cout << "Attempting to rollback last " << k << " operations..." << endl;
    bool success = rollbackManager->rollbackLastKOperations(k, requestManager, allocationEngine);
    if (success) {
        cout << "Rollback of " << k << " operations successful!" << endl;
    } else {
        cout << "Rollback failed." << endl;
    }
    return success;
}

void ParkingSystem::displaySystemStatus() const {
    cout << "\n=======================================" << endl;
    cout << "       SYSTEM STATUS REPORT" << endl;
    cout << "=======================================" << endl;
    
    cout << "\n--- Zones Summary ---" << endl;
    cout << "Total Zones: " << zoneCount << endl;
    for (int i = 0; i < zoneCount; i++) {
        Zone* zone = zones[i];
        cout << "  " << zone->getZoneId() << " (" << zone->getZoneName() << "): "
             << zone->getAvailableSlots() << "/" << zone->getTotalSlots() << " slots available" << endl;
    }
    
    cout << "\n--- Capacity Summary ---" << endl;
    cout << "Total Slots: " << getTotalSlots() << endl;
    cout << "Available Slots: " << getAvailableSlots() << endl;
    cout << "Occupied Slots: " << (getTotalSlots() - getAvailableSlots()) << endl;
    
    cout << "\n--- Requests Summary ---" << endl;
    cout << "Total Requests: " << getTotalRequests() << endl;
    cout << "Active Requests: " << getActiveRequests() << endl;
    cout << "Pending in Queue: " << getPendingRequestCount() << endl;
    
    cout << "\n--- Vehicles Summary ---" << endl;
    cout << "Registered Vehicles: " << vehicleBST->getCount() << endl;
    
    cout << "\n--- Rollback Status ---" << endl;
    cout << "Available Rollbacks: " << rollbackManager->getAvailableRollbacks() << endl;
    
    cout << "\n=======================================" << endl;
}

void ParkingSystem::displayZoneAnalytics() const {
    cout << "\n=======================================" << endl;
    cout << "         ZONE ANALYTICS" << endl;
    cout << "=======================================" << endl;
    
    for (int i = 0; i < zoneCount; i++) {
        Zone* zone = zones[i];
        int totalSlots = zone->getTotalSlots();
        int availableSlots = zone->getAvailableSlots();
        int occupiedSlots = totalSlots - availableSlots;
        double utilization = (totalSlots > 0) ? (occupiedSlots * 100.0 / totalSlots) : 0.0;
        
        cout << "\nZone: " << zone->getZoneName() << " (" << zone->getZoneId() << ")" << endl;
        cout << "  Total Slots: " << totalSlots << endl;
        cout << "  Occupied Slots: " << occupiedSlots << endl;
        cout << "  Available Slots: " << availableSlots << endl;
        cout << "  Utilization Rate: " << fixed << setprecision(2) << utilization << "%" << endl;
        
        // Display utilization bar
        cout << "  [";
        int barLength = 20;
        int filled = (utilization / 100.0) * barLength;
        for (int j = 0; j < barLength; j++) {
            cout << (j < filled ? "█" : "░");
        }
        cout << "]" << endl;
    }
    cout << "\n=======================================" << endl;
}

void ParkingSystem::displayRequestAnalytics() const {
    cout << "\n=======================================" << endl;
    cout << "        REQUEST ANALYTICS" << endl;
    cout << "=======================================" << endl;
    
    int total = requestManager->getRequestCount();
    int completed = requestManager->countByState(RequestState::RELEASED);
    int cancelled = requestManager->countByState(RequestState::CANCELLED);
    int allocated = requestManager->countByState(RequestState::ALLOCATED);
    int occupied = requestManager->countByState(RequestState::OCCUPIED);
    int requested = requestManager->countByState(RequestState::REQUESTED);
    
    cout << "\n--- Status Distribution ---" << endl;
    cout << "Total Requests: " << total << endl;
    cout << "  Completed (RELEASED): " << completed << endl;
    cout << "  Cancelled: " << cancelled << endl;
    cout << "  Active - ALLOCATED: " << allocated << endl;
    cout << "  Active - OCCUPIED: " << occupied << endl;
    cout << "  Pending - REQUESTED: " << requested << endl;
    
    if (total > 0) {
        cout << "\n--- Percentages ---" << endl;
        cout << "  Completion Rate: " << fixed << setprecision(1) 
             << (completed * 100.0 / total) << "%" << endl;
        cout << "  Cancellation Rate: " << (cancelled * 100.0 / total) << "%" << endl;
        cout << "  Active Rate: " << ((allocated + occupied) * 100.0 / total) << "%" << endl;
    }
    
    cout << "\n--- Performance Metrics ---" << endl;
    cout << "Average Parking Duration: " << fixed << setprecision(2) 
         << requestManager->getAverageDuration() << " minutes" << endl;
    
    cout << "\n=======================================" << endl;
}

void ParkingSystem::displayPeakUsage() const {
    cout << "\n=======================================" << endl;
    cout << "        PEAK USAGE ANALYSIS" << endl;
    cout << "=======================================" << endl;
    
    Zone* busiestZone = nullptr;
    Zone* mostAvailableZone = nullptr;
    int maxUtilization = 0;
    int minUtilization = 100;
    
    for (int i = 0; i < zoneCount; i++) {
        Zone* zone = zones[i];
        int totalSlots = zone->getTotalSlots();
        int availableSlots = zone->getAvailableSlots();
        int occupiedSlots = totalSlots - availableSlots;
        int utilization = (totalSlots > 0) ? (occupiedSlots * 100 / totalSlots) : 0;
        
        if (utilization > maxUtilization) {
            maxUtilization = utilization;
            busiestZone = zone;
        }
        
        if (utilization < minUtilization) {
            minUtilization = utilization;
            mostAvailableZone = zone;
        }
    }
    
    if (busiestZone != nullptr) {
        cout << "\n Busiest Zone:" << endl;
        cout << "  " << busiestZone->getZoneName() << " (" << busiestZone->getZoneId() << ")" << endl;
        cout << "  Utilization: " << maxUtilization << "%" << endl;
        cout << "  Available Slots: " << busiestZone->getAvailableSlots() 
             << "/" << busiestZone->getTotalSlots() << endl;
    }
    
    if (mostAvailableZone != nullptr) {
        cout << "\n Most Available Zone:" << endl;
        cout << "  " << mostAvailableZone->getZoneName() << " (" << mostAvailableZone->getZoneId() << ")" << endl;
        cout << "  Utilization: " << minUtilization << "%" << endl;
        cout << "  Available Slots: " << mostAvailableZone->getAvailableSlots() 
             << "/" << mostAvailableZone->getTotalSlots() << endl;
    }
    
    cout << "\n--- Recommendations ---" << endl;
    if (maxUtilization > 80) {
        cout << " High utilization detected. Consider adding more slots to " 
             << busiestZone->getZoneName() << " zone." << endl;
    }
    
    if (minUtilization < 20) {
        cout << " Low utilization in " << mostAvailableZone->getZoneName() 
             << ". Consider promotional offers for this zone." << endl;
    }
    
    cout << "\n=======================================" << endl;
}

int ParkingSystem::getTotalSlots() const {
    int total = 0;
    for (int i = 0; i < zoneCount; i++) {
        total += zones[i]->getTotalSlots();
    }
    return total;
}

int ParkingSystem::getAvailableSlots() const {
    int total = 0;
    for (int i = 0; i < zoneCount; i++) {
        total += zones[i]->getAvailableSlots();
    }
    return total;
}

int ParkingSystem::getTotalRequests() const {
    return requestManager->getRequestCount();
}

int ParkingSystem::getActiveRequests() const {
    return requestManager->countByState(RequestState::ALLOCATED) + 
           requestManager->countByState(RequestState::OCCUPIED);
}

void ParkingSystem::runTestSuite() {
    // Note: This would integrate with TestSuite class
    cout << "Test suite execution would run here." << endl;
    cout << "Use option 14 in main menu to run comprehensive tests." << endl;
}