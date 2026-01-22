#include <iostream>
#include "Zone.h"
#include "Vehicle.h"
#include "ParkingRequest.h"
#include "AllocationEngine.h"
#include "RequestManager.h"
#include "RollbackManager.h"
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

// Global test variables (will be replaced by ParkingSystem in next phase)
Zone* testZoneA = nullptr;
Zone* testZoneB = nullptr;
Zone* testZoneC = nullptr;
AllocationEngine* testEngine = nullptr;
RequestManager* requestManager = nullptr;
RollbackManager* rollbackManager = nullptr;
int vehicleCounter = 1000;
int requestCounter = 1000;

void setupTestEnvironment() {
    clearScreen();
    cout << "=== SETTING UP TEST ENVIRONMENT ===" << endl;
    
    // Create zones
    testZoneA = new Zone("Z1", "Downtown", 3);
    testZoneB = new Zone("Z2", "Uptown", 2);
    testZoneC = new Zone("Z3", "Midtown", 2);
    
    // Add areas and slots
    testZoneA->addArea("A1", 3);
    testZoneA->addArea("A2", 2);
    testZoneB->addArea("B1", 2);
    testZoneC->addArea("C1", 3);
    
    // Add slots to areas
    ParkingArea* area;
    
    area = testZoneA->findArea("A1");
    if (area) {
        area->addSlot("Z1-A1-S1");
        area->addSlot("Z1-A1-S2");
        area->addSlot("Z1-A1-S3");
    }
    
    area = testZoneA->findArea("A2");
    if (area) {
        area->addSlot("Z1-A2-S1");
        area->addSlot("Z1-A2-S2");
    }
    
    area = testZoneB->findArea("B1");
    if (area) {
        area->addSlot("Z2-B1-S1");
        area->addSlot("Z2-B1-S2");
    }
    
    area = testZoneC->findArea("C1");
    if (area) {
        area->addSlot("Z3-C1-S1");
        area->addSlot("Z3-C1-S2");
        area->addSlot("Z3-C1-S3");
    }
    
    // Create allocation engine
    testEngine = new AllocationEngine(10);
    testEngine->addZone(testZoneA);
    testEngine->addZone(testZoneB);
    testEngine->addZone(testZoneC);
    
    // Create managers
    requestManager = new RequestManager();
    rollbackManager = new RollbackManager(5);
    
    cout << "Test environment setup complete!" << endl;
    cout << "Zones created: 3" << endl;
    cout << "Total slots: " << (testZoneA->getTotalSlots() + testZoneB->getTotalSlots() + testZoneC->getTotalSlots()) << endl;
    cout << "Available slots: " << testEngine->getTotalAvailableSlots() << endl;
}

void cleanupTestEnvironment() {
    delete testZoneA;
    delete testZoneB;
    delete testZoneC;
    delete testEngine;
    delete requestManager;
    delete rollbackManager;
    
    testZoneA = nullptr;
    testZoneB = nullptr;
    testZoneC = nullptr;
    testEngine = nullptr;
    requestManager = nullptr;
    rollbackManager = nullptr;
    
    vehicleCounter = 1000;
    requestCounter = 1000;
}

void testPhase3() {
    clearScreen();
    cout << "=== PHASE 3 TESTING: Interactive System Test ===" << endl;
    
    setupTestEnvironment();
    
    cout << "\nPress Enter to continue to interactive testing...";
    cin.get();
}

