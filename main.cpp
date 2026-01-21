#include <iostream>
#include "Zone.h"
#include "Vehicle.h"
#include "ParkingRequest.h"
#include "AllocationEngine.h"
using namespace std;

// Helper functions (already defined in Phase 1)
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

void testPhase2() {
    clearScreen();
    cout << "=== PHASE 2 TESTING: Parking Requests & Allocation Engine ===" << endl;
    
    // Setup test data
    cout << "\n--- Setup: Creating Zones and Vehicles ---" << endl;
    
    // Create zones
    Zone zoneA("Z1", "Downtown", 2);
    Zone zoneB("Z2", "Uptown", 2);
    Zone zoneC("Z3", "Midtown", 2);
    
    // Add areas and slots
    zoneA.addArea("A1", 2);
    zoneB.addArea("B1", 1);
    zoneC.addArea("C1", 2);
    
    ParkingArea* areaA1 = zoneA.findArea("A1");
    ParkingArea* areaB1 = zoneB.findArea("B1");
    ParkingArea* areaC1 = zoneC.findArea("C1");
    
    if (areaA1) {
        areaA1->addSlot("Z1-A1-S1");
        areaA1->addSlot("Z1-A1-S2");
    }
    
    if (areaB1) {
        areaB1->addSlot("Z2-B1-S1");
    }
    
    if (areaC1) {
        areaC1->addSlot("Z3-C1-S1");
        areaC1->addSlot("Z3-C1-S2");
    }
    
    // Create vehicles
    Vehicle car1("CAR001", "Sedan", "Z1");
    Vehicle car2("CAR002", "SUV", "Z2");
    Vehicle car3("CAR003", "Hatchback", "Z1");
    Vehicle car4("CAR004", "Truck", "Z3");
    
    cout << "\n--- Test 1: Creating Allocation Engine ---" << endl;
    AllocationEngine engine(5);
    engine.addZone(&zoneA);
    engine.addZone(&zoneB);
    engine.addZone(&zoneC);
    
    engine.displayAllZones();
    
    cout << "\n--- Test 2: Creating Parking Requests ---" << endl;
    ParkingRequest request1("REQ001", &car1, "Z1");
    ParkingRequest request2("REQ002", &car2, "Z2");
    ParkingRequest request3("REQ003", &car3, "Z1");
    ParkingRequest request4("REQ004", &car4, "Z3");
    
    request1.displayRequestInfo();
    request2.displayRequestInfo();
    
    cout << "\n--- Test 3: Allocating Slots (Same Zone) ---" << endl;
    ParkingSlot* slot1 = engine.allocateSlot(&request1);
    if (slot1) {
        cout << "Allocating slot for request1: ";
        bool success = request1.allocateSlot(slot1, false);
        if (success) {
            request1.displayRequestInfo();
        }
    }
    
    cout << "\n--- Test 4: Testing State Transitions ---" << endl;
    cout << "Before markAsOccupied: " << request1.stateToString() << endl;
    request1.markAsOccupied();
    cout << "After markAsOccupied: " << request1.stateToString() << endl;
    
    // Test invalid transition
    cout << "\nAttempting invalid transition (markAsOccupied again):" << endl;
    request1.markAsOccupied();
    
    cout << "\n--- Test 5: Complete Parking Cycle ---" << endl;
    cout << "Completing parking for request1..." << endl;
    request1.markAsReleased();
    request1.displayRequestInfo();
    
    cout << "\n--- Test 6: Multiple Requests ---" << endl;
    // Allocate for request2
    ParkingSlot* slot2 = engine.allocateSlot(&request2);
    if (slot2) {
        request2.allocateSlot(slot2, false);
        request2.markAsOccupied();
        request2.displayRequestInfo();
    }
    
    // Allocate for request3
    ParkingSlot* slot3 = engine.allocateSlot(&request3);
    if (slot3) {
        request3.allocateSlot(slot3, false);
        request3.markAsOccupied();
        request3.displayRequestInfo();
    }
    
    // Allocate for request4
    ParkingSlot* slot4 = engine.allocateSlot(&request4);
    if (slot4) {
        request4.allocateSlot(slot4, false);
        request4.markAsOccupied();
        request4.displayRequestInfo();
    }
    
    cout << "\n--- Test 7: Cross-Zone Allocation ---" << endl;
    // Create another request for zone B (which should be full now)
    Vehicle car5("CAR005", "Compact", "Z2");
    ParkingRequest request5("REQ005", &car5, "Z2");
    
    ParkingSlot* slot5 = engine.allocateSlot(&request5);
    if (slot5) {
        // This should be cross-zone since Z2 only has 1 slot (already occupied by CAR002)
        bool crossZone = (slot5->getZoneId() != "Z2");
        request5.allocateSlot(slot5, crossZone);
        request5.markAsOccupied();
        request5.displayRequestInfo();
    }
    
    cout << "\n--- Test 8: Cancellation Test ---" << endl;
    Vehicle car6("CAR006", "Luxury", "Z1");
    ParkingRequest request6("REQ006", &car6, "Z1");
    
    ParkingSlot* slot6 = engine.allocateSlot(&request6);
    if (slot6) {
        request6.allocateSlot(slot6, false);
        cout << "Before cancellation: " << request6.stateToString() << endl;
        
        // Cancel the request
        request6.cancelRequest();
        cout << "After cancellation: " << request6.stateToString() << endl;
        request6.displayRequestInfo();
        
        // Verify slot is available again
        cout << "\nVerifying slot availability after cancellation:" << endl;
        slot6->displaySlotInfo();
    }
    
    cout << "\n--- Test 9: Invalid Cancellation ---" << endl;
    cout << "Attempting to cancel already released request:" << endl;
    request1.cancelRequest();
    
    cout << "\n--- Test 10: System Status ---" << endl;
    engine.displayAllZones();
    cout << "Total available slots in system: " << engine.getTotalAvailableSlots() << endl;
    
    cout << "\n=== PHASE 2 TESTING COMPLETE ===" << endl;
}

void mainMenu() {
    int choice;
    
    do {
        clearScreen();
        cout << "=== SMART PARKING SYSTEM - MAIN MENU ===" << endl;
        cout << "1. Run Phase 1 Tests" << endl;
        cout << "2. Run Phase 2 Tests" << endl;
        cout << "3. Exit" << endl;
        
        choice = getChoice(1, 3);
        
        switch(choice) {
            case 1:
                testPhase1(); // Need to define this or keep from Phase 1
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            case 2:
                testPhase2();
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            case 3:
                cout << "Exiting program..." << endl;
                break;
        }
    } while(choice != 3);
}

int main() {
    mainMenu();
    return 0;
}