from .parking_area import ParkingArea

class Zone:
    def __init__(self, zone_id=None, zone_name=None, max_areas=0):
        self.zone_id = zone_id
        self.zone_name = zone_name
        self.max_areas = max_areas
        self.current_areas = 0
        self.areas = []
        self.adjacent_zones = []  # List of adjacent zone IDs
    
    def get_zone_id(self):
        return self.zone_id
    
    def get_zone_name(self):
        return self.zone_name
    
    def get_max_areas(self):
        return self.max_areas
    
    def get_current_areas(self):
        return self.current_areas
    
    def add_area(self, area_id, max_slots):
        """Add an area to the zone."""
        if self.current_areas >= self.max_areas:
            print("Error: Cannot add more areas. Zone capacity reached!")
            return False
        
        area = ParkingArea(area_id, self.zone_id, max_slots)
        self.areas.append(area)
        self.current_areas += 1
        return True
    
    def find_area(self, area_id):
        """Find an area by ID."""
        for area in self.areas:
            if area.get_area_id() == area_id:
                return area
        return None
    
    def display_all_areas(self):
        """Display all areas in the zone."""
        print(f"\n=== Zone: {self.zone_name} ({self.zone_id}) ===")
        print(f"Total Areas: {self.current_areas}/{self.max_areas}")
        
        if self.current_areas == 0:
            print("No areas created yet.")
            return
        
        for i, area in enumerate(self.areas, 1):
            print(f"\nArea {i}: {area.get_area_id()}")
            print(f"Total Slots: {area.get_current_slots()}/{area.get_max_slots()}")
            print(f"Available Slots: {area.count_available_slots()}")
    
    def get_total_slots(self):
        """Get total number of slots in the zone."""
        total = 0
        for area in self.areas:
            total += area.get_current_slots()
        return total
    
    def get_available_slots(self):
        """Get number of available slots in the zone."""
        available = 0
        for area in self.areas:
            available += area.count_available_slots()
        return available
    
    def find_available_slot_in_zone(self):
        """Find an available slot in the zone."""
        for area in self.areas:
            slot = area.get_first_available_slot()
            if slot is not None:
                return slot
        return None
    
    def display_zone_info(self):
        """Display zone information."""
        print("\n=== Zone Information ===")
        print(f"Zone ID: {self.zone_id}")
        print(f"Zone Name: {self.zone_name}")
        print(f"Areas: {self.current_areas}/{self.max_areas}")
        print(f"Total Slots: {self.get_total_slots()}")
        print(f"Available Slots: {self.get_available_slots()}")
        print(f"Adjacent Zones: {', '.join(self.adjacent_zones) if self.adjacent_zones else 'None'}")
    
    def add_adjacent_zone(self, zone_id):
        """Add an adjacent zone."""
        if zone_id == self.zone_id:
            print("Error: Cannot add zone as adjacent to itself.")
            return False
        if zone_id in self.adjacent_zones:
            print(f"Zone {zone_id} is already adjacent to {self.zone_id}.")
            return False
        self.adjacent_zones.append(zone_id)
        print(f"Zone {zone_id} added as adjacent to {self.zone_id}.")
        return True
    
    def remove_adjacent_zone(self, zone_id):
        """Remove an adjacent zone."""
        if zone_id in self.adjacent_zones:
            self.adjacent_zones.remove(zone_id)
            return True
        return False
    
    def get_adjacent_zones(self):
        """Get list of adjacent zone IDs."""
        return self.adjacent_zones.copy()
    
    def is_adjacent(self, zone_id):
        """Check if a zone is adjacent."""
        return zone_id in self.adjacent_zones
    
    def get_zone_data(self):
        """Get zone data as dictionary for UI."""
        return {
            'zone_id': self.zone_id,
            'zone_name': self.zone_name,
            'total_slots': self.get_total_slots(),
            'available_slots': self.get_available_slots(),
            'areas': self.current_areas,
            'max_areas': self.max_areas,
            'adjacent_zones': self.adjacent_zones.copy()
        }