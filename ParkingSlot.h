#ifndef PARKINGSLOT_H
#define PARKINGSLOT_H

#include <string>
using namespace std;

class ParkingSlot {
private:
    string slotId;
    string zoneId;
    bool isAvailable;
    string vehicleId; // Vehicle currently parked

public:
    // Constructor
    ParkingSlot();
    ParkingSlot(const string& slotId, const string& zoneId);
    
    // Getters
    string getSlotId() const;
    string getZoneId() const;
    bool getAvailability() const;
    string getVehicleId() const;
    
    // Setters
    void setAvailability(bool available);
    void setVehicleId(const string& vehicleId);
    
    // Utility
    void displaySlotInfo() const;
};

#endif