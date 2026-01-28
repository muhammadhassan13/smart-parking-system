import os
import sys
from parking_system import ParkingSystem
from test_suite import TestSuite

def clear_screen():
    """Clear the console screen."""
    os.system('cls' if os.name == 'nt' else 'clear')

def get_choice(min_val, max_val):
    """Get and validate user choice."""
    while True:
        try:
            choice = int(input(f"\nEnter choice ({min_val}-{max_val}): "))
            if min_val <= choice <= max_val:
                return choice
            else:
                print(f"Invalid Input. Please enter a number between {min_val} and {max_val}")
        except ValueError:
            print("Invalid Input. Please enter a valid number.")

def display_main_menu():
    """Display the main menu."""
    clear_screen()
    print("=" * 40)
    print("    SMART PARKING ALLOCATION SYSTEM    ")
    print("=" * 40)
    print("1. Display System Status")
    print("2. Register New Vehicle")
    print("3. Create Parking Request")
    print("4. Process Next Request (Queue)")
    print("5. View Pending Requests")
    print("6. Allocate Slot to Request")
    print("7. Mark Request as Occupied")
    print("8. Mark Request as Released")
    print("9. Cancel Parking Request")
    print("10. View All Vehicles (BST)")
    print("11. Rollback Last Operation")
    print("12. Rollback Last K Operations")
    print("13. System Analytics")
    print("14. Run Test Suite (10 Tests)")
    print("15. Run Auto Demo Scenario")
    print("16. Exit")
    print("=" * 40)

def system_analytics_menu(system):
    """Display system analytics menu."""
    while True:
        clear_screen()
        print("=== SYSTEM ANALYTICS ===")
        print("1. Zone Analytics")
        print("2. Request Analytics")
        print("3. Peak Usage Analysis")
        print("4. Full System Report")
        print("5. Back to Main Menu")
        
        choice = get_choice(1, 5)
        
        if choice == 1:
            system.display_zone_analytics()
        elif choice == 2:
            system.display_request_analytics()
        elif choice == 3:
            system.display_peak_usage()
        elif choice == 4:
            system.display_system_status()
            system.display_zone_analytics()
            system.display_request_analytics()
            system.display_peak_usage()
        elif choice == 5:
            return
        
        if choice != 5:
            input("\nPress Enter to continue...")

def run_rollback_menu(system):
    """Run rollback menu."""
    clear_screen()
    print("=== ROLLBACK OPERATIONS ===")
    try:
        k = int(input("Enter number of operations to rollback: "))
        if k <= 0:
            print("Invalid number.")
        else:
            system.rollback_last_k_operations(k)
    except ValueError:
        print("Invalid input. Please enter a number.")