void interactiveTesting() {
    if (testEngine == nullptr || requestManager == nullptr || rollbackManager == nullptr) {
        cout << "Error: Test environment not initialized. Run Phase 3 tests first." << endl;
        return;
    }
    
    int choice;
    
    do {
        clearScreen();
        cout << "=== INTERACTIVE SYSTEM TESTING ===" << endl;
        cout << "1. Display System Status" << endl;
        cout << "2. Create New Vehicle" << endl;
        cout << "3. Create Parking Request" << endl;
        cout << "4. Allocate Slot to Request" << endl;
        cout << "5. Mark as Occupied" << endl;
        cout << "6. Mark as Released" << endl;
        cout << "7. Cancel Request" << endl;
        cout << "8. View All Requests" << endl;
        cout << "9. View Active Requests" << endl;
        cout << "10. Rollback Last Operation" << endl;
        cout << "11. View Rollback Stack" << endl;
        cout << "12. Run Analytics" << endl;
        cout << "13. Auto Test Scenario" << endl;
        cout << "14. Return to Main Menu" << endl;
        
        choice = getChoice(1, 14);
        
        switch(choice) {
            case 1: {
                clearScreen();
                cout << "=== SYSTEM STATUS ===" << endl;
                testEngine->displayAllZones();
                cout << "\nTotal Available Slots: " << testEngine->getTotalAvailableSlots() << endl;
                cout << "Total Requests: " << requestManager->getRequestCount() << endl;
                cout << "Available Rollbacks: " << rollbackManager->getAvailableRollbacks() << endl;
                break;
            }
            
            case 2: {
                clearScreen();
                cout << "=== CREATE NEW VEHICLE ===" << endl;
                vehicleCounter++;
                string vehicleId = "V" + to_string(vehicleCounter);
                
                cout << "Vehicle Types: 1. Sedan, 2. SUV, 3. Hatchback, 4. Truck" << endl;
                int typeChoice = getChoice(1, 4);
                
                string vehicleType;
                switch(typeChoice) {
                    case 1: vehicleType = "Sedan"; break;
                    case 2: vehicleType = "SUV"; break;
                    case 3: vehicleType = "Hatchback"; break;
                    case 4: vehicleType = "Truck"; break;
                }
                
                cout << "Preferred Zone (Z1, Z2, Z3): ";
                string zoneId;
                getline(cin, zoneId);
                
                Vehicle* newVehicle = new Vehicle(vehicleId, vehicleType, zoneId);
                cout << "\nCreated Vehicle:" << endl;
                newVehicle->displayVehicleInfo();
                
                // Note: Vehicle management will be added in ParkingSystem
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            
            case 3: {
                clearScreen();
                cout << "=== CREATE PARKING REQUEST ===" << endl;
                requestCounter++;
                string requestId = "R" + to_string(requestCounter);
                
                cout << "Enter Vehicle ID (e.g., V1001): ";
                string vehicleId;
                getline(cin, vehicleId);
                
                cout << "Enter Preferred Zone (Z1, Z2, Z3): ";
                string zoneId;
                getline(cin, zoneId);
                
                // Create a temporary vehicle for demo
                Vehicle* tempVehicle = new Vehicle(vehicleId, "Sedan", zoneId);
                ParkingRequest* newRequest = new ParkingRequest(requestId, tempVehicle, zoneId);
                requestManager->addRequest(newRequest);
                
                cout << "\nCreated Parking Request:" << endl;
                newRequest->displayRequestInfo();
                
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            
            case 4: {
                clearScreen();
                cout << "=== ALLOCATE SLOT TO REQUEST ===" << endl;
                
                cout << "Enter Request ID to allocate: ";
                string requestId;
                getline(cin, requestId);
                
                ParkingRequest* request = requestManager->findRequest(requestId);
                if (request == nullptr) {
                    cout << "Error: Request not found." << endl;
                } else {
                    ParkingSlot* slot = testEngine->allocateSlot(request);
                    if (slot != nullptr) {
                        bool crossZone = (slot->getZoneId() != request->getRequestedZoneId());
                        bool success = request->allocateSlot(slot, crossZone);
                        
                        if (success) {
                            // Record for rollback
                            rollbackManager->recordAllocation(requestId, slot->getSlotId(), slot->getZoneId());
                            cout << "Slot allocated successfully!" << endl;
                        }
                    } else {
                        cout << "Error: No available slots." << endl;
                    }
                }
                
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            
            case 5: {
                clearScreen();
                cout << "=== MARK REQUEST AS OCCUPIED ===" << endl;
                
                cout << "Enter Request ID: ";
                string requestId;
                getline(cin, requestId);
                
                ParkingRequest* request = requestManager->findRequest(requestId);
                if (request == nullptr) {
                    cout << "Error: Request not found." << endl;
                } else {
                    RequestState previousState = request->getCurrentState();
                    bool success = request->markAsOccupied();
                    if (success) {
                        rollbackManager->recordStateChange(requestId, previousState);
                        cout << "Request marked as OCCUPIED." << endl;
                    }
                }
                
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            
            case 6: {
                clearScreen();
                cout << "=== MARK REQUEST AS RELEASED ===" << endl;
                
                cout << "Enter Request ID: ";
                string requestId;
                getline(cin, requestId);
                
                ParkingRequest* request = requestManager->findRequest(requestId);
                if (request == nullptr) {
                    cout << "Error: Request not found." << endl;
                } else {
                    RequestState previousState = request->getCurrentState();
                    bool success = request->markAsReleased();
                    if (success) {
                        rollbackManager->recordStateChange(requestId, previousState);
                        cout << "Request marked as RELEASED." << endl;
                        cout << "Duration: " << request->calculateDuration() << " minutes" << endl;
                    }
                }
                
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            
            case 7: {
                clearScreen();
                cout << "=== CANCEL REQUEST ===" << endl;
                
                cout << "Enter Request ID: ";
                string requestId;
                getline(cin, requestId);
                
                ParkingRequest* request = requestManager->findRequest(requestId);
                if (request == nullptr) {
                    cout << "Error: Request not found." << endl;
                } else {
                    RequestState previousState = request->getCurrentState();
                    bool success = request->cancelRequest();
                    if (success) {
                        rollbackManager->recordCancellation(requestId);
                        cout << "Request cancelled successfully." << endl;
                    }
                }
                
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            
            case 8: {
                clearScreen();
                requestManager->displayAllRequests();
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            
            case 9: {
                clearScreen();
                requestManager->displayActiveRequests();
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            
            case 10: {
                clearScreen();
                cout << "=== ROLLBACK LAST OPERATION ===" << endl;
                bool success = rollbackManager->rollbackLastOperation(requestManager, testEngine);
                if (success) {
                    cout << "Rollback successful!" << endl;
                }
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            
            case 11: {
                clearScreen();
                rollbackManager->displayRollbackStack();
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            
            case 12: {
                clearScreen();
                cout << "=== SYSTEM ANALYTICS ===" << endl;
                cout << "Total Requests: " << requestManager->getRequestCount() << endl;
                cout << "Completed Requests: " << requestManager->countByState(RequestState::RELEASED) << endl;
                cout << "Cancelled Requests: " << requestManager->countByState(RequestState::CANCELLED) << endl;
                cout << "Active Requests: " << requestManager->countByState(RequestState::ALLOCATED) + 
                        requestManager->countByState(RequestState::OCCUPIED) << endl;
                cout << "Average Parking Duration: " << requestManager->getAverageDuration() << " minutes" << endl;
                cout << "\nZone Utilization:" << endl;
                cout << "Zone Z1: " << testZoneA->getTotalSlots() - testZoneA->getAvailableSlots() 
                     << "/" << testZoneA->getTotalSlots() << " slots occupied" << endl;
                cout << "Zone Z2: " << testZoneB->getTotalSlots() - testZoneB->getAvailableSlots() 
                     << "/" << testZoneB->getTotalSlots() << " slots occupied" << endl;
                cout << "Zone Z3: " << testZoneC->getTotalSlots() - testZoneC->getAvailableSlots() 
                     << "/" << testZoneC->getTotalSlots() << " slots occupied" << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            
            case 13: {
                clearScreen();
                cout << "=== AUTO TEST SCENARIO ===" << endl;
                cout << "Running automated test scenario..." << endl;
                
                // Create vehicles
                Vehicle v1("V-AUTO1", "Sedan", "Z1");
                Vehicle v2("V-AUTO2", "SUV", "Z2");
                Vehicle v3("V-AUTO3", "Truck", "Z1");
                
                // Create requests
                ParkingRequest* r1 = new ParkingRequest("R-AUTO1", &v1, "Z1");
                ParkingRequest* r2 = new ParkingRequest("R-AUTO2", &v2, "Z2");
                ParkingRequest* r3 = new ParkingRequest("R-AUTO3", &v3, "Z1");
                
                requestManager->addRequest(r1);
                requestManager->addRequest(r2);
                requestManager->addRequest(r3);
                
                // Allocate slots
                ParkingSlot* s1 = testEngine->allocateSlot(r1);
                if (s1) r1->allocateSlot(s1, false);
                
                ParkingSlot* s2 = testEngine->allocateSlot(r2);
                if (s2) r2->allocateSlot(s2, false);
                
                ParkingSlot* s3 = testEngine->allocateSlot(r3);
                if (s3) r3->allocateSlot(s3, s3->getZoneId() != "Z1");
                
                // Mark as occupied
                r1->markAsOccupied();
                r2->markAsOccupied();
                
                // Release one
                r1->markAsReleased();
                
                // Cancel one
                r3->cancelRequest();
                
                cout << "Auto test completed!" << endl;
                cout << "\nCurrent Status:" << endl;
                requestManager->displayRequestHistory();
                
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            }
            
            case 14:
                cout << "Returning to main menu..." << endl;
                break;
        }
        
    } while(choice != 14);
}

void mainMenu() {
    int choice;
    
    do {
        clearScreen();
        cout << "=== SMART PARKING SYSTEM - MAIN MENU ===" << endl;
        cout << "1. Run Phase 1 Tests (Basic Structures)" << endl;
        cout << "2. Run Phase 2 Tests (Requests & Allocation)" << endl;
        cout << "3. Run Phase 3 Tests (Interactive System)" << endl;
        cout << "4. Enter Interactive Testing Mode" << endl;
        cout << "5. Exit" << endl;
        
        choice = getChoice(1, 5);
        
        switch(choice) {
            case 1:
                // testPhase1(); // From Phase 1
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            case 2:
                // testPhase2(); // From Phase 2
                cout << "\nPress Enter to continue...";
                cin.get();
                break;
            case 3:
                testPhase3();
                break;
            case 4:
                interactiveTesting();
                break;
            case 5:
                cleanupTestEnvironment();
                cout << "Exiting program..." << endl;
                break;
        }
    } while(choice != 5);
}

int main() {
    mainMenu();
    return 0;
}