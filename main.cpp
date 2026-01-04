#include "unique_ptr.hpp"
#include "shared_ptr.hpp" // подключаем твой shared_ptr
#include <iostream>
#include <string>

// ===================== Test Classes =====================
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

// ===================== unique_ptr Tests =====================
void test_constructor() {
    std::cout << "\n=== Testing unique_ptr Constructors ===\n";
    
    unique_ptr<int> ptr1;
    std::cout << "ptr1 is " << (ptr1 ? "valid" : "null") << "\n";
    
    unique_ptr<int> ptr2(new int(42));
    std::cout << "ptr2 value: " << *ptr2 << "\n";
    
    unique_ptr<TestClass> ptr3(new TestClass(100, "object1"));
}

void test_move_semantics() {
    std::cout << "\n=== Testing unique_ptr Move Semantics ===\n";
    
    unique_ptr<int> ptr1(new int(10));
    std::cout << "ptr1 value: " << *ptr1 << "\n";
    
    unique_ptr<int> ptr2(std::move(ptr1));
    std::cout << "After move - ptr1 is " << (ptr1 ? "valid" : "null") << "\n";
    std::cout << "After move - ptr2 value: " << *ptr2 << "\n";
    
    unique_ptr<int> ptr3(new int(20));
    ptr3 = std::move(ptr2);
    std::cout << "After assignment - ptr2 is " << (ptr2 ? "valid" : "null") << "\n";
    std::cout << "After assignment - ptr3 value: " << *ptr3 << "\n";
}

void test_modifiers() {
    std::cout << "\n=== Testing unique_ptr Modifiers ===\n";
    
    unique_ptr<int> ptr1(new int(50));
    int* raw = ptr1.release();
    std::cout << "After release - ptr1 is " << (ptr1 ? "valid" : "null") << "\n";
    std::cout << "Raw pointer value: " << *raw << "\n";
    delete raw;
    
    unique_ptr<int> ptr2(new int(60));
    ptr2.reset(new int(70));
    std::cout << "After reset - ptr2 value: " << *ptr2 << "\n";
    
    unique_ptr<int> ptr3(new int(80));
    unique_ptr<int> ptr4(new int(90));
    std::cout << "Before swap - ptr3: " << *ptr3 << ", ptr4: " << *ptr4 << "\n";
    ptr3.swap(ptr4);
    std::cout << "After swap - ptr3: " << *ptr3 << ", ptr4: " << *ptr4 << "\n";
}

void test_observers() {
    std::cout << "\n=== Testing unique_ptr Observers ===\n";
    
    unique_ptr<TestClass> ptr(new TestClass(123, "observer_test"));
    TestClass* raw = ptr.get();
    std::cout << "Raw pointer via get(): " << raw << "\n";
    std::cout << "ptr is " << (ptr ? "valid" : "null") << "\n";
    (*ptr).display();
    ptr->display();
}

void test_comparisons() {
    std::cout << "\n=== Testing unique_ptr Comparison Operators ===\n";
    
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
    std::cout << "\n=== Testing unique_ptr Self-Assignment ===\n";
    
    unique_ptr<int> ptr(new int(42));
    std::cout << "Before self-assignment: " << *ptr << "\n";
    ptr = std::move(ptr);
    std::cout << "After self-assignment: " << (ptr ? "still valid" : "null") << "\n";
    if (ptr) std::cout << "Value: " << *ptr << "\n";
}

void test_memory_cleanup() {
    std::cout << "\n=== Testing unique_ptr Memory Cleanup ===\n";
    
    {
        unique_ptr<TestClass> ptr1(new TestClass(1, "scoped1"));
        unique_ptr<TestClass> ptr2(new TestClass(2, "scoped2"));
        std::cout << "Objects created in scope\n";
    }
    std::cout << "Scope ended - objects should be destructed\n";
}

// ===================== shared_ptr Tests =====================
void test_shared_ptr_basic()
{
    std::cout << "\n=== Testing shared_ptr Basic ===\n";

    shared_ptr<int> sp1(new int(42));
    std::cout << "sp1 use_count: " << sp1.use_count() << "\n";

    // Copy constructor
    shared_ptr<int> sp2(sp1);
    std::cout << "After copy, sp1 use_count: " << sp1.use_count() 
              << ", sp2 use_count: " << sp2.use_count() << "\n";

    // Copy assignment
    shared_ptr<int> sp3;
    sp3 = sp1;
    std::cout << "After copy assignment, sp1 use_count: " << sp1.use_count() 
              << ", sp3 use_count: " << sp3.use_count() << "\n";

    // Move constructor
    shared_ptr<int> sp4(std::move(sp3));
    std::cout << "After move constructor, sp3 is " << (sp3 ? "valid" : "null") 
              << ", sp4 use_count: " << sp4.use_count() << "\n";

    // Move assignment
    shared_ptr<int> sp5;
    sp5 = std::move(sp4);
    std::cout << "After move assignment, sp4 is " << (sp4 ? "valid" : "null") 
              << ", sp5 use_count: " << sp5.use_count() << "\n";

    // Reset
    sp1.reset();
    std::cout << "After sp1.reset(), sp1 is " << (sp1 ? "valid" : "null") 
              << ", sp2 use_count: " << sp2.use_count() << "\n";

    // Dereference
    if(sp2)
        std::cout << "Dereference sp2: " << *sp2 << "\n";
}

void test_shared_ptr_self_assignment()
{
    std::cout << "\n=== Testing shared_ptr Self-Assignment ===\n";

    shared_ptr<int> sp(new int(99));
    std::cout << "Before self-assignment, sp use_count: " << sp.use_count() << "\n";
    sp = sp;
    std::cout << "After self-assignment, sp use_count: " << sp.use_count() << "\n";
}

// ===================== main =====================
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

        std::cout << "\nStarting shared_ptr tests...\n";

        test_shared_ptr_basic();
        test_shared_ptr_self_assignment();
        
        std::cout << "\n=== All tests completed successfully! ===\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
