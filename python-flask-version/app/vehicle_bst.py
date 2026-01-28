from .vehicle import Vehicle

class VehicleBST:
    class BSTNode:
        def __init__(self, vehicle):
            self.vehicle = vehicle
            self.left = None
            self.right = None
    
    def __init__(self):
        self.root = None
        self.node_count = 0
    
    def _insert_rec(self, node, vehicle):
        """Recursive helper for insert."""
        if node is None:
            self.node_count += 1
            return self.BSTNode(vehicle)
        
        if vehicle.get_vehicle_id() < node.vehicle.get_vehicle_id():
            node.left = self._insert_rec(node.left, vehicle)
        elif vehicle.get_vehicle_id() > node.vehicle.get_vehicle_id():
            node.right = self._insert_rec(node.right, vehicle)
        # If equal, do nothing (vehicle IDs should be unique)
        
        return node
    
    def insert(self, vehicle):
        """Insert a vehicle into the BST."""
        if vehicle is None:
            return False
        
        self.root = self._insert_rec(self.root, vehicle)
        return True
    
    def _search_rec(self, node, vehicle_id):
        """Recursive helper for search."""
        if node is None:
            return None
        
        if vehicle_id == node.vehicle.get_vehicle_id():
            return node.vehicle
        elif vehicle_id < node.vehicle.get_vehicle_id():
            return self._search_rec(node.left, vehicle_id)
        else:
            return self._search_rec(node.right, vehicle_id)
    
    def search(self, vehicle_id):
        """Search for a vehicle by ID."""
        return self._search_rec(self.root, vehicle_id)
    
    def _inorder_rec(self, node):
        """Recursive helper for inorder traversal."""
        if node is None:
            return
        
        self._inorder_rec(node.left)
        node.vehicle.display_vehicle_info()
        self._inorder_rec(node.right)
    
    def display_inorder(self):
        """Display vehicles in inorder traversal."""
        print("\n=== VEHICLES (BST Inorder Traversal) ===")
        print(f"Total Vehicles: {self.node_count}")
        
        if self.root is None:
            print("No vehicles in BST.")
            return
        
        self._inorder_rec(self.root)
    
    def _collect_vehicles_rec(self, node, vehicles_list):
        """Recursive helper to collect all vehicles."""
        if node is None:
            return
        
        self._collect_vehicles_rec(node.left, vehicles_list)
        vehicles_list.append(node.vehicle)
        self._collect_vehicles_rec(node.right, vehicles_list)
    
    def get_all_vehicles(self):
        """Get all vehicles as a list."""
        vehicles = []
        self._collect_vehicles_rec(self.root, vehicles)
        return vehicles
    
    def _clear_rec(self, node):
        """Recursive helper for clear."""
        if node is None:
            return
        
        self._clear_rec(node.left)
        self._clear_rec(node.right)
        # Note: Vehicle objects are managed elsewhere
    
    def clear(self):
        """Clear the BST."""
        self._clear_rec(self.root)
        self.root = None
        self.node_count = 0
    
    def get_count(self):
        """Get the number of vehicles in the BST."""
        return self.node_count
    
    def is_license_plate_unique(self, license_plate):
        """Check if a license plate is already in the system."""
        if not license_plate:
            return True
        return self._check_license_plate_rec(self.root, license_plate)

    def _check_license_plate_rec(self, node, license_plate):
        if node is None:
            return True
        if node.vehicle.get_license_plate() == license_plate:
            return False
        return self._check_license_plate_rec(node.left, license_plate) and \
               self._check_license_plate_rec(node.right, license_plate)