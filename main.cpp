#include <iostream>
#include "ParkingSystem.h"
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

void displayMainMenu() {
    clearScreen();
    cout << "=======================================" << endl;
    cout << "    SMART PARKING ALLOCATION SYSTEM    " << endl;
    cout << "=======================================" << endl;
    cout << "1. Display System Status" << endl;
    cout << "2. Register New Vehicle" << endl;
    cout << "3. Create Parking Request" << endl;
    cout << "4. Allocate Slot to Request" << endl;
    cout << "5. Mark Request as Occupied" << endl;
    cout << "6. Mark Request as Released" << endl;
    cout << "7. Cancel Parking Request" << endl;
    cout << "8. View All Vehicles" << endl;
    cout << "9. View All Requests" << endl;
    cout << "10. View Active Requests" << endl;
    cout << "11. Rollback Last Operation" << endl;
    cout << "12. View Rollback Stack" << endl;
    cout << "13. System Analytics" << endl;
    cout << "14. Run Auto Test Scenario" << endl;
    cout << "15. Exit" << endl;
    cout << "=======================================" << endl;
}

void runAutoTestScenario(ParkingSystem& system) {
    clearScreen();
    cout << "=== AUTO TEST SCENARIO ===" << endl;
    
    // Register test vehicles
    cout << "\n1. Registering test vehicles..." << endl;
    system.addVehicle("Sedan", "Z1");
    system.addVehicle("SUV", "Z2");
    system.addVehicle("Truck", "Z1");
    system.addVehicle("Compact", "Z3");
    
    // Create parking requests
    cout << "\n2. Creating parking requests..." << endl;
    string req1 = system.createParkingRequest("V1000", "Z1");
    string req2 = system.createParkingRequest("V1001", "Z2");
    string req3 = system.createParkingRequest("V1002", "Z1");
    string req4 = system.createParkingRequest("V1003", "Z3");
    
    // Allocate slots
    cout << "\n3. Allocating slots..." << endl;
    system.allocateSlotToRequest(req1);
    system.allocateSlotToRequest(req2);
    system.allocateSlotToRequest(req3);
    system.allocateSlotToRequest(req4);
    
    // Mark as occupied
    cout << "\n4. Marking as occupied..." << endl;
    system.markAsOccupied(req1);
    system.markAsOccupied(req2);
    system.markAsOccupied(req3);
    
    // Release one
    cout << "\n5. Releasing one request..." << endl;
    system.markAsReleased(req1);
    
    // Cancel one
    cout << "\n6. Cancelling one request..." << endl;
    system.cancelRequest(req4);
    
    // Create cross-zone scenario
    cout << "\n7. Testing cross-zone allocation..." << endl;
    system.addVehicle("Luxury", "Z2");
    string req5 = system.createParkingRequest("V1004", "Z2");
    system.allocateSlotToRequest(req5); // Should be cross-zone since Z2 has only 2 slots
    
    // Test rollback
    cout << "\n8. Testing rollback..." << endl;
    system.rollbackLastOperation();
    
    cout << "\n=== AUTO TEST COMPLETED ===" << endl;
    cout << "\nFinal System Status:" << endl;
    system.displaySystemStatus();
}

void systemAnalyticsMenu(ParkingSystem& system) {
    int choice;
    
    do {
        clearScreen();
        cout << "=== SYSTEM ANALYTICS ===" << endl;
        cout << "1. Zone Analytics" << endl;
        cout << "2. Request Analytics" << endl;
        cout << "3. Peak Usage Analysis" << endl;
        cout << "4. Full System Report" << endl;
        cout << "5. Back to Main Menu" << endl;
        
        choice = getChoice(1, 5);
        
        switch(choice) {
            case 1:
                system.displayZoneAnalytics();
                break;
            case 2:
                system.displayRequestAnalytics();
                break;
            case 3:
                system.displayPeakUsage();
                break;
            case 4:
                system.displaySystemStatus();
                system.displayZoneAnalytics();
                system.displayRequestAnalytics();
                system.displayPeakUsage();
                break;
            case 5:
                return;
        }
        
        if (choice != 5) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    } while(choice != 5);
}

