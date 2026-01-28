from flask import Blueprint, render_template, request, redirect, url_for, flash, session
from .parking_system import ParkingSystem

bp = Blueprint('main', __name__)

# Global parking system instance
parking_system = ParkingSystem()

@bp.before_app_request
def check_auth():
    """Check if user is authenticated before each request"""
    # Exclude login and static files from auth check
    if request.endpoint in ['main.login', 'static']:
        return
    
    if not session.get('logged_in'):
        return redirect(url_for('main.login'))

@bp.route('/login', methods=['GET', 'POST'])
def login():
    """Login page"""
    if session.get('logged_in'):
        return redirect(url_for('main.index'))
        
    if request.method == 'POST':
        email = request.form.get('email')
        password = request.form.get('password')
        
        if email == 'hassanandshabih' and password == 'dsaproject':
            session['logged_in'] = True
            flash('Welcome to IntelliCity | Smart Parking', 'success')
            return redirect(url_for('main.index'))
        else:
            flash('Invalid credentials. Please try again.', 'error')
            
    return render_template('login.html')

@bp.route('/logout')
def logout():
    """Logout agent"""
    session.pop('logged_in', None)
    flash('Logged out successfully.', 'success')
    return redirect(url_for('main.login'))

@bp.route('/')
def index():
    """Main dashboard"""
    return render_template('index.html', system=parking_system)

@bp.route('/status')
def status():
    """Display system status"""
    return render_template('status.html', system=parking_system)

@bp.route('/register_vehicle', methods=['GET', 'POST'])
def register_vehicle():
    """Register a new vehicle"""
    if request.method == 'POST':
        vehicle_type = request.form.get('vehicle_type')
        license_plate = request.form.get('license_plate')
        owner_name = request.form.get('owner_name')
        preferred_zone = request.form.get('preferred_zone')

        if vehicle_type and license_plate and owner_name and preferred_zone:
            # 4-digit license plate validation
            import re
            if not re.match(r'^\d{4}$', license_plate):
                flash('License plate must be exactly 4 digits.', 'error')
            else:
                vehicle_id = parking_system.register_vehicle(vehicle_type, license_plate, owner_name, preferred_zone)
                if vehicle_id:
                    flash(f'Vehicle registered successfully with ID: {vehicle_id}', 'success')
                    return redirect(url_for('main.index'))
                else:
                    flash(f'Registration failed! License plate {license_plate} might already be registered.', 'error')
        else:
            flash('All fields are required', 'error')

    # Filter zones that have at least one slot available
    available_zones = [z for z in parking_system.zones if z.get_available_slots() > 0]
    return render_template('register_vehicle.html', zones=available_zones)

@bp.route('/create_request', methods=['GET', 'POST'])
def create_request():
    """Create a parking request"""
    if request.method == 'POST':
        vehicle_id = request.form.get('vehicle_id')
        zone_id = request.form.get('zone_id')

        if vehicle_id and zone_id:
            result = parking_system.create_parking_request(vehicle_id, zone_id)
            if result.startswith("Error"):
                flash(result, 'error')
            else:
                flash(f'Parking request created with ID: {result}', 'success')
                return redirect(url_for('main.index'))
        else:
            flash('All fields are required', 'error')

    # Filter zones that have at least one slot available
    available_zones = [z for z in parking_system.zones if z.get_available_slots() > 0]
    return render_template('create_request.html', system=parking_system, zones=available_zones)

@bp.route('/process_request')
def process_request():
    """Process next request in queue"""
    try:
        success, message = parking_system.process_next_request()
        if success:
            flash(message, 'success')
        else:
            flash(message, 'warning' if 'No requests' in message else 'error')
    except Exception as e:
        flash(str(e), 'error')

    return redirect(request.referrer or url_for('main.index'))

@bp.route('/pending_requests')
def pending_requests():
    """View pending requests"""
    return render_template('pending_requests.html', system=parking_system)

@bp.route('/allocate_slot/<request_id>')
def allocate_slot(request_id):
    """Allocate slot to request"""
    try:
        success, message = parking_system.allocate_slot_to_request(request_id)
        if success:
            flash(message, 'success')
        else:
            flash(message, 'error')
    except Exception as e:
        flash(str(e), 'error')

    return redirect(url_for('main.pending_requests'))

@bp.route('/mark_occupied/<request_id>')
def mark_occupied(request_id):
    """Mark request as occupied"""
    try:
        parking_system.mark_as_occupied(request_id)
        flash('Request marked as occupied', 'success')
    except Exception as e:
        flash(str(e), 'error')

    return redirect(url_for('main.pending_requests'))

@bp.route('/mark_released/<request_id>')
def mark_released(request_id):
    """Mark request as released"""
    try:
        parking_system.mark_as_released(request_id)
        flash('Request marked as released', 'success')
    except Exception as e:
        flash(str(e), 'error')

    return redirect(url_for('main.pending_requests'))

@bp.route('/cancel_request/<request_id>')
def cancel_request(request_id):
    """Cancel parking request"""
    try:
        parking_system.cancel_request(request_id)
        flash('Request cancelled', 'success')
    except Exception as e:
        flash(str(e), 'error')

    return redirect(url_for('main.pending_requests'))

@bp.route('/vehicles')
def vehicles():
    """View all vehicles"""
    return render_template('vehicles.html', system=parking_system)

@bp.route('/rollback/<int:k>')
def rollback(k):
    """Rollback last k operations"""
    try:
        parking_system.rollback_last_k_operations(k)
        flash(f'Rolled back {k} operations', 'success')
    except Exception as e:
        flash(str(e), 'error')

    return redirect(url_for('main.index'))

@bp.route('/zone_relationships')
def zone_relationships():
    """View zone relationships and adjacencies"""
    relationships = parking_system.get_zone_relationships()
    return render_template('zone_relationships.html', 
                          relationships=relationships,
                          system=parking_system)

@bp.route('/analytics')
def analytics():
    """System analytics"""
    data = parking_system.get_comprehensive_analytics()
    return render_template('analytics.html', 
                          system=parking_system,
                          data=data)

@bp.route('/run_tests')
def run_tests():
    """Run test suite"""
    # Reset system first to ensure clean state for tests
    parking_system.reset_system()
    
    from .test_suite import TestSuite
    test_suite = TestSuite(parking_system)
    results = test_suite.run_all_tests()
    return render_template('test_results.html', results=results)

@bp.route('/demo')
def demo():
    """Run demo scenario"""
    try:
        parking_system.run_demo_scenario()
        flash('Demo scenario completed. System populated with sample data.', 'success')
    except Exception as e:
        flash(f'Error running demo: {str(e)}', 'error')

    return redirect(url_for('main.index'))

@bp.route('/reset')
def reset_system():
    """Reset system to initial state"""
    try:
        parking_system.reset_system()
        flash('System reset successfully', 'success')
    except Exception as e:
        flash(f'Error resetting system: {str(e)}', 'error')
    
    return redirect(url_for('main.index'))