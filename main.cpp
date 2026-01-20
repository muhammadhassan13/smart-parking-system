#include <iostream>
#include "Zone.h"
#include "Vehicle.h"
using namespace std;

// Helper functions
void clearScreen() {
    system("cls");
}

int getChoice(int min, int max) {
    int choice;
    cout << endl;
    while(true) {
        cout << "Enter choice (" << min << "-" << max << "): ";
        cin >> choice;
        if(cin.fail() || choice < min || choice > max) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid Input. Please try again" << endl;
        } else {
            cin.ignore();
            return choice;
        }
    }
}

void testPhase1() {
    clearScreen();
    cout << "=== PHASE 1 TESTING: Basic Structures ===" << endl;
    
    // Test 1: Create Zones
    cout << "\n--- Test 1: Creating Zones ---" << endl;
    Zone zoneA("Z1", "Downtown", 3);
    Zone zoneB("Z2", "Uptown", 2);
    
    zoneA.displayZoneInfo();
    zoneB.displayZoneInfo();
    
    // Test 2: Add Areas to Zones
    cout << "\n--- Test 2: Adding Parking Areas ---" << endl;
    zoneA.addArea("A1", 5);
    zoneA.addArea("A2", 3);
    zoneB.addArea("B1", 4);
    
    zoneA.displayAllAreas();
    zoneB.displayAllAreas();
    
    // Test 3: Add Slots to Areas
    cout << "\n--- Test 3: Adding Parking Slots ---" << endl;
    
    // Get areas first
    ParkingArea* area1 = zoneA.findArea("A1");
    ParkingArea* area2 = zoneA.findArea("A2");
    ParkingArea* area3 = zoneB.findArea("B1");
    
    if (area1) {
        area1->addSlot("Z1-A1-S1");
        area1->addSlot("Z1-A1-S2");
        area1->addSlot("Z1-A1-S3");
        area1->displayAllSlots();
    }
    
    if (area2) {
        area2->addSlot("Z1-A2-S1");
        area2->displayAllSlots();
    }
    
    if (area3) {
        area3->addSlot("Z2-B1-S1");
        area3->addSlot("Z2-B1-S2");
        area3->displayAllSlots();
    }
    
    // Test 4: Create Vehicles
    cout << "\n--- Test 4: Creating Vehicles ---" << endl;
    Vehicle car1("CAR001", "Sedan", "Z1");
    Vehicle car2("CAR002", "SUV", "Z2");
    
    car1.displayVehicleInfo();
    car2.displayVehicleInfo();
    
    // Test 5: Test slot availability
    cout << "\n--- Test 5: Testing Slot Availability ---" << endl;
    cout << "\nZone A Available Slots: " << zoneA.getAvailableSlots() << endl;
    cout << "Zone B Available Slots: " << zoneB.getAvailableSlots() << endl;
    
    // Test 6: Find available slot
    cout << "\n--- Test 6: Finding Available Slot ---" << endl;
    ParkingSlot* availableSlot = zoneA.findAvailableSlotInZone();
    if (availableSlot) {
        cout << "Found available slot in Zone A: ";
        availableSlot->displaySlotInfo();
    }
    
    // Test 7: Test slot reservation
    cout << "\n--- Test 7: Testing Slot Reservation ---" << endl;
    if (availableSlot) {
        availableSlot->setAvailability(false);
        availableSlot->setVehicleId("CAR001");
        cout << "After reserving slot for CAR001:" << endl;
        availableSlot->displaySlotInfo();
        
        // Check availability again
        cout << "\nZone A Available Slots after reservation: " << zoneA.getAvailableSlots() << endl;
    }
    
    cout << "\n=== PHASE 1 TESTING COMPLETE ===" << endl;
}

void mainMenu() {
    int choice;
    
    do {
        clearScreen();
        cout << "=== SMART PARKING SYSTEM - MAIN MENU ===" << endl;
        cout << "1. Run Phase 1 Tests" << endl;
        cout << "2. Exit" << endl;
        
        choice = getChoice(1, 2);
        
        switch(choice) {
            case 1:
                testPhase1();
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            case 2:
                cout << "Exiting program..." << endl;
                break;
        }
    } while(choice != 2);
}

int main() {
    mainMenu();
    return 0;
}