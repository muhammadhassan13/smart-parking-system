from .parking_request import ParkingRequest, RequestState

class RequestNode:
    def __init__(self, request):
        self.request = request
        self.next = None

class RequestManager:
    def __init__(self):
        self.head = None
        self.tail = None
        self.request_count = 0
    
    def add_request(self, request):
        """Add a request to the manager."""
        if request is None:
            print("Error: Cannot add null request.")
            return False
        
        new_node = RequestNode(request)
        
        if self.head is None:
            self.head = new_node
            self.tail = new_node
        else:
            self.tail.next = new_node
            self.tail = new_node
        
        self.request_count += 1
        return True
    
    def find_request(self, request_id):
        """Find a request by ID."""
        current = self.head
        while current is not None:
            if current.request.get_request_id() == request_id:
                return current.request
            current = current.next
        return None
    
    def remove_request(self, request_id):
        """Remove a request by ID."""
        if self.head is None:
            return False
        
        # Special case: head node
        if self.head.request.get_request_id() == request_id:
            temp = self.head
            self.head = self.head.next
            if self.head is None:
                self.tail = None
            self.request_count -= 1
            return True
        
        # Search for node to delete
        current = self.head
        while current.next is not None:
            if current.next.request.get_request_id() == request_id:
                temp = current.next
                current.next = temp.next
                
                if temp == self.tail:
                    self.tail = current
                
                self.request_count -= 1
                return True
            current = current.next
        
        return False
    
    def get_request_count(self):
        """Get the number of requests."""
        return self.request_count
    
    def get_all_requests(self):
        """Get all requests as a list."""
        requests = []
        current = self.head
        while current is not None:
            requests.append(current.request)
            current = current.next
        return requests
    
    def display_all_requests(self):
        """Display all requests."""
        print(f"\n=== ALL PARKING REQUESTS ({self.request_count}) ===")
        
        if self.head is None:
            print("No requests found.")
            return
        
        current = self.head
        counter = 1
        
        while current is not None:
            print(f"\n{counter}. ", end="")
            current.request.display_request_info()
            current = current.next
            counter += 1
    
    def display_active_requests(self):
        """Display active requests."""
        print("\n=== ACTIVE PARKING REQUESTS ===")
        
        current = self.head
        active_count = 0
        counter = 1
        
        while current is not None:
            if current.request.is_active():
                print(f"\n{counter}. ", end="")
                current.request.display_request_info()
                active_count += 1
                counter += 1
            current = current.next
        
        if active_count == 0:
            print("No active requests found.")
        else:
            print(f"\nTotal active requests: {active_count}")
    
    def display_request_history(self):
        """Display request history."""
        print("\n=== REQUEST HISTORY ===")
        
        current = self.head
        completed = 0
        cancelled = 0
        active = 0
        counter = 1
        
        # First pass: count statistics
        while current is not None:
            state = current.request.get_current_state()
            if state == RequestState.RELEASED:
                completed += 1
            elif state == RequestState.CANCELLED:
                cancelled += 1
            elif current.request.is_active():
                active += 1
            current = current.next
        
        print(f"Total Requests: {self.request_count}")
        print(f"Completed: {completed}")
        print(f"Cancelled: {cancelled}")
        print(f"Active: {active}")
        print(f"Average Duration: {self.get_average_duration():.2f} minutes")
        
        # Display all requests
        current = self.head
        print("\nDetailed History:")
        while current is not None:
            print(f"{counter}. ", end="")
            print(f"ID: {current.request.get_request_id()}", end="")
            print(f", Vehicle: {current.request.get_vehicle().get_vehicle_id()}", end="")
            print(f", State: {current.request.state_to_string()}", end="")
            print(f", Zone: {current.request.get_requested_zone_id()}", end="")
            
            if current.request.get_allocated_slot() is not None:
                print(f", Slot: {current.request.get_allocated_slot().get_slot_id()}", end="")
            
            print()
            current = current.next
            counter += 1
    
    def count_by_state(self, state):
        """Count requests by state."""
        count = 0
        current = self.head
        
        while current is not None:
            if current.request.get_current_state() == state:
                count += 1
            current = current.next
        
        return count
    
    def get_average_duration(self):
        """Get average parking duration."""
        current = self.head
        total_duration = 0.0
        completed_count = 0
        
        while current is not None:
            if current.request.get_current_state() == RequestState.RELEASED:
                total_duration += current.request.calculate_duration()
                completed_count += 1
            current = current.next
        
        if completed_count == 0:
            return 0.0
        
        return total_duration / completed_count
    
    def get_requests_by_state(self, state):
        """Get all requests with a specific state."""
        requests = []
        current = self.head
        
        while current is not None:
            if current.request.get_current_state() == state:
                requests.append(current.request)
            current = current.next
        
        return requests
    
    def get_pending_requests(self):
        """Get all pending (REQUESTED state) requests."""
        return self.get_requests_by_state(RequestState.REQUESTED)
    
    def get_allocated_requests(self):
        """Get all allocated and occupied requests."""
        allocated = self.get_requests_by_state(RequestState.ALLOCATED)
        occupied = self.get_requests_by_state(RequestState.OCCUPIED)
        return allocated + occupied

    def get_total_revenue(self):
        """Calculate total revenue from completed and active requests."""
        total_revenue = 0.0
        current = self.head
        while current is not None:
            state = current.request.get_current_state()
            # Count revenue for any request that was allocated (including released)
            if state in [RequestState.ALLOCATED, RequestState.OCCUPIED, RequestState.RELEASED]:
                total_revenue += current.request.get_total_cost()
            current = current.next
        return total_revenue

    def get_cross_zone_count(self):
        """Count number of cross-zone allocations."""
        count = 0
        current = self.head
        while current is not None:
            if current.request.is_cross_zone_allocation():
                count += 1
            current = current.next
        return count

    def get_zone_request_distribution(self):
        """Get distribution of requests by requested zone."""
        distribution = {}
        current = self.head
        while current is not None:
            zone_id = current.request.get_requested_zone_id()
            distribution[zone_id] = distribution.get(zone_id, 0) + 1
            current = current.next
        return distribution