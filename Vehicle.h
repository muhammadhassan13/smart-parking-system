#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
using namespace std;

class Vehicle {
private:
    string vehicleId;
    string vehicleType;
    string preferredZone;
    
public:
    Vehicle();
    Vehicle(const string& vehicleId, const string& vehicleType, const string& preferredZone);
    
    // Getters
    string getVehicleId() const;
    string getVehicleType() const;
    string getPreferredZone() const;
    
    // Setters
    void setPreferredZone(const string& zone);
    
    // Utility
    void displayVehicleInfo() const;
};

#endif