#include "ParkingSystem.h"
#include <iostream>
#include <iomanip>
using namespace std;

ParkingSystem::ParkingSystem(int maxZones, int maxVehicles) 
    : maxZones(maxZones), maxVehicles(maxVehicles),
      zoneCount(0), vehicleCount(0),
      nextVehicleId(1000), nextRequestId(1000) {
    
    // Allocate arrays
    zones = new Zone*[maxZones];
    vehicles = new Vehicle*[maxVehicles];
    
    // Initialize pointers to nullptr
    for (int i = 0; i < maxZones; i++) {
        zones[i] = nullptr;
    }
    for (int i = 0; i < maxVehicles; i++) {
        vehicles[i] = nullptr;
    }
    
    // Create core components
    allocationEngine = new AllocationEngine(maxZones);
    requestManager = new RequestManager();
    rollbackManager = new RollbackManager();
    
    // Initialize with default zones
    initializeDefaultZones();
}

ParkingSystem::~ParkingSystem() {
    // Delete zones
    for (int i = 0; i < zoneCount; i++) {
        delete zones[i];
    }
    delete[] zones;
    
    // Delete vehicles
    for (int i = 0; i < vehicleCount; i++) {
        delete vehicles[i];
    }
    delete[] vehicles;
    
    // Delete core components
    delete allocationEngine;
    delete requestManager;
    delete rollbackManager;
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
        cout << "Error: Cannot add more zones. Maximum reached." << endl;
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
    
    return zone->addArea(areaId, maxSlots);
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
    
    return area->addSlot(slotId);
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
    string id = "V" + to_string(nextVehicleId);
    nextVehicleId++;
    return id;
}

string ParkingSystem::generateRequestId() {
    string id = "R" + to_string(nextRequestId);
    nextRequestId++;
    return id;
}

bool ParkingSystem::addVehicle(const string& vehicleType, const string& preferredZone) {
    if (vehicleCount >= maxVehicles) {
        cout << "Error: Cannot add more vehicles. Maximum reached." << endl;
        return false;
    }
    
    string vehicleId = generateVehicleId();
    vehicles[vehicleCount] = new Vehicle(vehicleId, vehicleType, preferredZone);
    vehicleCount++;
    
    cout << "Vehicle " << vehicleId << " added successfully." << endl;
    return true;
}

Vehicle* ParkingSystem::findVehicle(const string& vehicleId) const {
    for (int i = 0; i < vehicleCount; i++) {
        if (vehicles[i]->getVehicleId() == vehicleId) {
            return vehicles[i];
        }
    }
    return nullptr;
}

void ParkingSystem::displayAllVehicles() const {
    cout << "\n=== REGISTERED VEHICLES ===" << endl;
    cout << "Total Vehicles: " << vehicleCount << endl;
    
    if (vehicleCount == 0) {
        cout << "No vehicles registered." << endl;
        return;
    }
    
    for (int i = 0; i < vehicleCount; i++) {
        cout << i+1 << ". ";
        vehicles[i]->displayVehicleInfo();
    }
}

string ParkingSystem::createParkingRequest(const string& vehicleId, const string& requestedZone) {
    Vehicle* vehicle = findVehicle(vehicleId);
    if (vehicle == nullptr) {
        // If vehicle doesn't exist, create it temporarily
        vehicle = new Vehicle(vehicleId, "Unknown", requestedZone);
    }
    
    string requestId = generateRequestId();
    ParkingRequest* request = new ParkingRequest(requestId, vehicle, requestedZone);
    
    if (requestManager->addRequest(request)) {
        cout << "Parking request " << requestId << " created successfully." << endl;
        return requestId;
    } else {
        delete request;
        return "";
    }
}

bool ParkingSystem::allocateSlotToRequest(const string& requestId) {
    ParkingRequest* request = requestManager->findRequest(requestId);
    if (request == nullptr) {
        cout << "Error: Request " << requestId << " not found." << endl;
        return false;
    }
    
    ParkingSlot* slot = allocationEngine->allocateSlot(request);
    if (slot == nullptr) {
        cout << "Error: No available slots." << endl;
        return false;
    }
    
    bool crossZone = (slot->getZoneId() != request->getRequestedZoneId());
    bool success = request->allocateSlot(slot, crossZone);
    
    if (success) {
        rollbackManager->recordAllocation(requestId, slot->getSlotId(), slot->getZoneId());
        cout << "Slot allocated successfully!" << endl;
        if (crossZone) {
            cout << "Note: Cross-zone allocation. Extra cost applies." << endl;
        }
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
        cout << "Request marked as OCCUPIED." << endl;
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
        cout << "Request marked as RELEASED." << endl;
        cout << "Parking Duration: " << fixed << setprecision(2) 
             << request->calculateDuration() << " minutes" << endl;
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
        cout << "Request cancelled successfully." << endl;
    }
    
    return success;
}

