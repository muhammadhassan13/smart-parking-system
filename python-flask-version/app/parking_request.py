import datetime
from enum import Enum

class RequestState(Enum):
    REQUESTED = 1
    ALLOCATED = 2
    OCCUPIED = 3
    RELEASED = 4
    CANCELLED = 5

# Cost constants
BASE_PARKING_COST = 10.0  # Base cost for same-zone parking
CROSS_ZONE_ADJACENT_PENALTY = 3.0  # Extra cost for adjacent zone allocation
CROSS_ZONE_DISTANT_PENALTY = 5.0  # Extra cost for non-adjacent zone allocation

class ParkingRequest:
    def __init__(self, request_id=None, vehicle=None, zone_id=None):
        self.request_id = request_id
        self.vehicle = vehicle
        self.requested_zone_id = zone_id
        self.allocated_slot = None
        self.cross_zone_allocation = False
        self.is_adjacent_zone = False  # Track if cross-zone was to adjacent zone
        
        # Cost tracking
        self.base_cost = BASE_PARKING_COST
        self.penalty_cost = 0.0
        self.total_cost = BASE_PARKING_COST
        
        # Timestamps
        self.request_time = datetime.datetime.now()
        self.allocation_time = None
        self.release_time = None
        
        # State
        self.current_state = RequestState.REQUESTED
    
    def get_request_id(self):
        return self.request_id
    
    def get_vehicle(self):
        return self.vehicle
    
    def get_requested_zone_id(self):
        return self.requested_zone_id
    
    def get_request_time(self):
        return self.request_time
    
    def get_allocation_time(self):
        return self.allocation_time
    
    def get_release_time(self):
        return self.release_time
    
    def get_current_state(self):
        return self.current_state
    
    def get_allocated_slot(self):
        """Get the allocated slot."""
        return self.allocated_slot
    
    def get_allocated_slot_id(self):
        """Get the allocated slot ID safely."""
        return self.allocated_slot.get_slot_id() if self.allocated_slot else "N/A"
    
    def get_allocated_zone_id(self):
        """Get the allocated zone ID safely."""
        return self.allocated_slot.get_zone_id() if self.allocated_slot else "N/A"
    
    def get_vehicle_id(self):
        """Get the vehicle ID safely."""
        return self.vehicle.get_vehicle_id() if self.vehicle else "N/A"
    
    def is_cross_zone_allocation(self):
        return self.cross_zone_allocation
    
    def get_base_cost(self):
        """Get base parking cost."""
        return self.base_cost
    
    def get_penalty_cost(self):
        """Get cross-zone penalty cost."""
        return self.penalty_cost
    
    def get_total_cost(self):
        """Get total cost (base + penalty)."""
        return self.total_cost
    
    def get_request_data(self):
        """Get request data as dictionary for UI."""
        return {
            'request_id': self.request_id,
            'vehicle_id': self.get_vehicle_id(),
            'requested_zone': self.requested_zone_id,
            'allocated_slot': self.get_allocated_slot_id(),
            'allocated_zone': self.get_allocated_zone_id(),
            'state': self.state_to_string(),
            'cross_zone': self.cross_zone_allocation,
            'is_adjacent': self.is_adjacent_zone,
            'base_cost': self.base_cost,
            'penalty_cost': self.penalty_cost,
            'total_cost': self.total_cost,
            'request_time': self.request_time.strftime('%d/%m/%Y %H:%M:%S') if self.request_time else None,
            'duration': self.calculate_duration() if self.current_state == RequestState.RELEASED else 0
        }
    
    def allocate_slot(self, slot, cross_zone=False, is_adjacent=False):
        """Allocate a slot to this request."""
        if self.current_state != RequestState.REQUESTED:
            print("Error: Cannot allocate slot. Request is not in REQUESTED state.")
            return False
        
        if slot is None:
            print("Error: Cannot allocate null slot.")
            return False
        
        if not slot.get_availability():
            print("Error: Slot is not available.")
            return False
        
        self.allocated_slot = slot
        self.cross_zone_allocation = cross_zone
        self.is_adjacent_zone = is_adjacent
        self.current_state = RequestState.ALLOCATED
        self.allocation_time = datetime.datetime.now()
        
        # Calculate costs
        self.base_cost = BASE_PARKING_COST
        if cross_zone:
            if is_adjacent:
                self.penalty_cost = CROSS_ZONE_ADJACENT_PENALTY
            else:
                self.penalty_cost = CROSS_ZONE_DISTANT_PENALTY
        else:
            self.penalty_cost = 0.0
        self.total_cost = self.base_cost + self.penalty_cost
        
        # Mark slot as occupied
        slot.set_availability(False)
        slot.set_vehicle_id(self.vehicle.get_vehicle_id())
        
        return True
    
    def mark_as_occupied(self):
        """Mark the request as occupied."""
        if self.current_state != RequestState.ALLOCATED:
            print("Error: Cannot mark as occupied. Request is not in ALLOCATED state.")
            return False
        
        self.current_state = RequestState.OCCUPIED
        return True
    
    def mark_as_released(self):
        """Mark the request as released."""
        if self.current_state != RequestState.OCCUPIED:
            print("Error: Cannot release. Request is not in OCCUPIED state.")
            return False
        
        self.current_state = RequestState.RELEASED
        self.release_time = datetime.datetime.now()
        
        # Free the slot
        if self.allocated_slot is not None:
            self.allocated_slot.set_availability(True)
            self.allocated_slot.set_vehicle_id("")
        
        return True
    
    def cancel_request(self):
        """Cancel the parking request."""
        if self.current_state in [RequestState.RELEASED, RequestState.CANCELLED]:
            print("Error: Cannot cancel. Request is already completed or cancelled.")
            return False
        
        # Free the slot if it was allocated
        if self.allocated_slot is not None and self.current_state == RequestState.ALLOCATED:
            self.allocated_slot.set_availability(True)
            self.allocated_slot.set_vehicle_id("")
        
        self.current_state = RequestState.CANCELLED
        return True
    
    def to_dict(self):
        """Serialize to dictionary."""
        return {
            'request_id': self.request_id,
            'vehicle_id': self.vehicle.get_vehicle_id() if self.vehicle else None,
            'requested_zone_id': self.requested_zone_id,
            'allocated_slot_id': self.allocated_slot.get_slot_id() if self.allocated_slot else None,
            'cross_zone_allocation': self.cross_zone_allocation,
            'is_adjacent_zone': self.is_adjacent_zone,
            'base_cost': self.base_cost,
            'penalty_cost': self.penalty_cost,
            'total_cost': self.total_cost,
            'current_state': self.current_state.name,
            'request_time': self.request_time.strftime('%Y-%m-%d %H:%M:%S') if self.request_time else None,
            'allocation_time': self.allocation_time.strftime('%Y-%m-%d %H:%M:%S') if self.allocation_time else None,
            'release_time': self.release_time.strftime('%Y-%m-%d %H:%M:%S') if self.release_time else None
        }

    @classmethod
    def from_dict(cls, data):
        """Create request from dictionary."""
        req = cls(
            request_id=data.get('request_id'),
            zone_id=data.get('requested_zone_id')
        )
        # Note: Vehicle and Slot must be linked separately
        
        req.cross_zone_allocation = data.get('cross_zone_allocation', False)
        req.is_adjacent_zone = data.get('is_adjacent_zone', False)
        req.base_cost = data.get('base_cost', BASE_PARKING_COST)
        req.penalty_cost = data.get('penalty_cost', 0.0)
        req.total_cost = data.get('total_cost', BASE_PARKING_COST)
        
        if 'current_state' in data:
            try:
                req.current_state = RequestState[data['current_state']]
            except KeyError:
                pass
                
        # Parse times
        for time_field in ['request_time', 'allocation_time', 'release_time']:
            if data.get(time_field):
                try:
                    dt = datetime.datetime.strptime(data[time_field], '%Y-%m-%d %H:%M:%S')
                    setattr(req, time_field, dt)
                except ValueError:
                    pass
                    
        return req
    
    def display_request_info(self):
        """Display request information."""
        print("\n=== Parking Request Details ===")
        print(f"Request ID: {self.request_id}")
        print(f"Vehicle ID: {self.vehicle.get_vehicle_id() if self.vehicle else 'None'}")
        print(f"Requested Zone: {self.requested_zone_id}")
        print(f"Current State: {self.state_to_string()}")
        print(f"Cross Zone Allocation: {'Yes' if self.cross_zone_allocation else 'No'}")
        
        if self.allocated_slot is not None:
            print(f"Allocated Slot: {self.allocated_slot.get_slot_id()} "
                  f"(Zone: {self.allocated_slot.get_zone_id()})")
            
            # Display costs
            print(f"Base Cost: ${self.base_cost:.2f}")
            if self.penalty_cost > 0:
                print(f"Cross-Zone Penalty: ${self.penalty_cost:.2f}")
            print(f"Total Cost: ${self.total_cost:.2f}")
        else:
            print("Allocated Slot: None")
        
        # Display times
        if self.request_time:
            print(f"Request Time: {self.request_time.strftime('%d/%m/%Y %H:%M:%S')}")
        
        if self.allocation_time:
            print(f"Allocation Time: {self.allocation_time.strftime('%d/%m/%Y %H:%M:%S')}")
        
        if self.release_time:
            print(f"Release Time: {self.release_time.strftime('%d/%m/%Y %H:%M:%S')}")
        
        if self.current_state == RequestState.RELEASED:
            print(f"Duration: {self.calculate_duration():.2f} minutes")
    
    def state_to_string(self):
        """Convert state to string representation."""
        state_map = {
            RequestState.REQUESTED: "REQUESTED",
            RequestState.ALLOCATED: "ALLOCATED",
            RequestState.OCCUPIED: "OCCUPIED",
            RequestState.RELEASED: "RELEASED",
            RequestState.CANCELLED: "CANCELLED"
        }
        return state_map.get(self.current_state, "UNKNOWN")
    
    def calculate_duration(self):
        """Calculate parking duration in minutes."""
        if self.current_state != RequestState.RELEASED or self.release_time is None:
            return 0.0
        
        end_time = self.release_time
        start_time = self.allocation_time if self.allocation_time else self.request_time
        
        duration = (end_time - start_time).total_seconds() / 60.0
        return duration
    
    def is_active(self):
        """Check if request is active."""
        return self.current_state in [RequestState.REQUESTED, RequestState.ALLOCATED, RequestState.OCCUPIED]