def run_auto_demo(system):
    """Run auto demo scenario."""
    clear_screen()
    print("=== AUTO DEMO SCENARIO ===")
    print("This will demonstrate all system features:")
    print("1. Vehicle Registration")
    print("2. Request Creation & Queue")
    print("3. Request Processing")
    print("4. State Transitions")
    print("5. Cross-Zone Allocation")
    print("6. Cancellation & Rollback")
    print("7. Analytics Display")
    print("\nPress Enter to start demo...")
    input()
    
    # Step 1: Display initial state
    clear_screen()
    print("=== STEP 1: INITIAL SYSTEM STATE ===")
    system.display_system_status()
    print("\nPress Enter to continue...")
    input()
    
    # Step 2: Register vehicles
    clear_screen()
    print("=== STEP 2: VEHICLE REGISTRATION ===")
    system.add_vehicle("Sedan", "Z1")
    system.add_vehicle("SUV", "Z2")
    system.add_vehicle("Truck", "Z1")
    system.add_vehicle("Compact", "Z3")
    system.add_vehicle("Luxury", "Z2")
    system.add_vehicle("Motorcycle", "Z1")
    print("\n6 vehicles registered successfully!")
    print("\nPress Enter to continue...")
    input()
    
    # Step 3: Create parking requests
    clear_screen()
    print("=== STEP 3: CREATING PARKING REQUESTS ===")
    print("Creating 5 parking requests...")
    
    req1 = system.create_parking_request("V1000", "Z1")
    req2 = system.create_parking_request("V1001", "Z2")
    req3 = system.create_parking_request("V1002", "Z1")
    req4 = system.create_parking_request("V1003", "Z3")
    req5 = system.create_parking_request("V1004", "Z2")
    
    print(f"\nRequests created and added to queue:")
    print(f"1. {req1} - V1000 -> Z1")
    print(f"2. {req2} - V1001 -> Z2")
    print(f"3. {req3} - V1002 -> Z1")
    print(f"4. {req4} - V1003 -> Z3")
    print(f"5. {req5} - V1004 -> Z2")
    
    print("\nPress Enter to view queue...")
    input()
    
    clear_screen()
    system.display_pending_requests()
    print("\nPress Enter to process requests...")
    input()
    
    # Step 4: Process requests from queue
    clear_screen()
    print("=== STEP 4: PROCESSING REQUESTS FROM QUEUE ===")
    
    print("Processing request 1...")
    system.process_next_request()
    print("\nPress Enter to continue...")
    input()
    
    print("\nProcessing request 2...")
    system.process_next_request()
    print("\nPress Enter to continue...")
    input()
    
    print("\nProcessing request 3...")
    system.process_next_request()
    print("\nPress Enter to continue...")
    input()
    
    print("\nProcessing request 4...")
    system.process_next_request()
    print("\nPress Enter to continue...")
    input()
    
    print("\nProcessing request 5...")
    system.process_next_request()
    print("\nPress Enter to continue...")
    input()
    
    # Step 5: Complete lifecycle for some requests
    clear_screen()
    print("=== STEP 5: COMPLETE PARKING LIFECYCLE ===")
    
    print(f"Completing lifecycle for request {req1}...")
    system.mark_as_occupied(req1)
    print("\nPress Enter to continue...")
    input()
    
    print("\nVehicle parked for 2 minutes (simulated)...")
    print(f"Releasing request {req1}...")
    system.mark_as_released(req1)
    print("\nPress Enter to continue...")
    input()
    
    # Step 6: Demonstrate cross-zone allocation
    clear_screen()
    print("=== STEP 6: CROSS-ZONE ALLOCATION DEMO ===")
    
    print("Creating additional requests for Z2 (which has 2 slots)...")
    system.add_vehicle("Van", "Z2")
    req6 = system.create_parking_request("V1005", "Z2")
    system.process_next_request()
    
    print("\nZ2 now has 2/2 slots occupied.")
    print("Creating one more request for Z2...")
    
    system.add_vehicle("Coupe", "Z2")
    req7 = system.create_parking_request("V1006", "Z2")
    system.process_next_request()
    
    print("\nThis should trigger CROSS-ZONE allocation!")
    print("\nPress Enter to continue...")
    input()
    
    # Step 7: Demonstrate cancellation
    clear_screen()
    print("=== STEP 7: CANCELLATION DEMO ===")
    
    print(f"Cancelling request {req3}...")
    system.cancel_request(req3)
    
    print("\nSlot should be freed and available for new requests.")
    print("\nPress Enter to continue...")
    input()
    
    # Step 8: Demonstrate rollback
    clear_screen()
    print("=== STEP 8: ROLLBACK DEMO ===")
    
    print(f"Current available slots: {system.get_available_slots()}")
    print("Performing rollback of last operation (cancellation)...")
    
    system.rollback_last_operation()
    
    print(f"\nAvailable slots after rollback: {system.get_available_slots()}")
    print("Cancellation has been undone!")
    print("\nPress Enter to continue...")
    input()
    
    # Step 9: Show analytics
    clear_screen()
    print("=== STEP 9: ANALYTICS DEMO ===")
    
    print("Displaying system analytics...")
    print("\nPress Enter for system status...")
    input()
    
    system.display_system_status()
    print("\nPress Enter for zone analytics...")
    input()
    
    system.display_zone_analytics()
    print("\nPress Enter for request analytics...")
    input()
    
    system.display_request_analytics()
    print("\nPress Enter for peak usage analysis...")
    input()
    
    system.display_peak_usage()
    
    # Step 10: Final summary
    clear_screen()
    print("=" * 40)
    print("       DEMO SCENARIO COMPLETE!")
    print("=" * 40)
    print("\nSummary of operations performed:")
    print("✓ 7 vehicles registered")
    print("✓ 7 parking requests created")
    print("✓ 7 requests processed from queue")
    print("✓ Complete lifecycle demonstrated")
    print("✓ Cross-zone allocation triggered")
    print("✓ Cancellation with slot restoration")
    print("✓ Rollback operation demonstrated")
    print("✓ All analytics functions tested")
    print(f"\nFinal System State:")
    print(f"Total Slots: {system.get_total_slots()}")
    print(f"Available Slots: {system.get_available_slots()}")
    print(f"Total Requests: {system.get_total_requests()}")
    print(f"Active Requests: {system.get_active_requests()}")
    print("\n" + "=" * 40)

def main():
    """Main function."""
    system = ParkingSystem()
    test_suite = TestSuite()
    
    print("Initializing Smart Parking System...")
    print("System ready!")
    print("Press Enter to continue...")
    input()
    
    while True:
        display_main_menu()
        choice = get_choice(1, 16)
        
        if choice == 1:
            system.display_system_status()
        elif choice == 2:
            clear_screen()
            print("=== REGISTER NEW VEHICLE ===")
            vehicle_type = input("Vehicle Type: ")
            zone = input("Preferred Zone (Z1/Z2/Z3): ")
            system.add_vehicle(vehicle_type, zone)
        elif choice == 3:
            clear_screen()
            print("=== CREATE PARKING REQUEST ===")
            vehicle_id = input("Vehicle ID: ")
            zone = input("Requested Zone (Z1/Z2/Z3): ")
            req_id = system.create_parking_request(vehicle_id, zone)
            if req_id:
                print(f"Request created with ID: {req_id}")
        elif choice == 4:
            system.process_next_request()
        elif choice == 5:
            system.display_pending_requests()
        elif choice == 6:
            clear_screen()
            print("=== ALLOCATE SLOT TO REQUEST ===")
            req_id = input("Request ID: ")
            system.allocate_slot_to_request(req_id)
        elif choice == 7:
            clear_screen()
            print("=== MARK AS OCCUPIED ===")
            req_id = input("Request ID: ")
            system.mark_as_occupied(req_id)
        elif choice == 8:
            clear_screen()
            print("=== MARK AS RELEASED ===")
            req_id = input("Request ID: ")
            system.mark_as_released(req_id)
        elif choice == 9:
            clear_screen()
            print("=== CANCEL REQUEST ===")
            req_id = input("Request ID: ")
            system.cancel_request(req_id)
        elif choice == 10:
            system.display_all_vehicles()
        elif choice == 11:
            system.rollback_last_operation()
        elif choice == 12:
            run_rollback_menu(system)
        elif choice == 13:
            system_analytics_menu(system)
        elif choice == 14:
            test_suite.run_all_tests()
        elif choice == 15:
            run_auto_demo(system)
        elif choice == 16:
            print("Exiting Smart Parking System. Goodbye!")
            break
        
        if choice != 16 and choice != 13:
            input("\nPress Enter to continue...")

if __name__ == "__main__":
    main()