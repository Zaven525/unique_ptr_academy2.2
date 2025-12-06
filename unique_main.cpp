#include "unique_ptr.hpp"
#include <iostream>
#include <string>

class TestClass {
public:
    int value;
    std::string name;
    
    TestClass(int v = 0, std::string n = "default") : value(v), name(n) {
        std::cout << "TestClass constructed: " << name << " (value: " << value << ")\n";
    }
    
    ~TestClass() {
        std::cout << "TestClass destructed: " << name << "\n";
    }
    
    void display() const {
        std::cout << "TestClass: " << name << ", value: " << value << "\n";
    }
};

void test_constructor() {
    std::cout << "\n=== Testing Constructors ===\n";
    
    // Default constructor
    unique_ptr<int> ptr1;
    std::cout << "ptr1 is " << (ptr1 ? "valid" : "null") << "\n";
    
    // Constructor with raw pointer
    unique_ptr<int> ptr2(new int(42));
    std::cout << "ptr2 value: " << *ptr2 << "\n";
    
    // Constructor with custom class
    unique_ptr<TestClass> ptr3(new TestClass(100, "object1"));
}

void test_move_semantics() {
    std::cout << "\n=== Testing Move Semantics ===\n";
    
    unique_ptr<int> ptr1(new int(10));
    std::cout << "ptr1 value: " << *ptr1 << "\n";
    
    // Move constructor
    unique_ptr<int> ptr2(std::move(ptr1));
    std::cout << "After move - ptr1 is " << (ptr1 ? "valid" : "null") << "\n";
    std::cout << "After move - ptr2 value: " << *ptr2 << "\n";
    
    // Move assignment
    unique_ptr<int> ptr3(new int(20));
    ptr3 = std::move(ptr2);
    std::cout << "After assignment - ptr2 is " << (ptr2 ? "valid" : "null") << "\n";
    std::cout << "After assignment - ptr3 value: " << *ptr3 << "\n";
}

void test_modifiers() {
    std::cout << "\n=== Testing Modifiers ===\n";
    
    unique_ptr<int> ptr1(new int(50));
    
    // Test release
    int* raw = ptr1.release();
    std::cout << "After release - ptr1 is " << (ptr1 ? "valid" : "null") << "\n";
    std::cout << "Raw pointer value: " << *raw << "\n";
    delete raw; // Must manually delete after release
    
    // Test reset
    unique_ptr<int> ptr2(new int(60));
    ptr2.reset(new int(70));
    std::cout << "After reset - ptr2 value: " << *ptr2 << "\n";
    
    // Test swap
    unique_ptr<int> ptr3(new int(80));
    unique_ptr<int> ptr4(new int(90));
    std::cout << "Before swap - ptr3: " << *ptr3 << ", ptr4: " << *ptr4 << "\n";
    ptr3.swap(&ptr4);
    std::cout << "After swap - ptr3: " << *ptr3 << ", ptr4: " << *ptr4 << "\n";
}

void test_observers() {
    std::cout << "\n=== Testing Observers ===\n";
    
    unique_ptr<TestClass> ptr(new TestClass(123, "observer_test"));
    
    // Test get()
    TestClass* raw = ptr.get();
    std::cout << "Raw pointer via get(): " << raw << "\n";
    
    // Test operator bool
    std::cout << "ptr is " << (ptr ? "valid" : "null") << "\n";
    
    // Test dereference operator
    (*ptr).display();
    
    // Test arrow operator
    ptr->display();
}

void test_comparisons() {
    std::cout << "\n=== Testing Comparison Operators ===\n";
    
    unique_ptr<int> ptr1(new int(10));
    unique_ptr<int> ptr2(new int(20));
    unique_ptr<int> ptr3;
    
    std::cout << "ptr1 == ptr2: " << (ptr1 == ptr2) << "\n";
    std::cout << "ptr1 != ptr2: " << (ptr1 != ptr2) << "\n";
    std::cout << "ptr1 < ptr2: " << (ptr1 < ptr2) << "\n";
    std::cout << "ptr1 > ptr2: " << (ptr1 > ptr2) << "\n";
    std::cout << "ptr3 == nullptr (empty): " << (!ptr3) << "\n";
}

void test_self_assignment() {
    std::cout << "\n=== Testing Self-Assignment Protection ===\n";
    
    unique_ptr<int> ptr(new int(42));
    std::cout << "Before self-assignment: " << *ptr << "\n";
    ptr = std::move(ptr);
    std::cout << "After self-assignment: " << (ptr ? "still valid" : "null") << "\n";
    if (ptr) std::cout << "Value: " << *ptr << "\n";
}

void test_memory_cleanup() {
    std::cout << "\n=== Testing Memory Cleanup ===\n";
    
    {
        unique_ptr<TestClass> ptr1(new TestClass(1, "scoped1"));
        unique_ptr<TestClass> ptr2(new TestClass(2, "scoped2"));
        std::cout << "Objects created in scope\n";
    }
    std::cout << "Scope ended - objects should be destructed\n";
}

int main() {
    std::cout << "Starting unique_ptr tests...\n";
    
    try {
        test_constructor();
        test_move_semantics();
        test_modifiers();
        test_observers();
        test_comparisons();
        test_self_assignment();
        test_memory_cleanup();
        
        std::cout << "\n=== All tests completed successfully! ===\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}