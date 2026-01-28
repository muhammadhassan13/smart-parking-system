import datetime
from .zone import Zone
from .allocation_engine import AllocationEngine
from .request_manager import RequestManager, RequestState
from .parking_request import ParkingRequest
from .rollback_manager import RollbackManager
from .request_queue import RequestQueue
from .vehicle_bst import VehicleBST
from .vehicle import Vehicle
from .data_manager import DataManager

class ParkingSystem:
    def __init__(self, max_zones=10):
        self.max_zones = max_zones
        self.zone_count = 0
        self.zones = []
        
        # Counters for ID generation
        self.next_vehicle_id = 1000
        self.next_request_id = 1000
        
        # Create core components
        self.allocation_engine = AllocationEngine(max_zones)
        self.request_manager = RequestManager()
        self.rollback_manager = RollbackManager()
        self.request_queue = RequestQueue()
        self.vehicle_bst = VehicleBST()
        
        # Initialize with default zones
        self._initialize_default_zones()
        
        # Load data if available
        if self.load_system_state():
            print("System state restored from file.")
        else:
            print(f"Parking System initialized with {self.zone_count} zones.")
    
    def _initialize_default_zones(self):
        """Initialize the system with default zones."""
        # Create default zones
        self.add_zone("Z1", "Downtown", 3)
        self.add_zone("Z2", "Uptown", 2)
        self.add_zone("Z3", "Midtown", 2)
        
        # Add areas to zones
        self.add_area_to_zone("Z1", "A1", 3)
        self.add_area_to_zone("Z1", "A2", 2)
        self.add_area_to_zone("Z2", "B1", 4)
        self.add_area_to_zone("Z3", "C1", 8)
        
        # Add slots to areas
        self.add_slot_to_area("Z1", "A1", "Z1-A1-S1")
        self.add_slot_to_area("Z1", "A1", "Z1-A1-S2")
        self.add_slot_to_area("Z1", "A1", "Z1-A1-S3")
        self.add_slot_to_area("Z1", "A2", "Z1-A2-S1")
        self.add_slot_to_area("Z1", "A2", "Z1-A2-S2")
        self.add_slot_to_area("Z2", "B1", "Z2-B1-S1")
        self.add_slot_to_area("Z2", "B1", "Z2-B1-S2")
        self.add_slot_to_area("Z2", "B1", "Z2-B1-S3")
        self.add_slot_to_area("Z2", "B1", "Z2-B1-S4")
        self.add_slot_to_area("Z3", "C1", "Z3-C1-S1")
        self.add_slot_to_area("Z3", "C1", "Z3-C1-S2")
        self.add_slot_to_area("Z3", "C1", "Z3-C1-S3")
        self.add_slot_to_area("Z3", "C1", "Z3-C1-S4")
        self.add_slot_to_area("Z3", "C1", "Z3-C1-S5")
        self.add_slot_to_area("Z3", "C1", "Z3-C1-S6")
        self.add_slot_to_area("Z3", "C1", "Z3-C1-S7")
        self.add_slot_to_area("Z3", "C1", "Z3-C1-S8")
        
        # Set up zone adjacencies (bidirectional)
        # Z1 (Downtown) <-> Z2 (Uptown)
        # Z2 (Uptown) <-> Z3 (Midtown)
        # Z1 (Downtown) <-> Z3 (Midtown)
        self.add_zone_adjacency("Z1", "Z2")
        self.add_zone_adjacency("Z2", "Z3")
        self.add_zone_adjacency("Z1", "Z3")
    
    def add_zone(self, zone_id, zone_name, max_areas):
        """Add a new zone to the system."""
        if self.zone_count >= self.max_zones:
            print(f"Error: Cannot add more zones. Maximum {self.max_zones} reached.")
            return False
        
        # Check if zone already exists
        for zone in self.zones:
            if zone.get_zone_id() == zone_id:
                print(f"Error: Zone {zone_id} already exists.")
                return False
        
        new_zone = Zone(zone_id, zone_name, max_areas)
        self.zones.append(new_zone)
        self.allocation_engine.add_zone(new_zone)
        self.zone_count += 1
        
        print(f"Zone {zone_id} ({zone_name}) added successfully.")
        return True
    
    def add_area_to_zone(self, zone_id, area_id, max_slots):
        """Add an area to a zone."""
        zone = self.find_zone(zone_id)
        if zone is None:
            print(f"Error: Zone {zone_id} not found.")
            return False
        
        success = zone.add_area(area_id, max_slots)
        if success:
            print(f"Area {area_id} added to zone {zone_id} with capacity {max_slots} slots.")
        return success
    
    def add_slot_to_area(self, zone_id, area_id, slot_id):
        """Add a slot to an area."""
        zone = self.find_zone(zone_id)
        if zone is None:
            print(f"Error: Zone {zone_id} not found.")
            return False
        
        area = zone.find_area(area_id)
        if area is None:
            print(f"Error: Area {area_id} not found in zone {zone_id}")
            return False
        
        success = area.add_slot(slot_id)
        if success:
            print(f"Slot {slot_id} added to area {area_id} in zone {zone_id}")
        return success
    
    def find_zone(self, zone_id):
        """Find a zone by ID."""
        for zone in self.zones:
            if zone.get_zone_id() == zone_id:
                return zone
        return None
    
    def add_zone_adjacency(self, zone_id1, zone_id2):
        """Add bidirectional adjacency between two zones."""
        zone1 = self.find_zone(zone_id1)
        zone2 = self.find_zone(zone_id2)
        
        if zone1 is None:
            print(f"Error: Zone {zone_id1} not found.")
            return False
        if zone2 is None:
            print(f"Error: Zone {zone_id2} not found.")
            return False
        
        # Add bidirectional adjacency
        zone1.add_adjacent_zone(zone_id2)
        zone2.add_adjacent_zone(zone_id1)
        return True
    
    def get_zone_relationships(self):
        """Get zone relationship data for UI display."""
        zones_data = []
        for zone in self.zones:
            zones_data.append(zone.get_zone_data())
        
        # Build adjacency connections for graph visualization
        connections = []
        processed = set()
        for zone in self.zones:
            for adj_id in zone.get_adjacent_zones():
                pair = tuple(sorted([zone.get_zone_id(), adj_id]))
                if pair not in processed:
                    connections.append({
                        'from': zone.get_zone_id(),
                        'to': adj_id,
                        'from_name': zone.get_zone_name(),
                        'to_name': self.find_zone(adj_id).get_zone_name() if self.find_zone(adj_id) else adj_id
                    })
                    processed.add(pair)
        
        return {
            'zones': zones_data,
            'connections': connections,
            'total_zones': self.zone_count,
            'total_connections': len(connections)
        }
    
    def _generate_vehicle_id(self):
        """Generate a unique vehicle ID."""
        vehicle_id = f"V{self.next_vehicle_id}"
        self.next_vehicle_id += 1
        return vehicle_id
    
    def _generate_request_id(self):
        """Generate a unique request ID."""
        request_id = f"R{self.next_request_id}"
        self.next_request_id += 1
        return request_id
    
    def add_vehicle(self, vehicle_type, preferred_zone):
        """Add a new vehicle to the system."""
        vehicle_id = self._generate_vehicle_id()
        from .vehicle import Vehicle
        vehicle = Vehicle(vehicle_id, vehicle_type, preferred_zone)
        
        if self.vehicle_bst.insert(vehicle):
            print(f"Vehicle {vehicle_id} ({vehicle_type}) registered successfully.")
            print(f"Preferred Zone: {preferred_zone}")
            self.save_system_state()
            return vehicle_id
        else:
            print("Error: Failed to register vehicle.")
            return None
    
    def register_vehicle(self, vehicle_type, license_plate, owner_name, preferred_zone=None):
        """Register a new vehicle with detailed information."""
        # Check license plate uniqueness
        if not self.vehicle_bst.is_license_plate_unique(license_plate):
            print(f"Error: License plate {license_plate} is already registered.")
            return None

        vehicle_id = self._generate_vehicle_id()
        from .vehicle import Vehicle
        
        # If preferred_zone not provided, default to Z1
        if not preferred_zone:
            preferred_zone = "Z1"
            
        vehicle = Vehicle(vehicle_id, vehicle_type, preferred_zone, license_plate, owner_name)
        
        if self.vehicle_bst.insert(vehicle):
            print(f"Vehicle {vehicle_id} ({vehicle_type}) registered successfully.")
            print(f"License Plate: {license_plate}, Owner: {owner_name}, Preferred Zone: {preferred_zone}")
            self.save_system_state()
            return vehicle_id
        else:
            print("Error: Failed to register vehicle.")
            return None
    
    def find_vehicle(self, vehicle_id):
        """Find a vehicle by ID."""
        return self.vehicle_bst.search(vehicle_id)
    
    def display_all_vehicles(self):
        """Display all vehicles in the BST."""
        self.vehicle_bst.display_inorder()
    
    def create_parking_request(self, vehicle_id, requested_zone):
        """Create a new parking request."""
        # 1. Validate vehicle existence (no auto-register)
        vehicle = self.find_vehicle(vehicle_id)
        if vehicle is None:
            return f"Error: Vehicle {vehicle_id} is not registered. Please register the vehicle first."
        
        # 2. Validate zone capacity
        zone = self.find_zone(requested_zone)
        if zone is None:
            return f"Error: Zone {requested_zone} does not exist."
        
        if zone.get_available_slots() <= 0:
            return f"Error: Zone {requested_zone} is full. Please select a different zone."
        
        request_id = self._generate_request_id()
        from .parking_request import ParkingRequest
        request = ParkingRequest(request_id, vehicle, requested_zone)
        
        # Add to queue first
        if self.request_queue.enqueue(request):
            print(f"Parking request {request_id} created successfully.")
            self.save_system_state()
            return request_id # Success return ID as before for consistency
        else:
            return "Error: Request queue is full."
    
    def process_next_request(self):
        """Process the next request from the queue and attempt immediate allocation."""
        request = self.request_queue.dequeue()
        if request is None:
            return False, "No pending requests in queue."
        
        # Add to request manager
        self.request_manager.add_request(request)
        
        # Try to allocate immediately
        success, message = self.allocate_slot_to_request(request.get_request_id())
        return success, message
    
    def allocate_slot_to_request(self, request_id):
        """Allocate a slot to a specific request and return (success, message)."""
        request = self.request_manager.find_request(request_id)
        if request is None:
            return False, f"Error: Request {request_id} not found."
        
        if request.get_current_state() != RequestState.REQUESTED:
            return False, f"Error: Request is already {request.state_to_string()}."
        
        slot = self.allocation_engine.allocate_slot(request)
        if slot is None:
            return False, "Error: No available slots in the entire system."
        
        vehicle = request.get_vehicle()
        requested_zone_id = request.get_requested_zone_id()
        preferred_zone_id = vehicle.get_preferred_zone() if vehicle else requested_zone_id
        allocated_zone_id = slot.get_zone_id()
        
        # Cross zone check relative to preference
        cross_zone = (preferred_zone_id != allocated_zone_id)
        
        is_adjacent = False
        if cross_zone:
            preferred_zone = self.find_zone(preferred_zone_id)
            if preferred_zone and preferred_zone.is_adjacent(allocated_zone_id):
                is_adjacent = True
        
        success = request.allocate_slot(slot, cross_zone, is_adjacent)
        
        if success:
            self.rollback_manager.record_allocation(request_id, slot.get_slot_id(), slot.get_zone_id())
            self.save_system_state()
            
            # Construct success message with cross-zone details
            msg = f"Slot {request.get_allocated_slot_id()} allocated in {allocated_zone_id}."
            if preferred_zone_id != allocated_zone_id:
                reason = "Preferred zone was full" if preferred_zone_id == requested_zone_id else f"Redirected from preferred zone {preferred_zone_id}"
                msg += f" (Note: Cross-zone allocation occurred because {reason}; penalty of ${request.get_penalty_cost():.2f} applied)."
            
            return True, msg
        else:
            return False, "Error: Internal allocation failure."
    
    def mark_as_occupied(self, request_id):
        """Mark a request as occupied."""
        request = self.request_manager.find_request(request_id)
        if request is None:
            print(f"Error: Request {request_id} not found.")
            return False
        
        previous_state = request.get_current_state()
        success = request.mark_as_occupied()
        
        if success:
            self.rollback_manager.record_state_change(request_id, previous_state)
            print(f"Request {request_id} marked as OCCUPIED.")
            print(f"Vehicle is now parked in slot {request.get_allocated_slot_id()}")
            self.save_system_state()
        else:
            print("Error: Cannot mark as OCCUPIED.")
            print(f"Current state: {request.state_to_string()}")
            print("Required state: ALLOCATED")
        
        return success
    
    def mark_as_released(self, request_id):
        """Mark a request as released."""
        request = self.request_manager.find_request(request_id)
        if request is None:
            print(f"Error: Request {request_id} not found.")
            return False
        
        previous_state = request.get_current_state()
        success = request.mark_as_released()
        
        if success:
            self.rollback_manager.record_state_change(request_id, previous_state)
            print(f"Request {request_id} marked as RELEASED.")
            print(f"Slot {request.get_allocated_slot_id()} is now available.")
            print(f"Parking Duration: {request.calculate_duration():.2f} minutes")
            self.save_system_state()
        else:
            print("Error: Cannot mark as RELEASED.")
            print(f"Current state: {request.state_to_string()}")
            print("Required state: OCCUPIED")
        
        return success
    
    def cancel_request(self, request_id):
        """Cancel a parking request."""
        request = self.request_manager.find_request(request_id)
        if request is None:
            print(f"Error: Request {request_id} not found.")
            return False
        
        previous_state = request.get_current_state()
        success = request.cancel_request()
        
        if success:
            self.rollback_manager.record_cancellation(request_id)
            print(f"Request {request_id} cancelled successfully.")
            
            if previous_state == RequestState.ALLOCATED:
                print(f"Slot {request.get_allocated_slot_id()} has been freed.")
            self.save_system_state()
        else:
            print("Error: Cannot cancel request.")
            print(f"Current state: {request.state_to_string()}")
            print("Cancellation only allowed from REQUESTED or ALLOCATED states.")
        
        return success
    
    def display_pending_requests(self):
        """Display all pending requests in the queue."""
        self.request_queue.display_queue()
    
    def get_pending_request_count(self):
        """Get the number of pending requests."""
        return self.request_queue.get_size()
    
    def rollback_last_operation(self):
        """Rollback the last operation."""
        print("Attempting to rollback last operation...")
        success = self.rollback_manager.rollback_last_operation(self.request_manager, self.allocation_engine)
        if success:
            print("Rollback successful!")
            self.save_system_state()
        else:
            print("Rollback failed or no operations to rollback.")
        return success
    
    def rollback_last_k_operations(self, k):
        """Rollback the last k operations."""
        print(f"Attempting to rollback last {k} operations...")
        success = self.rollback_manager.rollback_last_k_operations(k, self.request_manager, self.allocation_engine)
        if success:
            print(f"Rollback of {k} operations successful!")
            self.save_system_state()
        else:
            print("Rollback failed.")
        return success
    
    def display_system_status(self):
        """Display the system status."""
        print("\n" + "=" * 40)
        print("       SYSTEM STATUS REPORT")
        print("=" * 40)
        
        print("\n--- Zones Summary ---")
        print(f"Total Zones: {self.zone_count}")
        for zone in self.zones:
            print(f"  {zone.get_zone_id()} ({zone.get_zone_name()}): "
                  f"{zone.get_available_slots()}/{zone.get_total_slots()} slots available")
        
        print("\n--- Capacity Summary ---")
        print(f"Total Slots: {self.get_total_slots()}")
        print(f"Available Slots: {self.get_available_slots()}")
        print(f"Occupied Slots: {self.get_total_slots() - self.get_available_slots()}")
        
        print("\n--- Requests Summary ---")
        print(f"Total Requests: {self.get_total_requests()}")
        print(f"Active Requests: {self.get_active_requests()}")
        print(f"Pending in Queue: {self.get_pending_request_count()}")
        
        print("\n--- Vehicles Summary ---")
        print(f"Registered Vehicles: {self.vehicle_bst.get_count()}")
        
        print("\n--- Rollback Status ---")
        print(f"Available Rollbacks: {self.rollback_manager.get_available_rollbacks()}")
        
        print("\n" + "=" * 40)
    
    def display_zone_analytics(self):
        """Display zone analytics."""
        print("\n" + "=" * 40)
        print("         ZONE ANALYTICS")
        print("=" * 40)
        
        for zone in self.zones:
            total_slots = zone.get_total_slots()
            available_slots = zone.get_available_slots()
            occupied_slots = total_slots - available_slots
            utilization = (occupied_slots * 100.0 / total_slots) if total_slots > 0 else 0.0
            
            print(f"\nZone: {zone.get_zone_name()} ({zone.get_zone_id()})")
            print(f"  Total Slots: {total_slots}")
            print(f"  Occupied Slots: {occupied_slots}")
            print(f"  Available Slots: {available_slots}")
            print(f"  Utilization Rate: {utilization:.2f}%")
            
            # Display utilization bar
            bar_length = 20
            filled = int((utilization / 100.0) * bar_length)
            print("  [" + "█" * filled + "░" * (bar_length - filled) + "]")
        
        print("\n" + "=" * 40)

    def get_comprehensive_analytics(self):
        """Get comprehensive analytics data for UI."""
        
        # Zone Analytics
        zones_data = []
        for zone in self.zones:
            total = zone.get_total_slots()
            available = zone.get_available_slots()
            occupied = total - available
            utilization = (occupied * 100.0 / total) if total > 0 else 0.0
            zones_data.append({
                'zone_id': zone.get_zone_id(),
                'zone_name': zone.get_zone_name(),
                'total_slots': total,
                'available_slots': available,
                'occupied_slots': occupied,
                'utilization': utilization
            })

        # Request Analytics
        total_requests = self.request_manager.get_request_count()
        completed = self.request_manager.count_by_state(RequestState.RELEASED)
        cancelled = self.request_manager.count_by_state(RequestState.CANCELLED)
        active = self.request_manager.count_by_state(RequestState.ALLOCATED) + \
                 self.request_manager.count_by_state(RequestState.OCCUPIED)
        
        # Financial Analytics
        total_revenue = self.request_manager.get_total_revenue()
        cross_zone_count = self.request_manager.get_cross_zone_count()
        
        return {
            'zones': zones_data,
            'requests': {
                'total': total_requests,
                'completed': completed,
                'cancelled': cancelled,
                'active': active,
                'distribution': self.request_manager.get_zone_request_distribution()
            },
            'financial': {
                'total_revenue': total_revenue,
                'cross_zone_count': cross_zone_count,
                'avg_revenue_per_request': (total_revenue / total_requests) if total_requests > 0 else 0
            }
        }
    
    def run_demo_scenario(self):
        """Run a demo scenario to populate data."""
        print("Running demo scenario...")
        
        # 1. Register diverse vehicles first
        demo_vehicles = [
            ("Sedan", "1234", "John Doe", "Z1"),
            ("SUV", "5678", "Jane Smith", "Z2"),
            ("Truck", "9012", "Bob Wilson", "Z1"),
            ("Compact", "3456", "Alice Brown", "Z3"),
            ("Luxury", "7890", "Charlie Green", "Z2"),
            ("EV", "1111", "Eve White", "Z1"),
            ("Van", "2222", "Frank Black", "Z3")
        ]
        
        vehicle_ids = []
        for v_type, plate, owner, pref_zone in demo_vehicles:
            vid = self.register_vehicle(v_type, plate, owner, pref_zone)
            if vid:
                vehicle_ids.append(vid)
        
        # 2. Create and process some requests
        if len(vehicle_ids) >= 5:
            # Create requests for the first 5 vehicles
            for i in range(5):
                zone_id = "Z1" if i % 2 == 0 else "Z2"
                req_id = self.create_parking_request(vehicle_ids[i], zone_id)
                if req_id and not req_id.startswith("Error"):
                    # Process and allocate some immediately
                    self.process_next_request()
            
            # 3. Mark some as occupied
            active_requests = self.request_manager.get_allocated_requests()
            if len(active_requests) >= 2:
                self.mark_as_occupied(active_requests[0].get_request_id())
                self.mark_as_occupied(active_requests[1].get_request_id())
            
            # 4. Release one to show completed status
            if len(active_requests) >= 1:
                req_to_release = active_requests[0].get_request_id()
                # Check if it's already occupied (required for release)
                req = self.request_manager.find_request(req_to_release)
                if req and req.get_current_state() == RequestState.OCCUPIED:
                    self.mark_as_released(req_to_release)
                else:
                    # If not occupied yet, mark it occupied then release
                    self.mark_as_occupied(req_to_release)
                    self.mark_as_released(req_to_release)
        
        # 5. Add one to queue without processing
        if len(vehicle_ids) >= 6:
            self.create_parking_request(vehicle_ids[5], "Z3")
            
        self.save_system_state()
        print("Demo scenario completed successfully.")
        return True
    
    def display_request_analytics(self):
        """Display request analytics."""
        print("\n" + "=" * 40)
        print("        REQUEST ANALYTICS")
        print("=" * 40)
        
        total = self.request_manager.get_request_count()
        completed = self.request_manager.count_by_state(RequestState.RELEASED)
        cancelled = self.request_manager.count_by_state(RequestState.CANCELLED)
        allocated = self.request_manager.count_by_state(RequestState.ALLOCATED)
        occupied = self.request_manager.count_by_state(RequestState.OCCUPIED)
        requested = self.request_manager.count_by_state(RequestState.REQUESTED)
        
        print("\n--- Status Distribution ---")
        print(f"Total Requests: {total}")
        print(f"  Completed (RELEASED): {completed}")
        print(f"  Cancelled: {cancelled}")
        print(f"  Active - ALLOCATED: {allocated}")
        print(f"  Active - OCCUPIED: {occupied}")
        print(f"  Pending - REQUESTED: {requested}")
        
        if total > 0:
            print("\n--- Percentages ---")
            print(f"  Completion Rate: {completed * 100.0 / total:.1f}%")
            print(f"  Cancellation Rate: {cancelled * 100.0 / total:.1f}%")
            print(f"  Active Rate: {(allocated + occupied) * 100.0 / total:.1f}%")
        
        print("\n--- Performance Metrics ---")
        print(f"Average Parking Duration: {self.request_manager.get_average_duration():.2f} minutes")
        
        print("\n" + "=" * 40)
    
    def display_peak_usage(self):
        """Display peak usage analysis."""
        print("\n" + "=" * 40)
        print("        PEAK USAGE ANALYSIS")
        print("=" * 40)
        
        busiest_zone = None
        most_available_zone = None
        max_utilization = 0
        min_utilization = 100
        
        for zone in self.zones:
            total_slots = zone.get_total_slots()
            available_slots = zone.get_available_slots()
            occupied_slots = total_slots - available_slots
            utilization = (occupied_slots * 100 // total_slots) if total_slots > 0 else 0
            
            if utilization > max_utilization:
                max_utilization = utilization
                busiest_zone = zone
            
            if utilization < min_utilization:
                min_utilization = utilization
                most_available_zone = zone
        
        if busiest_zone is not None:
            print("\n Busiest Zone:")
            print(f"  {busiest_zone.get_zone_name()} ({busiest_zone.get_zone_id()})")
            print(f"  Utilization: {max_utilization}%")
            print(f"  Available Slots: {busiest_zone.get_available_slots()}/{busiest_zone.get_total_slots()}")
        
        if most_available_zone is not None:
            print("\n Most Available Zone:")
            print(f"  {most_available_zone.get_zone_name()} ({most_available_zone.get_zone_id()})")
            print(f"  Utilization: {min_utilization}%")
            print(f"  Available Slots: {most_available_zone.get_available_slots()}/{most_available_zone.get_total_slots()}")
        
        print("\n--- Recommendations ---")
        if max_utilization > 80:
            print(f" High utilization detected. Consider adding more slots to {busiest_zone.get_zone_name()} zone.")
        
        if min_utilization < 20:
            print(f" Low utilization in {most_available_zone.get_zone_name()}. Consider promotional offers for this zone.")
        
        print("\n" + "=" * 40)
    
    def get_total_slots(self):
        """Get total number of slots in the system."""
        total = 0
        for zone in self.zones:
            total += zone.get_total_slots()
        return total
    
    def get_available_slots(self):
        """Get number of available slots in the system."""
        total = 0
        for zone in self.zones:
            total += zone.get_available_slots()
        return total
    
    def get_total_requests(self):
        """Get total number of requests."""
        return self.request_manager.get_request_count()
    
    def get_active_requests(self):
        """Get number of active requests."""
        return (self.request_manager.count_by_state(RequestState.ALLOCATED) + 
                self.request_manager.count_by_state(RequestState.OCCUPIED))
    
    def save_system_state(self):
        """Save system state to file."""
        data = {
            'role_counters': {
                'vehicle': self.next_vehicle_id,
                'request': self.next_request_id
            },
            'zones': [zone.get_zone_data() for zone in self.zones],
            'slots': [],
            'vehicles': [v.to_dict() for v in self.vehicle_bst.get_all_vehicles()],
            'requests': [r.to_dict() for r in self.request_manager.get_all_requests()]
        }
        
        # Save slot states
        for zone in self.zones:
            for area in zone.areas:
                for slot in area.slots:
                    data['slots'].append(slot.to_dict())
                    
        return DataManager.save_to_file(data)

    def load_system_state(self):
        """Load system state from file."""
        data = DataManager.load_from_file()
        if not data:
            return False
            
        print("Restoring system state...")
        
        # Restore counters
        if 'role_counters' in data:
            self.next_vehicle_id = data['role_counters'].get('vehicle', 1000)
            self.next_request_id = data['role_counters'].get('request', 1000)
            
        # Restore Zones (clear defaults if custom zones exist in data)
        if 'zones' in data and data['zones']:
            # Clear default zones before restoring
            self.zones = []
            self.zone_count = 0
            # Reset allocation engine zones correctly
            self.allocation_engine.zones = [None] * self.allocation_engine.max_zones
            self.allocation_engine.zone_count = 0
            
            for z_data in data['zones']:
                self.add_zone(z_data['zone_id'], z_data['zone_name'], z_data.get('max_areas', 5))
                # Restore adjacencies if they exist
                if 'adjacent_zones' in z_data:
                    zone = self.find_zone(z_data['zone_id'])
                    for adj_id in z_data['adjacent_zones']:
                        if adj_id not in zone.get_adjacent_zones():
                            self.add_zone_adjacency(z_data['zone_id'], adj_id)

        # Restore Vehicles
        if 'vehicles' in data:
            for v_data in data['vehicles']:
                vehicle = Vehicle.from_dict(v_data)
                self.vehicle_bst.insert(vehicle)
        
        # Restore Slots
        if 'slots' in data:
            for s_data in data['slots']:
                zone = self.find_zone(s_data['zone_id'])
                if zone:
                    parts = s_data['slot_id'].split('-')
                    if len(parts) >= 3:
                        area_id = parts[1]
                        area = zone.find_area(area_id)
                        if area:
                            for slot in area.slots:
                                if slot.get_slot_id() == s_data['slot_id']:
                                    slot.is_available = s_data['is_available']
                                    slot.vehicle_id = s_data['vehicle_id']
                                    break
        
        # Restore Requests
        if 'requests' in data:
            for r_data in data['requests']:
                req = ParkingRequest.from_dict(r_data)
                
                # Link vehicle
                if r_data.get('vehicle_id'):
                    req.vehicle = self.vehicle_bst.search(r_data['vehicle_id'])
                
                # Link slot
                if r_data.get('allocated_slot_id'):
                    parts = r_data['allocated_slot_id'].split('-')
                    zone_id = parts[0]
                    area_id = parts[1]
                    zone = self.find_zone(zone_id)
                    if zone:
                        area = zone.find_area(area_id)
                        if area:
                            found_slot = False
                            for slot in area.slots:
                                if slot.get_slot_id() == r_data['allocated_slot_id']:
                                    req.allocated_slot = slot
                                    found_slot = True
                                    break
                            if not found_slot:
                                print(f"Warning: Could not find slot {r_data['allocated_slot_id']} in zone {zone_id} area {area_id}")
                        else:
                            print(f"Warning: Could not find area {area_id} in zone {zone_id} for request {req.request_id}")
                    else:
                        print(f"Warning: Could not find zone {zone_id} for request {req.request_id}")
                
                self.request_manager.add_request(req)
                if req.current_state == RequestState.REQUESTED:
                    self.request_queue.enqueue(req)
                    
        return True

    def reset_system(self):
        """Reset the system to initial state."""
        self.zone_count = 0
        self.zones = []
        self.next_vehicle_id = 1000
        self.next_request_id = 1000
        
        # Re-create core components
        self.allocation_engine = AllocationEngine(self.max_zones)
        self.request_manager = RequestManager()
        self.rollback_manager = RollbackManager()
        self.request_queue = RequestQueue()
        self.vehicle_bst = VehicleBST()
        
        # Clear data file
        DataManager.delete_data_file()
        
        # Re-initialize default zones
        self._initialize_default_zones()
        
        print("System reset successfully.")
        return True