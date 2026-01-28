from .parking_slot import ParkingSlot

class ParkingArea:
    def __init__(self, area_id=None, zone_id=None, max_slots=0):
        self.area_id = area_id
        self.zone_id = zone_id
        self.max_slots = max_slots
        self.current_slots = 0
        self.slots = []
    
    def get_area_id(self):
        return self.area_id
    
    def get_zone_id(self):
        return self.zone_id
    
    def get_max_slots(self):
        return self.max_slots
    
    def get_current_slots(self):
        return self.current_slots
    
    def add_slot(self, slot_id):
        """Add a slot to the area."""
        if self.current_slots >= self.max_slots:
            print("Error: Cannot add more slots. Area is full!")
            return False
        
        slot = ParkingSlot(slot_id, self.zone_id)
        self.slots.append(slot)
        self.current_slots += 1
        return True
    
    def find_slot(self, slot_id):
        """Find a slot by ID."""
        for slot in self.slots:
            if slot.get_slot_id() == slot_id:
                return slot
        return None
    
    def get_first_available_slot(self):
        """Get the first available slot in the area."""
        for slot in self.slots:
            if slot.get_availability():
                return slot
        return None
    
    def display_all_slots(self):
        """Display all slots in the area."""
        print(f"\n=== Area: {self.area_id} (Zone: {self.zone_id}) ===")
        print(f"Total Slots: {self.current_slots}/{self.max_slots}")
        
        if self.current_slots == 0:
            print("No slots created yet.")
            return
        
        for i, slot in enumerate(self.slots, 1):
            print(f"{i}. ", end="")
            slot.display_slot_info()
    
    def count_available_slots(self):
        """Count available slots in the area."""
        count = 0
        for slot in self.slots:
            if slot.get_availability():
                count += 1
        return count