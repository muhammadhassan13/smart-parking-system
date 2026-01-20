#include "Zone.h"
#include <iostream>
using namespace std;

Zone::Zone() : maxAreas(0), currentAreas(0), areas(nullptr) {}

Zone::Zone(const string& zoneId, const string& zoneName, int maxAreas)
    : zoneId(zoneId), zoneName(zoneName), maxAreas(maxAreas), currentAreas(0) {
    areas = new ParkingArea*[maxAreas];
    for (int i = 0; i < maxAreas; i++) {
        areas[i] = nullptr;
    }
}

Zone::~Zone() {
    for (int i = 0; i < currentAreas; i++) {
        delete areas[i];
    }
    delete[] areas;
}

string Zone::getZoneId() const {
    return zoneId;
}

string Zone::getZoneName() const {
    return zoneName;
}

int Zone::getMaxAreas() const {
    return maxAreas;
}

int Zone::getCurrentAreas() const {
    return currentAreas;
}

bool Zone::addArea(const string& areaId, int maxSlots) {
    if (currentAreas >= maxAreas) {
        cout << "Error: Cannot add more areas. Zone capacity reached!" << endl;
        return false;
    }
    
    areas[currentAreas] = new ParkingArea(areaId, zoneId, maxSlots);
    currentAreas++;
    return true;
}

ParkingArea* Zone::findArea(const string& areaId) {
    for (int i = 0; i < currentAreas; i++) {
        if (areas[i]->getAreaId() == areaId) {
            return areas[i];
        }
    }
    return nullptr;
}

void Zone::displayAllAreas() const {
    cout << "\n=== Zone: " << zoneName << " (" << zoneId << ") ===" << endl;
    cout << "Total Areas: " << currentAreas << "/" << maxAreas << endl;
    
    if (currentAreas == 0) {
        cout << "No areas created yet." << endl;
        return;
    }
    
    for (int i = 0; i < currentAreas; i++) {
        cout << "\nArea " << i+1 << ": " << areas[i]->getAreaId() << endl;
        cout << "Total Slots: " << areas[i]->getCurrentSlots() << "/" << areas[i]->getMaxSlots() << endl;
        cout << "Available Slots: " << areas[i]->countAvailableSlots() << endl;
    }
}

int Zone::getTotalSlots() const {
    int total = 0;
    for (int i = 0; i < currentAreas; i++) {
        total += areas[i]->getCurrentSlots();
    }
    return total;
}

int Zone::getAvailableSlots() const {
    int available = 0;
    for (int i = 0; i < currentAreas; i++) {
        available += areas[i]->countAvailableSlots();
    }
    return available;
}

ParkingSlot* Zone::findAvailableSlotInZone() {
    for (int i = 0; i < currentAreas; i++) {
        ParkingSlot* slot = areas[i]->getFirstAvailableSlot();
        if (slot != nullptr) {
            return slot;
        }
    }
    return nullptr;
}

void Zone::displayZoneInfo() const {
    cout << "\n=== Zone Information ===" << endl;
    cout << "Zone ID: " << zoneId << endl;
    cout << "Zone Name: " << zoneName << endl;
    cout << "Areas: " << currentAreas << "/" << maxAreas << endl;
    cout << "Total Slots: " << getTotalSlots() << endl;
    cout << "Available Slots: " << getAvailableSlots() << endl;
}