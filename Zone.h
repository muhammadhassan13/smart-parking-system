#ifndef ZONE_H
#define ZONE_H

#include <string>
#include "ParkingArea.h"
using namespace std;

class Zone {
private:
    string zoneId;
    string zoneName;
    ParkingArea** areas; // Array of pointers to ParkingArea
    int maxAreas;
    int currentAreas;
    
public:
    Zone();
    Zone(const string& zoneId, const string& zoneName, int maxAreas);
    ~Zone();
    
    // Getters
    string getZoneId() const;
    string getZoneName() const;
    int getMaxAreas() const;
    int getCurrentAreas() const;
    
    // Area management
    bool addArea(const string& areaId, int maxSlots);
    ParkingArea* findArea(const string& areaId);
    void displayAllAreas() const;
    
    // Slot availability
    int getTotalSlots() const;
    int getAvailableSlots() const;
    ParkingSlot* findAvailableSlotInZone();
    
    // Utility
    void displayZoneInfo() const;
};

#endif