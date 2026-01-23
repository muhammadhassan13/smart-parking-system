#ifndef VEHICLEBST_H
#define VEHICLEBST_H

#include "Vehicle.h"
#include <string>
using namespace std;

class VehicleBST {
private:
    struct BSTNode {
        Vehicle* vehicle;
        BSTNode* left;
        BSTNode* right;
        
        BSTNode(Vehicle* v) : vehicle(v), left(nullptr), right(nullptr) {}
    };
    
    BSTNode* root;
    int nodeCount;
    
    // Helper methods
    BSTNode* insertRec(BSTNode* node, Vehicle* vehicle);
    Vehicle* searchRec(BSTNode* node, const string& vehicleId) const;
    void inorderRec(BSTNode* node) const;
    void clearRec(BSTNode* node);
    
public:
    VehicleBST();
    ~VehicleBST();
    
    // BST operations
    bool insert(Vehicle* vehicle);
    Vehicle* search(const string& vehicleId) const;
    void displayInorder() const;
    void clear();
    int getCount() const;
};

#endif