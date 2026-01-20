#include "ParkingSlot.h"
#include <iostream>
using namespace std;

ParkingSlot::ParkingSlot() : isAvailable(true), vehicleId("") {}

ParkingSlot::ParkingSlot(const string& slotId, const string& zoneId) 
    : slotId(slotId), zoneId(zoneId), isAvailable(true), vehicleId("") {}

string ParkingSlot::getSlotId() const {
    return slotId;
}

string ParkingSlot::getZoneId() const {
    return zoneId;
}

bool ParkingSlot::getAvailability() const {
    return isAvailable;
}

string ParkingSlot::getVehicleId() const {
    return vehicleId;
}

void ParkingSlot::setAvailability(bool available) {
    isAvailable = available;
}

void ParkingSlot::setVehicleId(const string& vehicleId) {
    this->vehicleId = vehicleId;
}

void ParkingSlot::displaySlotInfo() const {
    cout << "Slot ID: " << slotId 
         << ", Zone: " << zoneId 
         << ", Available: " << (isAvailable ? "Yes" : "No");
    if (!isAvailable && !vehicleId.empty()) {
        cout << ", Vehicle: " << vehicleId;
    }
    cout << endl;
}