from .parking_request import ParkingRequest

class RequestQueue:
    class QueueNode:
        def __init__(self, request):
            self.request = request
            self.next = None
    
    def __init__(self, max_size=100):
        self.front = None
        self.rear = None
        self.queue_size = 0
        self.max_size = max_size
    
    def enqueue(self, request):
        """Enqueue a request."""
        if self.is_full():
            print("Error: Request queue is full. Cannot add more requests.")
            return False
        
        new_node = self.QueueNode(request)
        
        if self.is_empty():
            self.front = self.rear = new_node
        else:
            self.rear.next = new_node
            self.rear = new_node
        
        self.queue_size += 1
        return True
    
    def dequeue(self):
        """Dequeue a request."""
        if self.is_empty():
            return None
        
        temp = self.front
        request = temp.request
        
        self.front = self.front.next
        if self.front is None:
            self.rear = None
        
        self.queue_size -= 1
        return request
    
    def peek(self):
        """Peek at the front request."""
        if self.is_empty():
            return None
        return self.front.request
    
    def is_empty(self):
        """Check if queue is empty."""
        return self.front is None
    
    def is_full(self):
        """Check if queue is full."""
        return self.queue_size >= self.max_size
    
    def get_size(self):
        """Get queue size."""
        return self.queue_size
    
    def display_queue(self):
        """Display the queue."""
        print("\n=== PENDING REQUESTS QUEUE ===")
        print(f"Queue Size: {self.queue_size}/{self.max_size}")
        
        if self.is_empty():
            print("Queue is empty.")
            return
        
        current = self.front
        counter = 1
        
        while current is not None:
            print(f"\n{counter}. ", end="")
            print(f"Request ID: {current.request.get_request_id()}", end="")
            print(f", Vehicle: {current.request.get_vehicle().get_vehicle_id()}", end="")
            print(f", Zone: {current.request.get_requested_zone_id()}", end="")
            print(f", State: {current.request.state_to_string()}")
            current = current.next
            counter += 1
    
    def get_all_requests(self):
        """Get all requests as a list."""
        requests = []
        current = self.front
        while current is not None:
            requests.append(current.request)
            current = current.next
        return requests
    
    def clear(self):
        """Clear the queue."""
        while not self.is_empty():
            self.dequeue()