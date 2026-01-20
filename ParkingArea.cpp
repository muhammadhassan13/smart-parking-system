#include "ParkingArea.h"
#include <iostream>
using namespace std;

ParkingArea::ParkingArea() : maxSlots(0), currentSlots(0), slots(nullptr) {}

ParkingArea::ParkingArea(const string& areaId, const string& zoneId, int maxSlots)
    : areaId(areaId), zoneId(zoneId), maxSlots(maxSlots), currentSlots(0) {
    slots = new ParkingSlot[maxSlots];
}

ParkingArea::~ParkingArea() {
    delete[] slots;
}

string ParkingArea::getAreaId() const {
    return areaId;
}

string ParkingArea::getZoneId() const {
    return zoneId;
}

int ParkingArea::getMaxSlots() const {
    return maxSlots;
}

int ParkingArea::getCurrentSlots() const {
    return currentSlots;
}

bool ParkingArea::addSlot(const string& slotId) {
    if (currentSlots >= maxSlots) {
        cout << "Error: Cannot add more slots. Area is full!" << endl;
        return false;
    }
    
    slots[currentSlots] = ParkingSlot(slotId, zoneId);
    currentSlots++;
    return true;
}

ParkingSlot* ParkingArea::findSlot(const string& slotId) {
    for (int i = 0; i < currentSlots; i++) {
        if (slots[i].getSlotId() == slotId) {
            return &slots[i];
        }
    }
    return nullptr;
}

ParkingSlot* ParkingArea::getFirstAvailableSlot() {
    for (int i = 0; i < currentSlots; i++) {
        if (slots[i].getAvailability()) {
            return &slots[i];
        }
    }
    return nullptr;
}

void ParkingArea::displayAllSlots() const {
    cout << "\n=== Area: " << areaId << " (Zone: " << zoneId << ") ===" << endl;
    cout << "Total Slots: " << currentSlots << "/" << maxSlots << endl;
    
    if (currentSlots == 0) {
        cout << "No slots created yet." << endl;
        return;
    }
    
    for (int i = 0; i < currentSlots; i++) {
        cout << i+1 << ". ";
        slots[i].displaySlotInfo();
    }
}

int ParkingArea::countAvailableSlots() const {
    int count = 0;
    for (int i = 0; i < currentSlots; i++) {
        if (slots[i].getAvailability()) {
            count++;
        }
    }
    return count;
}