#ifndef PARKINGAREA_H
#define PARKINGAREA_H

#include <string>
#include "ParkingSlot.h"
using namespace std;

class ParkingArea {
private:
    string areaId;
    string zoneId;
    ParkingSlot* slots; // Array of parking slots
    int maxSlots;
    int currentSlots;
    
public:
    ParkingArea();
    ParkingArea(const string& areaId, const string& zoneId, int maxSlots);
    ~ParkingArea();
    
    // Getters
    string getAreaId() const;
    string getZoneId() const;
    int getMaxSlots() const;
    int getCurrentSlots() const;
    
    // Slot management
    bool addSlot(const string& slotId);
    ParkingSlot* findSlot(const string& slotId);
    ParkingSlot* getFirstAvailableSlot();
    void displayAllSlots() const;
    
    // Statistics
    int countAvailableSlots() const;
};

#endif