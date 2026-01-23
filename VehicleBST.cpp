#include "VehicleBST.h"
#include <iostream>
using namespace std;

VehicleBST::VehicleBST() : root(nullptr), nodeCount(0) {}

VehicleBST::~VehicleBST() {
    clear();
}

void VehicleBST::clearRec(BSTNode* node) {
    if (node == nullptr) return;
    
    clearRec(node->left);
    clearRec(node->right);
    // Note: Vehicle objects are managed elsewhere
    delete node;
}

VehicleBST::BSTNode* VehicleBST::insertRec(BSTNode* node, Vehicle* vehicle) {
    if (node == nullptr) {
        nodeCount++;
        return new BSTNode(vehicle);
    }
    
    if (vehicle->getVehicleId() < node->vehicle->getVehicleId()) {
        node->left = insertRec(node->left, vehicle);
    } else if (vehicle->getVehicleId() > node->vehicle->getVehicleId()) {
        node->right = insertRec(node->right, vehicle);
    }
    // If equal, do nothing (vehicle IDs should be unique)
    
    return node;
}

bool VehicleBST::insert(Vehicle* vehicle) {
    if (vehicle == nullptr) {
        return false;
    }
    
    root = insertRec(root, vehicle);
    return true;
}

Vehicle* VehicleBST::searchRec(BSTNode* node, const string& vehicleId) const {
    if (node == nullptr) {
        return nullptr;
    }
    
    if (vehicleId == node->vehicle->getVehicleId()) {
        return node->vehicle;
    } else if (vehicleId < node->vehicle->getVehicleId()) {
        return searchRec(node->left, vehicleId);
    } else {
        return searchRec(node->right, vehicleId);
    }
}

Vehicle* VehicleBST::search(const string& vehicleId) const {
    return searchRec(root, vehicleId);
}

void VehicleBST::inorderRec(BSTNode* node) const {
    if (node == nullptr) return;
    
    inorderRec(node->left);
    node->vehicle->displayVehicleInfo();
    inorderRec(node->right);
}

void VehicleBST::displayInorder() const {
    cout << "\n=== VEHICLES (BST Inorder Traversal) ===" << endl;
    cout << "Total Vehicles: " << nodeCount << endl;
    
    if (root == nullptr) {
        cout << "No vehicles in BST." << endl;
        return;
    }
    
    inorderRec(root);
}

void VehicleBST::clear() {
    clearRec(root);
    root = nullptr;
    nodeCount = 0;
}

int VehicleBST::getCount() const {
    return nodeCount;
}