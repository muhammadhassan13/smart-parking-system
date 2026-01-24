#ifndef TESTSUITE_H
#define TESTSUITE_H

#include "ParkingSystem.h"

class TestSuite {
private:
    ParkingSystem* system;
    int testsPassed;
    int totalTests;
    
public:
    TestSuite();
    ~TestSuite();
    
    void runAllTests();
    
private:
    // Individual test cases
    void test1_ZoneCreation();
    void test2_SlotAllocation();
    void test3_CrossZoneAllocation();
    void test4_RequestLifecycle();
    void test5_Cancellation();
    void test6_Rollback();
    void test7_QueueOperations();
    void test8_BSTOperations();
    void test9_InvalidTransitions();
    void test10_AnalyticsAfterRollback();
    
    // Helper
    void printTestResult(const string& testName, bool passed);
};

#endif