# Smart Parking Allocation System - Flask Web Application

## Overview
This is a Flask web application port of the Smart Parking Allocation System. It provides a web interface for managing parking zones, vehicles, and requests with all the original functionality including:
- Zone-based parking management
- Binary Search Tree for vehicle management
- Queue-based request processing
- Cross-zone allocation
- Rollback functionality
- Comprehensive analytics
- Test suite

## Project Structure
smart_parking_system/
├── app/ # Flask application package
│   ├── __init__.py # Flask app factory
│   ├── routes.py # Web routes and views
│   ├── parking_system.py # Main parking system class
│   ├── allocation_engine.py # Allocation engine logic
│   ├── parking_request.py # Parking request management
│   ├── parking_slot.py # Individual parking slots
│   ├── parking_area.py # Parking area management
│   ├── zone.py # Zone management
│   ├── vehicle.py # Vehicle class
│   ├── vehicle_bst.py # Binary Search Tree for vehicles
│   ├── request_manager.py # Request manager
│   ├── request_queue.py # Request queue
│   ├── rollback_manager.py # Rollback functionality
│   └── test_suite.py # Test suite
├── templates/ # HTML templates
│   ├── base.html # Base template
│   ├── index.html # Dashboard
│   ├── register_vehicle.html # Vehicle registration
│   ├── create_request.html # Request creation
│   ├── pending_requests.html # Request management
│   ├── vehicles.html # Vehicle list
│   ├── analytics.html # System analytics
│   ├── status.html # System status
│   └── test_results.html # Test results
├── static/ # Static files (CSS, JS, images)
│   └── style.css # Custom styles
├── main.py # Flask application entry point
├── requirements.txt # Python dependencies
└── README.md # This file


## Features
1. **Web Interface**: Modern Bootstrap-based UI for all operations
2. **Multi-Zone Management**: Organize parking into zones, areas, and slots
3. **Vehicle BST**: Efficient vehicle lookup using Binary Search Tree
4. **Request Queue**: Process parking requests in FIFO order
5. **Cross-Zone Allocation**: Automatically allocate slots in other zones when preferred zone is full
6. **State Management**: Complete parking request lifecycle (REQUESTED → ALLOCATED → OCCUPIED → RELEASED)
7. **Rollback System**: Undo operations with configurable depth
8. **Analytics**: Comprehensive system analytics and reporting
9. **Test Suite**: 10 comprehensive test cases

## How to Run

### Prerequisites
- Python 3.7+
- pip package manager

### Installation
1. Clone or download the project
2. Navigate to the project directory
3. Install dependencies:
```bash
pip install -r requirements.txt
```

### Running the Application
```bash
python main.py
```

The application will start on `http://localhost:8000`

### Login Credentials
- **Username:** `hassanandshabih`
- **Password:** `dsaproject`

### Web Interface Features
- **Dashboard**: Overview of system status and quick actions
- **Vehicle Registration**: Register new vehicles with type and details
- **Request Management**: Create parking requests and manage their lifecycle
- **System Status**: View current system state and zone information
- **Analytics**: Comprehensive system analytics and reports
- **Test Suite**: Run automated tests to verify functionality

## API Endpoints
- `/` - Dashboard
- `/status` - System status
- `/register_vehicle` - Vehicle registration form
- `/create_request` - Create parking request form
- `/pending_requests` - View and manage requests
- `/vehicles` - View all registered vehicles
- `/analytics` - System analytics
- `/run_tests` - Execute test suite
cd smart_parking_system

# Run the main application
python main.py

Usage Instructions
Main Menu Options:
Display System Status: View current system state

Register New Vehicle: Add a new vehicle to the system

Create Parking Request: Create a parking request for a vehicle

Process Next Request (Queue): Process the next request from the queue

View Pending Requests: Show all pending requests in the queue

Allocate Slot to Request: Manually allocate a slot to a specific request

Mark Request as Occupied: Mark a request as occupied (vehicle parked)

Mark Request as Released: Mark a request as released (vehicle left)

Cancel Parking Request: Cancel an existing request

View All Vehicles (BST): Display all vehicles in the BST

Rollback Last Operation: Undo the last operation

Rollback Last K Operations: Undo multiple operations

System Analytics: View detailed analytics

Run Test Suite (10 Tests): Run all test cases

Run Auto Demo Scenario: Run a comprehensive demo

Exit: Exit the application

Default Setup
The system initializes with:

3 Zones: Downtown (Z1), Uptown (Z2), Midtown (Z3)

5 Areas: A1, A2, B1, C1

10 Slots: Distributed across zones and areas

State Transitions
Parking requests follow this lifecycle:

REQUESTED: Request created and added to queue

ALLOCATED: Slot allocated to the request

OCCUPIED: Vehicle parked in the allocated slot

RELEASED: Vehicle left, slot freed

CANCELLED: Request cancelled (can happen at REQUESTED or ALLOCATED states)

Rollback System
The system maintains a stack of operations that can be rolled back:

Supports rollback of last operation

Supports rollback of last K operations

Maximum rollback depth: 10 operations (configurable)

Future Enhancements
GUI Interface: Convert to PyQt5 or Tkinter

Database Integration: Store data in SQLite or PostgreSQL

Web Interface: Create a Flask/Django web application

Real-time Updates: Add WebSocket support for real-time updates

Payment Integration: Add payment processing

Mobile App: Create companion mobile application

Machine Learning: Predictive analytics for peak usage

text

## How to Run in VS Code:

1. Create a new folder called `smart_parking_system`
2. Copy all the above Python files into this folder
3. Open the folder in VS Code
4. Click the "Run" button (green triangle) in the top-right corner or press F5
5. The application will start with a console interface

## Key Differences from C++:

1. **Memory Management**: Python handles memory automatically (no manual `new`/`delete`)
2. **Data Structures**: Lists instead of arrays, references instead of pointers
3. **Input/Output**: Python's `print()` and `input()` instead of `cout`/`cin`
4. **Time Handling**: `datetime` module instead of C++ `<ctime>`
5. **Enums**: Python's `Enum` class for state management
6. **Exception Handling**: Python try/except instead of error codes

## Features Maintained:

- ✅ All original C++ functionality
- ✅ Zone-based parking system
- ✅ Binary Search Tree for vehicles
- ✅ Queue-based request processing
- ✅ Cross-zone allocation
- ✅ Rollback system with stack
- ✅ State machine for requests
- ✅ Comprehensive analytics
- ✅ Test suite with 10 tests
- ✅ Auto-demo scenario
- ✅ Menu-driven interface