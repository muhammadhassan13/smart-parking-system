#include "AllocationEngine.h"
#include <iostream>
using namespace std;

AllocationEngine::AllocationEngine(int maxZones) 
    : maxZones(maxZones), zoneCount(0) {
    zones = new Zone*[maxZones];
    for (int i = 0; i < maxZones; i++) {
        zones[i] = nullptr;
    }
}

AllocationEngine::~AllocationEngine() {
    for (int i = 0; i < zoneCount; i++) {
        // Note: We don't delete zones here as they're managed elsewhere
        zones[i] = nullptr;
    }
    delete[] zones;
}

bool AllocationEngine::addZone(Zone* zone) {
    if (zoneCount >= maxZones) {
        cout << "Error: Cannot add more zones to engine." << endl;
        return false;
    }
    
    // Check if zone already exists
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i]->getZoneId() == zone->getZoneId()) {
            cout << "Error: Zone " << zone->getZoneId() << " already exists in engine." << endl;
            return false;
        }
    }
    
    zones[zoneCount] = zone;
    zoneCount++;
    return true;
}

Zone* AllocationEngine::findZone(const string& zoneId) {
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i]->getZoneId() == zoneId) {
            return zones[i];
        }
    }
    return nullptr;
}

ParkingSlot* AllocationEngine::allocateSlot(ParkingRequest* request) {
    if (request == nullptr) {
        cout << "Error: Cannot allocate slot for null request." << endl;
        return nullptr;
    }
    
    string requestedZoneId = request->getRequestedZoneId();
    Zone* requestedZone = findZone(requestedZoneId);
    
    if (requestedZone == nullptr) {
        cout << "Error: Requested zone " << requestedZoneId << " not found." << endl;
        return nullptr;
    }
    
    // Try to allocate in requested zone first
    ParkingSlot* slot = requestedZone->findAvailableSlotInZone();
    
    if (slot != nullptr) {
        // Allocation successful in requested zone
        return slot;
    }
    
    // If requested zone is full, try cross-zone allocation
    cout << "Zone " << requestedZoneId << " is full. Attempting cross-zone allocation..." << endl;
    return allocateCrossZone(request);
}

ParkingSlot* AllocationEngine::allocateCrossZone(ParkingRequest* request) {
    Zone* currentZone = findZone(request->getRequestedZoneId());
    if (currentZone == nullptr) {
        return nullptr;
    }
    
    // Find next available zone
    Zone* nextZone = getNextAvailableZone(request->getRequestedZoneId());
    
    if (nextZone != nullptr) {
        ParkingSlot* slot = nextZone->findAvailableSlotInZone();
        if (slot != nullptr) {
            cout << "Cross-zone allocation successful! Allocated in zone " 
                 << nextZone->getZoneId() << endl;
            return slot;
        }
    }
    
    cout << "Error: No available slots in any zone." << endl;
    return nullptr;
}

Zone* AllocationEngine::getNextAvailableZone(const string& currentZoneId) {
    // Simple round-robin search for next available zone
    int currentIndex = -1;
    
    // Find current zone index
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i]->getZoneId() == currentZoneId) {
            currentIndex = i;
            break;
        }
    }
    
    if (currentIndex == -1) {
        return nullptr;
    }
    
    // Search for next zone with available slots
    for (int offset = 1; offset < zoneCount; offset++) {
        int nextIndex = (currentIndex + offset) % zoneCount;
        if (zones[nextIndex]->getAvailableSlots() > 0) {
            return zones[nextIndex];
        }
    }
    
    return nullptr;
}

void AllocationEngine::displayAllZones() const {
    cout << "\n=== All Zones in Allocation Engine ===" << endl;
    cout << "Total Zones: " << zoneCount << endl;
    
    for (int i = 0; i < zoneCount; i++) {
        zones[i]->displayZoneInfo();
        cout << endl;
    }
}

int AllocationEngine::getTotalAvailableSlots() const {
    int total = 0;
    for (int i = 0; i < zoneCount; i++) {
        total += zones[i]->getAvailableSlots();
    }
    return total;
}