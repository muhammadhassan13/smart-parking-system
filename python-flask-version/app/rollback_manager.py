import datetime
from enum import Enum
from .parking_request import RequestState

class RollbackType(Enum):
    ALLOCATION = 1
    CANCELLATION = 2
    STATE_CHANGE = 3

class RollbackOperation:
    def __init__(self, operation_type, request_id):
        self.type = operation_type
        self.request_id = request_id
        self.slot_id = ""
        self.zone_id = ""
        self.previous_state = None
        self.operation_time = datetime.datetime.now()

class RollbackStack:
    class StackNode:
        def __init__(self, operation):
            self.operation = operation
            self.next = None
    
    def __init__(self, max_operations=100):
        self.top = None
        self.stack_size = 0
        self.max_size = max_operations
    
    def push(self, operation):
        """Push an operation onto the stack."""
        if self.stack_size >= self.max_size:
            # Remove oldest operation if max size reached
            if self.top is None:
                return False
            
            # If only one node, clear it
            if self.top.next is None:
                self.top = None
                self.stack_size = 0
            else:
                # Find second last node
                current = self.top
                while current.next.next is not None:
                    current = current.next
                
                # Remove last node
                current.next = None
                self.stack_size -= 1
        
        new_node = self.StackNode(operation)
        new_node.next = self.top
        self.top = new_node
        self.stack_size += 1
        return True
    
    def pop(self):
        """Pop an operation from the stack."""
        if self.is_empty():
            return None
        
        temp = self.top
        operation = temp.operation
        self.top = self.top.next
        self.stack_size -= 1
        return operation
    
    def is_empty(self):
        """Check if stack is empty."""
        return self.top is None
    
    def get_size(self):
        """Get stack size."""
        return self.stack_size
    
    def clear(self):
        """Clear the stack."""
        while not self.is_empty():
            self.pop()

class RollbackManager:
    def __init__(self, max_operations=10):
        self.max_rollback_operations = max_operations
        self.operation_stack = RollbackStack(max_operations)
    
    def record_allocation(self, request_id, slot_id, zone_id):
        """Record an allocation operation."""
        op = RollbackOperation(RollbackType.ALLOCATION, request_id)
        op.slot_id = slot_id
        op.zone_id = zone_id
        self.operation_stack.push(op)
        print(f"Recorded allocation operation for request {request_id}")
    
    def record_cancellation(self, request_id):
        """Record a cancellation operation."""
        op = RollbackOperation(RollbackType.CANCELLATION, request_id)
        self.operation_stack.push(op)
        print(f"Recorded cancellation operation for request {request_id}")
    
    def record_state_change(self, request_id, previous_state):
        """Record a state change operation."""
        op = RollbackOperation(RollbackType.STATE_CHANGE, request_id)
        op.previous_state = previous_state
        self.operation_stack.push(op)
        print(f"Recorded state change operation for request {request_id}")
    
    def rollback_last_operation(self, request_manager, engine):
        """Rollback the last operation."""
        if self.operation_stack.is_empty():
            print("No operations to rollback.")
            return False
        
        op = self.operation_stack.pop()
        if op is None:
            return False
        
        success = False
        
        if op.type == RollbackType.ALLOCATION:
            success = self._undo_allocation(op, request_manager, engine)
        elif op.type == RollbackType.CANCELLATION:
            success = self._undo_cancellation(op, request_manager, engine)
        elif op.type == RollbackType.STATE_CHANGE:
            success = self._undo_state_change(op, request_manager)
        
        return success
    
    def rollback_last_k_operations(self, k, request_manager, engine):
        """Rollback the last k operations."""
        if k <= 0:
            print("Invalid number of operations to rollback.")
            return False
        
        available = self.operation_stack.get_size()
        if k > available:
            print(f"Only {available} operations available for rollback.")
            k = available
        
        print(f"Rolling back last {k} operations...")
        
        all_success = True
        for i in range(k):
            if not self.rollback_last_operation(request_manager, engine):
                all_success = False
                print(f"Failed to rollback operation {i + 1}")
        
        return all_success
    
    def _undo_allocation(self, op, request_manager, engine):
        """Undo an allocation operation."""
        request = request_manager.find_request(op.request_id)
        if request is None:
            print(f"Error: Request {op.request_id} not found for rollback.")
            return False
        
        print(f"Undoing allocation for request {op.request_id}...")
        
        # This is a simplified implementation
        # In a real system, you would restore the exact state
        request.cancel_request()
        
        print(f"Successfully rolled back allocation for request {op.request_id}")
        print(f"Slot {op.slot_id} is now available again.")
        return True
    
    def _undo_cancellation(self, op, request_manager, engine):
        """Undo a cancellation operation."""
        request = request_manager.find_request(op.request_id)
        if request is None:
            print(f"Error: Request {op.request_id} not found for rollback.")
            return False
        
        print(f"Undoing cancellation for request {op.request_id}...")
        
        # Simplified implementation
        print(f"Request {op.request_id} state changed from CANCELLED to REQUESTED")
        print("Note: Manual slot reallocation required.")
        return True
    
    def _undo_state_change(self, op, request_manager):
        """Undo a state change operation."""
        request = request_manager.find_request(op.request_id)
        if request is None:
            print(f"Error: Request {op.request_id} not found for rollback.")
            return False
        
        print(f"Undoing state change for request {op.request_id}...")
        
        # Log what we would do
        print("Would restore to state: ", end="")
        if op.previous_state == RequestState.REQUESTED:
            print("REQUESTED")
        elif op.previous_state == RequestState.ALLOCATED:
            print("ALLOCATED")
        elif op.previous_state == RequestState.OCCUPIED:
            print("OCCUPIED")
        elif op.previous_state == RequestState.RELEASED:
            print("RELEASED")
        elif op.previous_state == RequestState.CANCELLED:
            print("CANCELLED")
        
        return True
    
    def display_rollback_stack(self):
        """Display the rollback stack."""
        print("\n=== ROLLBACK STACK ===")
        print(f"Available rollbacks: {self.operation_stack.get_size()}")
        print(f"Max rollbacks: {self.max_rollback_operations}")
        
        if self.operation_stack.is_empty():
            print("Stack is empty.")
        else:
            print(f"\nTop operation in stack:")
            print(f"{self.operation_stack.get_size()} operations available for rollback.")
    
    def get_available_rollbacks(self):
        """Get number of available rollbacks."""
        return self.operation_stack.get_size()