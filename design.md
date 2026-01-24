SMART PARKING ALLOCATION SYSTEM

DESIGN DOCUMENTATION

* * *

1.  SYSTEM ARCHITECTURE OVERVIEW
    

The system follows a modular design with clearly separated components to ensure maintainability, scalability, and clarity.

Data Structures Used:

-   Arrays: Used for Zones, Areas, and Slots storage
    
-   Linked List: RequestManager for handling parking requests dynamically
    
-   Stack: RollbackManager for undo operations (LIFO)
    
-   Queue: RequestQueue for pending requests (FIFO)
    
-   Binary Search Tree (BST): VehicleBST for efficient vehicle lookup
    
-   State Machine: ParkingRequest lifecycle management
    

Class Hierarchy:  
ParkingSystem (Main Controller)

-   AllocationEngine
    
-   RequestManager (Linked List)
    
-   RollbackManager (Stack)
    
-   RequestQueue (Queue)
    
-   VehicleBST (Binary Search Tree)
    
-   Zone (Array of ParkingAreas)
    
    -   ParkingArea (Array of ParkingSlots)
        
        -   ParkingSlot
            
-   Vehicle
    
-   ParkingRequest (State Machine)
    

* * *

2.  ZONE AND SLOT REPRESENTATION
    

Zone Implementation:

-   Each Zone contains an array of ParkingArea pointers
    
-   Zone adjacency is implemented using a round-robin strategy in the AllocationEngine
    
-   Vehicles have preferred zones, and the system always attempts same-zone allocation first
    

Slot Representation:  
Class ParkingSlot contains:

-   slotId: Unique identifier
    
-   zoneId: Parent zone identifier
    
-   isAvailable: Availability status
    
-   vehicleId: ID of the currently parked vehicle
    

* * *

3.  ALLOCATION STRATEGY
    

Same-Zone Preference:

-   The system first checks the requested zone for available slots
    
-   Allocation uses a first-available slot strategy within the zone
    

Cross-Zone Allocation:

-   Triggered when the requested zone has no available slots
    
-   Uses round-robin traversal to find the next available zone
    
-   A penalty or extra cost can be applied for cross-zone allocation
    

Allocation Algorithm:

1.  Locate the requested zone
    
2.  If the zone has an available slot, allocate it
    
3.  Otherwise, search the next zone using round-robin
    
4.  If a slot is found, allocate with penalty
    
5.  If no slots are available, allocation fails
    

* * *

4.  REQUEST LIFECYCLE STATE MACHINE
    

Request States:

-   REQUESTED: Request created
    
-   ALLOCATED: Slot assigned
    
-   OCCUPIED: Vehicle parked
    
-   RELEASED: Vehicle departed
    
-   CANCELLED: Request cancelled
    

Valid State Transitions:

-   REQUESTED → ALLOCATED → OCCUPIED → RELEASED
    
-   REQUESTED → CANCELLED
    
-   ALLOCATED → CANCELLED
    

Invalid transitions are blocked through strict state validation logic.

* * *

5.  ROLLBACK DESIGN
    

Rollback Stack:

-   Implemented using a stack (LIFO)
    
-   Stores RollbackOperation objects
    
-   Each operation records:
    
    -   Operation type
        
    -   Request ID
        
    -   Slot ID
        
    -   Zone ID
        
    -   Previous state
        
-   Maximum rollback depth is configurable (default: 10 operations)
    

Operation Types:

-   ALLOCATION: Reverts slot assignment
    
-   CANCELLATION: Restores a cancelled request
    
-   STATE\_CHANGE: Reverts request to its previous state
    

Rollback Process:

1.  Pop the latest operation from the stack
    
2.  Identify the operation type
    
3.  Execute the corresponding undo logic
    
4.  Restore system state
    

* * *

6.  TIME AND SPACE COMPLEXITY
    

Time Complexity:

-   Slot Allocation: O(n), where n is the number of slots in a zone
    
-   Vehicle Search (BST): O(log n) average, O(n) worst case
    
-   Request Queue Operations: O(1)
    
