#include "Vehicle.h"
#include <iostream>
using namespace std;

Vehicle::Vehicle() : vehicleId(""), vehicleType(""), preferredZone("") {}

Vehicle::Vehicle(const string& vehicleId, const string& vehicleType, const string& preferredZone)
    : vehicleId(vehicleId), vehicleType(vehicleType), preferredZone(preferredZone) {}

string Vehicle::getVehicleId() const {
    return vehicleId;
}

string Vehicle::getVehicleType() const {
    return vehicleType;
}

string Vehicle::getPreferredZone() const {
    return preferredZone;
}

void Vehicle::setPreferredZone(const string& zone) {
    preferredZone = zone;
}

void Vehicle::displayVehicleInfo() const {
    cout << "Vehicle ID: " << vehicleId 
         << ", Type: " << vehicleType 
         << ", Preferred Zone: " << preferredZone << endl;
}