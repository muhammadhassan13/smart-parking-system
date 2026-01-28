class ParkingSlot:
    def __init__(self, slot_id=None, zone_id=None):
        self.slot_id = slot_id
        self.zone_id = zone_id
        self.is_available = True
        self.vehicle_id = ""
    
    def get_slot_id(self):
        return self.slot_id
    
    def get_zone_id(self):
        return self.zone_id
    
    def get_availability(self):
        return self.is_available
    
    def get_vehicle_id(self):
        return self.vehicle_id
    
    def set_availability(self, available):
        self.is_available = available
    
    def set_vehicle_id(self, vehicle_id):
        self.vehicle_id = vehicle_id
    
    def to_dict(self):
        """Serialize to dictionary."""
        return {
            'slot_id': self.slot_id,
            'zone_id': self.zone_id,
            'is_available': self.is_available,
            'vehicle_id': self.vehicle_id
        }
    
    @classmethod
    def from_dict(cls, data):
        """Create slot from dictionary."""
        slot = cls(
            slot_id=data.get('slot_id'),
            zone_id=data.get('zone_id')
        )
        slot.is_available = data.get('is_available', True)
        slot.vehicle_id = data.get('vehicle_id', "")
        return slot
    
    def display_slot_info(self):
        """Display slot information."""
        info = f"Slot ID: {self.slot_id}, Zone: {self.zone_id}, Available: {'Yes' if self.is_available else 'No'}"
        if not self.is_available and self.vehicle_id:
            info += f", Vehicle: {self.vehicle_id}"
        print(info)