bool ParkingSystem::rollbackLastOperation() {
    return rollbackManager->rollbackLastOperation(requestManager, allocationEngine);
}

bool ParkingSystem::rollbackLastKOperations(int k) {
    return rollbackManager->rollbackLastKOperations(k, requestManager, allocationEngine);
}

void ParkingSystem::displaySystemStatus() const {
    cout << "\n=== PARKING SYSTEM STATUS ===" << endl;
    cout << "Zones: " << zoneCount << endl;
    cout << "Total Slots: " << getTotalSlots() << endl;
    cout << "Available Slots: " << getAvailableSlots() << endl;
    cout << "Registered Vehicles: " << vehicleCount << endl;
    cout << "Total Requests: " << getTotalRequests() << endl;
    cout << "Active Requests: " << getActiveRequests() << endl;
    cout << "Available Rollbacks: " << rollbackManager->getAvailableRollbacks() << endl;
    
    // Display zone details
    cout << "\n--- Zone Details ---" << endl;
    for (int i = 0; i < zoneCount; i++) {
        zones[i]->displayZoneInfo();
        cout << endl;
    }
}

void ParkingSystem::displayZoneAnalytics() const {
    cout << "\n=== ZONE ANALYTICS ===" << endl;
    
    for (int i = 0; i < zoneCount; i++) {
        Zone* zone = zones[i];
        int totalSlots = zone->getTotalSlots();
        int availableSlots = zone->getAvailableSlots();
        int occupiedSlots = totalSlots - availableSlots;
        double utilization = (totalSlots > 0) ? (occupiedSlots * 100.0 / totalSlots) : 0.0;
        
        cout << "\nZone: " << zone->getZoneName() << " (" << zone->getZoneId() << ")" << endl;
        cout << "Total Slots: " << totalSlots << endl;
        cout << "Occupied Slots: " << occupiedSlots << endl;
        cout << "Available Slots: " << availableSlots << endl;
        cout << "Utilization Rate: " << fixed << setprecision(2) << utilization << "%" << endl;
    }
}

void ParkingSystem::displayRequestAnalytics() const {
    cout << "\n=== REQUEST ANALYTICS ===" << endl;
    
    int total = requestManager->getRequestCount();
    int completed = requestManager->countByState(RequestState::RELEASED);
    int cancelled = requestManager->countByState(RequestState::CANCELLED);
    int active = requestManager->countByState(RequestState::ALLOCATED) + 
                 requestManager->countByState(RequestState::OCCUPIED);
    
    cout << "Total Requests: " << total << endl;
    cout << "Completed: " << completed << " (" 
         << (total > 0 ? (completed * 100.0 / total) : 0) << "%)" << endl;
    cout << "Cancelled: " << cancelled << " (" 
         << (total > 0 ? (cancelled * 100.0 / total) : 0) << "%)" << endl;
    cout << "Active: " << active << " (" 
         << (total > 0 ? (active * 100.0 / total) : 0) << "%)" << endl;
    cout << "Average Parking Duration: " << fixed << setprecision(2) 
         << requestManager->getAverageDuration() << " minutes" << endl;
}

void ParkingSystem::displayPeakUsage() const {
    cout << "\n=== PEAK USAGE ANALYSIS ===" << endl;
    
    // Simplified peak usage - zone with least available slots
    Zone* busiestZone = nullptr;
    int minAvailable = 1000000;
    
    for (int i = 0; i < zoneCount; i++) {
        int available = zones[i]->getAvailableSlots();
        if (available < minAvailable) {
            minAvailable = available;
            busiestZone = zones[i];
        }
    }
    
    if (busiestZone != nullptr) {
        cout << "Busiest Zone: " << busiestZone->getZoneName() 
             << " (" << busiestZone->getZoneId() << ")" << endl;
        cout << "Available Slots: " << minAvailable << endl;
        cout << "Utilization: " 
             << (busiestZone->getTotalSlots() - minAvailable) * 100.0 / busiestZone->getTotalSlots() 
             << "%" << endl;
    }
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