-   Rollback Operation: O(1) per operation
    
-   Zone Utilization Calculation: O(z × a), where z is zones and a is areas
    

Space Complexity:  
Overall Space Usage: O(z + a + s + v + r)

Where:

-   z = number of zones
    
-   a = number of areas
    
-   s = number of slots
    
-   v = number of vehicles
    
-   r = number of requests
    

Rollback Stack Space: O(k), where k is the maximum rollback operations

* * *

7.  DATA STRUCTURE JUSTIFICATIONS
    

Arrays (Zones, Areas, Slots):

-   Fixed capacity matches real parking infrastructure
    
-   Fast random access
    
-   Simple memory management
    

Linked List (Requests):

-   Supports dynamic growth
    
-   Easy insertion and deletion
    
-   Maintains natural time-based ordering
    

Stack (Rollback):

-   LIFO structure is ideal for undo functionality
    
-   Efficient push and pop operations
    

Queue (Pending Requests):

-   FIFO ensures fair request processing
    
-   Handles request bursts efficiently
    
-   Decouples request input from processing
    

Binary Search Tree (Vehicles):

-   Efficient average search time
    
-   Maintains sorted order
    
-   Supports efficient insertion and deletion
    

* * *

8.  TESTING STRATEGY
    

Test Cases Implemented:

1.  Zone creation and structural validation
    
2.  Slot allocation correctness
    
3.  Cross-zone allocation handling
    
4.  Request lifecycle validation
    
5.  Cancellation and slot restoration
    
6.  Rollback mechanism verification
    
7.  Queue FIFO behavior
    
8.  BST search and insertion
    
9.  Invalid state transition detection
    
10.  Analytics correctness after rollback
     

Testing Approach:

-   Unit testing for individual components
    
-   Integration testing for complete workflows
    
-   Edge case and error condition testing
    

* * *

9.  KEY DESIGN PATTERNS
    

State Pattern:

-   ParkingRequest implements a state machine
    
-   Each state defines its valid transitions
    

Command Pattern:

-   Rollback operations encapsulate undo logic
    
-   Enables flexible undo functionality
    

Centralized Controller (Singleton-like):

-   ParkingSystem acts as the main controller
    
-   Coordinates all subsystems
    

* * *

10.  LIMITATIONS AND FUTURE ENHANCEMENTS
     

Current Limitations:

-   Fixed maximum capacities
    
-   Simplified zone adjacency (round-robin)
    
-   In-memory storage only
    

Future Enhancements:

-   Dynamic resizing of arrays
    
-   Graph-based zone connectivity
    
-   File-based persistence
    
-   Real-time pricing model
    
-   Multi-threaded request handling
    
-   Web or API interface
    
-   Predictive analytics and reporting
    

* * *

11.  COMPILATION AND EXECUTION
     

Compilation Command:  
g++ -o parking\_system \*.cpp

Files Required:  
Core: ParkingSlot, ParkingArea, Zone, Vehicle, ParkingRequest  
Engine: AllocationEngine, RequestManager, RollbackManager  
Structures: RequestQueue, VehicleBST  
System: ParkingSystem, TestSuite  
Main: main.cpp, design document

* * *

12.  CONCLUSION
     

The Smart Parking Allocation System successfully delivers:

-   Zone-based parking management
    
-   Stateful request lifecycle handling
    
-   Intelligent same-zone and cross-zone allocation
    
-   Robust rollback mechanism
    
-   Comprehensive analytics
    
-   Use of multiple data structures (Array, Linked List, Stack, Queue, BST)
    
-   More than 10 validated test cases
    

The modular and extensible design ensures maintainability and future scalability while meeting all project requirements.

* * *

FINAL COMPILATION COMMAND:  
g++ -o parking\_system main.cpp ParkingSlot.cpp ParkingArea.cpp Zone.cpp Vehicle.cpp ParkingRequest.cpp AllocationEngine.cpp RequestManager.cpp RollbackManager.cpp RequestQueue.cpp VehicleBST.cpp ParkingSystem.cpp TestSuite.cpp

RUN COMMAND:  
./parking\_system