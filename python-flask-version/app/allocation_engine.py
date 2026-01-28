class AllocationEngine:
    def __init__(self, max_zones):
        self.max_zones = max_zones
        self.zone_count = 0
        self.zones = [None] * max_zones
    
    def add_zone(self, zone):
        """Add a zone to the engine."""
        if self.zone_count >= self.max_zones:
            print("Error: Cannot add more zones to engine.")
            return False
        
        # Check if zone already exists
        for i in range(self.zone_count):
            if self.zones[i].get_zone_id() == zone.get_zone_id():
                print(f"Error: Zone {zone.get_zone_id()} already exists in engine.")
                return False
        
        self.zones[self.zone_count] = zone
        self.zone_count += 1
        return True
    
    def find_zone(self, zone_id):
        """Find a zone by ID."""
        for i in range(self.zone_count):
            if self.zones[i].get_zone_id() == zone_id:
                return self.zones[i]
        return None
    
    def allocate_slot(self, request):
        """Allocate a slot for a parking request."""
        if request is None:
            print("Error: Cannot allocate slot for null request.")
            return None
        
        requested_zone_id = request.get_requested_zone_id()
        requested_zone = self.find_zone(requested_zone_id)
        
        if requested_zone is None:
            print(f"Error: Requested zone {requested_zone_id} not found.")
            return None
        
        # Try to allocate in requested zone first
        slot = requested_zone.find_available_slot_in_zone()
        
        if slot is not None:
            # Allocation successful in requested zone
            return slot
        
        # If requested zone is full, try cross-zone allocation
        print(f"Zone {requested_zone_id} is full. Attempting cross-zone allocation...")
        return self.allocate_cross_zone(request)
    
    def allocate_cross_zone(self, request):
        """Allocate a slot in a different zone (cross-zone allocation).
        Priority: Adjacent zones first, then any available zone."""
        current_zone = self.find_zone(request.get_requested_zone_id())
        if current_zone is None:
            return None
        
        # First, try adjacent zones
        adjacent_zone_ids = current_zone.get_adjacent_zones()
        for adj_zone_id in adjacent_zone_ids:
            adj_zone = self.find_zone(adj_zone_id)
            if adj_zone is not None and adj_zone.get_available_slots() > 0:
                slot = adj_zone.find_available_slot_in_zone()
                if slot is not None:
                    print(f"Cross-zone allocation to adjacent zone {adj_zone_id} successful!")
                    return slot
        
        # If no adjacent zone available, try any zone
        print("No adjacent zones available. Trying other zones...")
        next_zone = self._get_next_available_zone(request.get_requested_zone_id())
        
        if next_zone is not None:
            slot = next_zone.find_available_slot_in_zone()
            if slot is not None:
                print(f"Cross-zone allocation to zone {next_zone.get_zone_id()} successful!")
                return slot
        
        print("Error: No available slots in any zone.")
        return None
    
    def _get_next_available_zone(self, current_zone_id):
        """Get the next available zone using round-robin search."""
        current_index = -1
        
        # Find current zone index
        for i in range(self.zone_count):
            if self.zones[i].get_zone_id() == current_zone_id:
                current_index = i
                break
        
        if current_index == -1:
            return None
        
        # Search for next zone with available slots
        for offset in range(1, self.zone_count + 1):
            next_index = (current_index + offset) % self.zone_count
            if self.zones[next_index].get_available_slots() > 0:
                return self.zones[next_index]
        
        return None
    
    def display_all_zones(self):
        """Display all zones in the engine."""
        print("\n=== All Zones in Allocation Engine ===")
        print(f"Total Zones: {self.zone_count}")
        
        for i in range(self.zone_count):
            self.zones[i].display_zone_info()
            print()
    
    def get_total_available_slots(self):
        """Get total available slots across all zones."""
        total = 0
        for i in range(self.zone_count):
            total += self.zones[i].get_available_slots()
        return total