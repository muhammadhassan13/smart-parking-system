import json
import os
import shutil

class DataManager:
    @staticmethod
    def save_to_file(data, filename="parking_data.json"):
        """Save data dictionary to JSON file."""
        try:
            # Create a backup if file exists
            if os.path.exists(filename):
                backup_name = filename + ".bak"
                try:
                    if os.path.exists(backup_name):
                        os.remove(backup_name)
                    shutil.copy2(filename, backup_name)
                except Exception as e:
                    print(f"Warning: Failed to create backup: {e}")

            with open(filename, 'w') as f:
                json.dump(data, f, indent=4)
            print(f"Data saved successfully to {filename}")
            return True
        except Exception as e:
            print(f"Error saving data: {e}")
            return False

    @staticmethod
    def load_from_file(filename="parking_data.json"):
        """Load data dictionary from JSON file."""
        if not os.path.exists(filename):
            print(f"No data file found at {filename}")
            return None
        
        try:
            with open(filename, 'r') as f:
                data = json.load(f)
            print(f"Data loaded successfully from {filename}")
            return data
        except Exception as e:
            print(f"Error loading data: {e}")
            return None
    
    @staticmethod
    def delete_data_file(filename="parking_data.json"):
        """Delete the data file."""
        try:
            if os.path.exists(filename):
                os.remove(filename)
                print(f"Deleted data file {filename}")
            
            backup_name = filename + ".bak"
            if os.path.exists(backup_name):
                os.remove(backup_name)
                print(f"Deleted backup file {backup_name}")
                
            return True
        except Exception as e:
            print(f"Error deleting data file: {e}")
            return False