int main() {
    ParkingSystem system;
    int choice;
    
    cout << "Initializing Smart Parking System..." << endl;
    cout << "System ready!" << endl;
    cout << "Press Enter to continue...";
    cin.get();
    
    do {
        displayMainMenu();
        choice = getChoice(1, 15);
        
        switch(choice) {
            case 1: {
                system.displaySystemStatus();
                break;
            }
            
            case 2: {
                clearScreen();
                cout << "=== REGISTER NEW VEHICLE ===" << endl;
                
                cout << "Vehicle Type (Sedan/SUV/Truck/Compact): ";
                string type;
                getline(cin, type);
                
                cout << "Preferred Zone (Z1/Z2/Z3): ";
                string zone;
                getline(cin, zone);
                
                system.addVehicle(type, zone);
                break;
            }
            
            case 3: {
                clearScreen();
                cout << "=== CREATE PARKING REQUEST ===" << endl;
                
                cout << "Vehicle ID (or leave empty for new vehicle): ";
                string vehicleId;
                getline(cin, vehicleId);
                
                if (vehicleId.empty()) {
                    cout << "Creating new vehicle..." << endl;
                    cout << "Vehicle Type: ";
                    string type;
                    getline(cin, type);
                    
                    cout << "Preferred Zone: ";
                    string zone;
                    getline(cin, zone);
                    
                    system.addVehicle(type, zone);
                    vehicleId = "V" + to_string(1000 + (system.getTotalRequests() % 1000));
                }
                
                cout << "Requested Zone (Z1/Z2/Z3): ";
                string requestedZone;
                getline(cin, requestedZone);
                
                string requestId = system.createParkingRequest(vehicleId, requestedZone);
                if (!requestId.empty()) {
                    cout << "Request created with ID: " << requestId << endl;
                }
                break;
            }
            
            case 4: {
                clearScreen();
                cout << "=== ALLOCATE SLOT TO REQUEST ===" << endl;
                
                cout << "Enter Request ID: ";
                string requestId;
                getline(cin, requestId);
                
                system.allocateSlotToRequest(requestId);
                break;
            }
            
            case 5: {
                clearScreen();
                cout << "=== MARK REQUEST AS OCCUPIED ===" << endl;
                
                cout << "Enter Request ID: ";
                string requestId;
                getline(cin, requestId);
                
                system.markAsOccupied(requestId);
                break;
            }
            
            case 6: {
                clearScreen();
                cout << "=== MARK REQUEST AS RELEASED ===" << endl;
                
                cout << "Enter Request ID: ";
                string requestId;
                getline(cin, requestId);
                
                system.markAsReleased(requestId);
                break;
            }
            
            case 7: {
                clearScreen();
                cout << "=== CANCEL PARKING REQUEST ===" << endl;
                
                cout << "Enter Request ID: ";
                string requestId;
                getline(cin, requestId);
                
                system.cancelRequest(requestId);
                break;
            }
            
            case 8: {
                clearScreen();
                system.displayAllVehicles();
                break;
            }
            
            case 9: {
                clearScreen();
                cout << "=== ALL PARKING REQUESTS ===" << endl;
                // We need to add a method to RequestManager to get all requests
                // For now, display system status
                system.displaySystemStatus();
                break;
            }
            
            case 10: {
                clearScreen();
                cout << "=== ACTIVE REQUESTS ===" << endl;
                cout << "Active Requests: " << system.getActiveRequests() << endl;
                break;
            }
            
            case 11: {
                clearScreen();
                cout << "=== ROLLBACK LAST OPERATION ===" << endl;
                bool success = system.rollbackLastOperation();
                if (success) {
                    cout << "Rollback successful!" << endl;
                }
                break;
            }
            
            case 12: {
                clearScreen();
                // Note: We need to add this method to ParkingSystem
                cout << "Rollback feature is active." << endl;
                cout << "Last operation can be undone using option 11." << endl;
                break;
            }
            
            case 13: {
                systemAnalyticsMenu(system);
                break;
            }
            
            case 14: {
                runAutoTestScenario(system);
                break;
            }
            
            case 15: {
                cout << "Exiting Smart Parking System. Goodbye!" << endl;
                break;
            }
        }
        
        if (choice != 15 && choice != 13) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        
    } while(choice != 15);
    
    return 0;
}