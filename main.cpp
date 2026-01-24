#include <iostream>
#include "ParkingSystem.h"
#include "TestSuite.h"
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
    cout << "4. Process Next Request (Queue)" << endl;
    cout << "5. View Pending Requests" << endl;
    cout << "6. Allocate Slot to Request" << endl;
    cout << "7. Mark Request as Occupied" << endl;
    cout << "8. Mark Request as Released" << endl;
    cout << "9. Cancel Parking Request" << endl;
    cout << "10. View All Vehicles (BST)" << endl;
    cout << "11. Rollback Last Operation" << endl;
    cout << "12. Rollback Last K Operations" << endl;
    cout << "13. System Analytics" << endl;
    cout << "14. Run Test Suite (10 Tests)" << endl;
    cout << "15. Run Auto Demo Scenario" << endl;
    cout << "16. Exit" << endl;
    cout << "=======================================" << endl;
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

void runRollbackMenu(ParkingSystem& system) {
    clearScreen();
    cout << "=== ROLLBACK OPERATIONS ===" << endl;
    cout << "Enter number of operations to rollback: ";
    
    int k;
    cin >> k;
    cin.ignore();
    
    if (k <= 0) {
        cout << "Invalid number." << endl;
    } else {
        system.rollbackLastKOperations(k);
    }
}

