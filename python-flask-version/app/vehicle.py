import datetime

class Vehicle:
    def __init__(self, vehicle_id=None, vehicle_type=None, preferred_zone=None, license_plate="", owner_name=""):
        self.vehicle_id = vehicle_id
        self.vehicle_type = vehicle_type
        self.preferred_zone = preferred_zone
        self.license_plate = license_plate
        self.owner_name = owner_name
        self.registration_time = datetime.datetime.now()
    
    def get_vehicle_id(self):
        return self.vehicle_id
    
    def get_vehicle_type(self):
        return self.vehicle_type
    
    def get_preferred_zone(self):
        return self.preferred_zone
    
    def get_license_plate(self):
        return self.license_plate
    
    def get_owner_name(self):
        return self.owner_name
    
    def get_registration_time(self):
        return self.registration_time.strftime("%d/%m/%Y %H:%M:%S")
    
    def set_preferred_zone(self, zone):
        self.preferred_zone = zone
    
    def display_vehicle_info(self):
        """Display vehicle information."""
        print(f"Vehicle ID: {self.vehicle_id}, "
              f"License: {self.license_plate}, "
              f"Owner: {self.owner_name}, "
              f"Type: {self.vehicle_type}, "
              f"Preferred Zone: {self.preferred_zone}, "
              f"Registered: {self.get_registration_time()}")
    
    def to_dict(self):
        """Serialize to dictionary."""
        return {
            'vehicle_id': self.vehicle_id,
            'vehicle_type': self.vehicle_type,
            'preferred_zone': self.preferred_zone,
            'license_plate': self.license_plate,
            'owner_name': self.owner_name,
            'registration_time': self.registration_time.strftime("%Y-%m-%d %H:%M:%S")
        }
    
    @classmethod
    def from_dict(cls, data):
        """Create vehicle from dictionary."""
        vehicle = cls(
            vehicle_id=data.get('vehicle_id'),
            vehicle_type=data.get('vehicle_type'),
            preferred_zone=data.get('preferred_zone'),
            license_plate=data.get('license_plate', ""),
            owner_name=data.get('owner_name', "")
        )
        if 'registration_time' in data:
            try:
                vehicle.registration_time = datetime.datetime.strptime(
                    data['registration_time'], "%Y-%m-%d %H:%M:%S"
                )
            except ValueError:
                pass
        return vehicle