int main() {
    ParkingSystem system;
    TestSuite testSuite;
    int choice;
    
    cout << "Initializing Smart Parking System..." << endl;
    cout << "System ready!" << endl;
    cout << "Press Enter to continue...";
    cin.get();
    
    do {
        displayMainMenu();
        choice = getChoice(1, 16);
        
        switch(choice) {
            case 1:
                system.displaySystemStatus();
                break;
                
            case 2: {
                clearScreen();
                cout << "=== REGISTER NEW VEHICLE ===" << endl;
                cout << "Vehicle Type: ";
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
                cout << "Vehicle ID: ";
                string vehicleId;
                getline(cin, vehicleId);
                cout << "Requested Zone (Z1/Z2/Z3): ";
                string zone;
                getline(cin, zone);
                string reqId = system.createParkingRequest(vehicleId, zone);
                if (!reqId.empty()) {
                    cout << "Request created with ID: " << reqId << endl;
                }
                break;
            }
                
            case 4:
                system.processNextRequest();
                break;
                
            case 5:
                system.displayPendingRequests();
                break;
                
            case 6: {
                clearScreen();
                cout << "=== ALLOCATE SLOT TO REQUEST ===" << endl;
                cout << "Request ID: ";
                string reqId;
                getline(cin, reqId);
                system.allocateSlotToRequest(reqId);
                break;
            }
                
            case 7: {
                clearScreen();
                cout << "=== MARK AS OCCUPIED ===" << endl;
                cout << "Request ID: ";
                string reqId;
                getline(cin, reqId);
                system.markAsOccupied(reqId);
                break;
            }
                
            case 8: {
                clearScreen();
                cout << "=== MARK AS RELEASED ===" << endl;
                cout << "Request ID: ";
                string reqId;
                getline(cin, reqId);
                system.markAsReleased(reqId);
                break;
            }
                
            case 9: {
                clearScreen();
                cout << "=== CANCEL REQUEST ===" << endl;
                cout << "Request ID: ";
                string reqId;
                getline(cin, reqId);
                system.cancelRequest(reqId);
                break;
            }
                
            case 10:
                system.displayAllVehicles();
                break;
                
            case 11:
                system.rollbackLastOperation();
                break;
                
            case 12:
                runRollbackMenu(system);
                break;
                
            case 13:
                systemAnalyticsMenu(system);
                break;
                
            case 14:
                testSuite.runAllTests();
                break;
                
            case 15: {
                clearScreen();
                cout << "=== AUTO DEMO SCENARIO ===" << endl;
                cout << "This will demonstrate all system features:" << endl;
                cout << "1. Vehicle Registration" << endl;
                cout << "2. Request Creation & Queue" << endl;
                cout << "3. Request Processing" << endl;
                cout << "4. State Transitions" << endl;
                cout << "5. Cross-Zone Allocation" << endl;
                cout << "6. Cancellation & Rollback" << endl;
                cout << "7. Analytics Display" << endl;
                cout << "\nPress Enter to start demo...";
                cin.get();
                
                // Step 1: Display initial state
                clearScreen();
                cout << "=== STEP 1: INITIAL SYSTEM STATE ===" << endl;
                system.displaySystemStatus();
                cout << "\nPress Enter to continue...";
                cin.get();
                
                // Step 2: Register vehicles
                clearScreen();
                cout << "=== STEP 2: VEHICLE REGISTRATION ===" << endl;
                system.addVehicle("Sedan", "Z1");
                system.addVehicle("SUV", "Z2");
                system.addVehicle("Truck", "Z1");
                system.addVehicle("Compact", "Z3");
                system.addVehicle("Luxury", "Z2");
                system.addVehicle("Motorcycle", "Z1");
                cout << "\n6 vehicles registered successfully!" << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                
                // Step 3: Create parking requests
                clearScreen();
                cout << "=== STEP 3: CREATING PARKING REQUESTS ===" << endl;
                cout << "Creating 5 parking requests..." << endl;
                
                string req1 = system.createParkingRequest("V1000", "Z1");
                string req2 = system.createParkingRequest("V1001", "Z2");
                string req3 = system.createParkingRequest("V1002", "Z1");
                string req4 = system.createParkingRequest("V1003", "Z3");
                string req5 = system.createParkingRequest("V1004", "Z2");
                
                cout << "\nRequests created and added to queue:" << endl;
                cout << "1. " << req1 << " - V1000 -> Z1" << endl;
                cout << "2. " << req2 << " - V1001 -> Z2" << endl;
                cout << "3. " << req3 << " - V1002 -> Z1" << endl;
                cout << "4. " << req4 << " - V1003 -> Z3" << endl;
                cout << "5. " << req5 << " - V1004 -> Z2" << endl;
                
                cout << "\nPress Enter to view queue...";
                cin.get();
                
                clearScreen();
                system.displayPendingRequests();
                cout << "\nPress Enter to process requests...";
                cin.get();
                
                // Step 4: Process requests from queue
                clearScreen();
                cout << "=== STEP 4: PROCESSING REQUESTS FROM QUEUE ===" << endl;
                
                cout << "Processing request 1..." << endl;
                system.processNextRequest();
                cout << "\nPress Enter to continue...";
                cin.get();
                
                cout << "\nProcessing request 2..." << endl;
                system.processNextRequest();
                cout << "\nPress Enter to continue...";
                cin.get();
                
                cout << "\nProcessing request 3..." << endl;
                system.processNextRequest();
                cout << "\nPress Enter to continue...";
                cin.get();
                
                cout << "\nProcessing request 4..." << endl;
                system.processNextRequest();
                cout << "\nPress Enter to continue...";
                cin.get();
                
                cout << "\nProcessing request 5..." << endl;
                system.processNextRequest();
                cout << "\nPress Enter to continue...";
                cin.get();
                
                // Step 5: Complete lifecycle for some requests
                clearScreen();
                cout << "=== STEP 5: COMPLETE PARKING LIFECYCLE ===" << endl;
                
                cout << "Completing lifecycle for request " << req1 << "..." << endl;
                system.markAsOccupied(req1);
                cout << "\nPress Enter to continue...";
                cin.get();
                
                cout << "\nVehicle parked for 2 minutes (simulated)..." << endl;
                cout << "Releasing request " << req1 << "..." << endl;
                system.markAsReleased(req1);
                cout << "\nPress Enter to continue...";
                cin.get();
                
                // Step 6: Demonstrate cross-zone allocation
                clearScreen();
                cout << "=== STEP 6: CROSS-ZONE ALLOCATION DEMO ===" << endl;
                
                cout << "Creating additional requests for Z2 (which has 2 slots)..." << endl;
                system.addVehicle("Van", "Z2");
                string req6 = system.createParkingRequest("V1005", "Z2");
                system.processNextRequest();
                
                cout << "\nZ2 now has 2/2 slots occupied." << endl;
                cout << "Creating one more request for Z2..." << endl;
                
                system.addVehicle("Coupe", "Z2");
                string req7 = system.createParkingRequest("V1006", "Z2");
                system.processNextRequest();
                
                cout << "\nThis should trigger CROSS-ZONE allocation!" << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                
                // Step 7: Demonstrate cancellation
                clearScreen();
                cout << "=== STEP 7: CANCELLATION DEMO ===" << endl;
                
                cout << "Cancelling request " << req3 << "..." << endl;
                system.cancelRequest(req3);
                
                cout << "\nSlot should be freed and available for new requests." << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                
                // Step 8: Demonstrate rollback
                clearScreen();
                cout << "=== STEP 8: ROLLBACK DEMO ===" << endl;
                
                cout << "Current available slots: " << system.getAvailableSlots() << endl;
                cout << "Performing rollback of last operation (cancellation)..." << endl;
                
                system.rollbackLastOperation();
                
                cout << "\nAvailable slots after rollback: " << system.getAvailableSlots() << endl;
                cout << "Cancellation has been undone!" << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                
                // Step 9: Show analytics
                clearScreen();
                cout << "=== STEP 9: ANALYTICS DEMO ===" << endl;
                
                cout << "Displaying system analytics..." << endl;
                cout << "\nPress Enter for system status...";
                cin.get();
                
                system.displaySystemStatus();
                cout << "\nPress Enter for zone analytics...";
                cin.get();
                
                system.displayZoneAnalytics();
                cout << "\nPress Enter for request analytics...";
                cin.get();
                
                system.displayRequestAnalytics();
                cout << "\nPress Enter for peak usage analysis...";
                cin.get();
                
                system.displayPeakUsage();
                
                // Step 10: Final summary
                clearScreen();
                cout << "=======================================" << endl;
                cout << "       DEMO SCENARIO COMPLETE!" << endl;
                cout << "=======================================" << endl;
                cout << "\nSummary of operations performed:" << endl;
                cout << "✓ 7 vehicles registered" << endl;
                cout << "✓ 7 parking requests created" << endl;
                cout << "✓ 7 requests processed from queue" << endl;
                cout << "✓ Complete lifecycle demonstrated" << endl;
                cout << "✓ Cross-zone allocation triggered" << endl;
                cout << "✓ Cancellation with slot restoration" << endl;
                cout << "✓ Rollback operation demonstrated" << endl;
                cout << "✓ All analytics functions tested" << endl;
                cout << "\nFinal System State:" << endl;
                cout << "Total Slots: " << system.getTotalSlots() << endl;
                cout << "Available Slots: " << system.getAvailableSlots() << endl;
                cout << "Total Requests: " << system.getTotalRequests() << endl;
                cout << "Active Requests: " << system.getActiveRequests() << endl;
                cout << "\n=======================================" << endl;
                break;
            }
                
            case 16:
                cout << "Exiting Smart Parking System. Goodbye!" << endl;
                break;
        }
        
        if (choice != 16 && choice != 13) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        
    } while(choice != 16);
    
    return 0;
}