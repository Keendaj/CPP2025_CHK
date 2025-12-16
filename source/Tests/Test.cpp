#include "Test.h"
#include "../wrapper.h"
#include "../engine.h"
#include "../TestSubjects/Subject1.h"
#include <iostream>

void addWrappers(Engine& engine)
{
    std::shared_ptr<Subject1> subject1 = std::make_shared<Subject1>();

    auto wrapper1 = makeWrapper(subject1, &Subject1::doSomething, { {"a", 0.0}, {"b", ""} });
    engine.registerCommand("doSomething", wrapper1);

    auto wrapper2 = makeWrapper(subject1, &Subject1::calculate, { {"x", 0}, {"y", 0}, {"z", 0} });
    engine.registerCommand("calculate", wrapper2);

    auto wrapper3 = makeWrapper(subject1, &Subject1::printMessage, { {"msg", ""} });
    engine.registerCommand("printMessage", wrapper3);

    auto wrapper4 = makeWrapper(subject1, &Subject1::divide, { {"a", 1.0}, {"b", 1.0} });
    engine.registerCommand("divide", wrapper4);
}

void testBasicOperations(Engine& engine) {
    std::cout << "Basic Operations Test" << std::endl;
    bool passed = true;
    
    std::cout << "\nChecking command registration:" << std::endl;
    if (!engine.hasCommand("doSomething")) {
        std::cout << "Command 'doSomething' not found" << std::endl;
        passed = false;
    } else {
        std::cout << "doSomething - registered" << std::endl;
    }
    
    if (!engine.hasCommand("calculate")) {
        std::cout << "Command 'calculate' not found" << std::endl;
        passed = false;
    } else {
        std::cout << "calculate - registered" << std::endl;
    }
    
    if (!engine.hasCommand("printMessage")) {
        std::cout << "Command 'printMessage' not found" << std::endl;
        passed = false;
    } else {
        std::cout << "printMessage - registered" << std::endl;
    }
    
    if (!engine.hasCommand("divide")) {
        std::cout << "Command 'divide' not found" << std::endl;
        passed = false;
    } else {
        std::cout << "divide - registered" << std::endl;
    }
    
    std::cout << "\nExecuting void commands:" << std::endl;
    try {
        std::cout << "  printMessage: ";
        engine.execute("printMessage", {{"msg", "Test message"}});
        std::cout << "executed successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "error: " << e.what() << std::endl;
        passed = false;
    }

    std::cout << "\nExecuting returning commands:" << std::endl;
    try {
        std::any result = engine.execute("calculate", {{"x", 10}, {"y", 20}, {"z", 30}});
        int sum = std::any_cast<int>(result);
        std::cout << "  calculate(10,20,30) = " << sum << std::endl;
        
        if (sum != 60) {
            std::cout << "Expected 60, got " << sum << std::endl;
            passed = false;
        } else {
            std::cout << "  Result correct" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "  calculate: error: " << e.what() << std::endl;
        passed = false;
    }
    
    if (passed) {
        std::cout << "\nAll basic operations work correctly" << std::endl;
    } else {
        std::cout << "\nBasic operations test failed" << std::endl;
    }
}

void testArguments(Engine& engine) {
    std::cout << "\nArgument Handling Test" << std::endl;
    bool passed = true;
    
    std::cout << "\nTesting default values:" << std::endl;
    try {
        std::any result = engine.execute("calculate", {});
        int sum = std::any_cast<int>(result);
        std::cout << "  calculate() = " << sum << " (all defaults)" << std::endl;
        
        if (sum != 0) {
            std::cout << "Expected 0, got " << sum << std::endl;
            passed = false;
        } else {
            std::cout << " Default values work" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "  error: " << e.what() << std::endl;
        passed = false;
    }
    
    std::cout << "\nTesting partial override:" << std::endl;
    try {
        std::any result = engine.execute("calculate", {{"x", 5}, {"z", 3}});
        int sum = std::any_cast<int>(result);
        std::cout << "  calculate(x=5, z=3) = " << sum << std::endl;
        
        if (sum != 8) {
            std::cout << "Expected 8, got " << sum << std::endl;
            passed = false;
        } else {
            std::cout << "Partial override works" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "  error: " << e.what() << std::endl;
        passed = false;
    }
    
    std::cout << "\nTesting full override:" << std::endl;
    try {
        std::any result = engine.execute("divide", {{"a", 15.0}, {"b", 3.0}});
        double quotient = std::any_cast<double>(result);
        std::cout << "  divide(15.0, 3.0) = " << quotient << std::endl;
        
        if (quotient != 5.0) {
            std::cout << "Expected 5.0, got " << quotient << std::endl;
            passed = false;
        } else {
            std::cout << " Full override works" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "  error: " << e.what() << std::endl;
        passed = false;
    }
    
    std::cout << "\nChecking return types:" << std::endl;
    try {
        if (engine.getReturnType("printMessage") == typeid(void)) {
            std::cout << "  printMessage returns void" << std::endl;
        } else {
            std::cout << "printMessage should return void" << std::endl;
            passed = false;
        }
        
        if (engine.getReturnType("calculate") == typeid(int)) {
            std::cout << " calculate returns int" << std::endl;
        } else {
            std::cout << " calculate should return int" << std::endl;
            passed = false;
        }
        
        if (engine.getReturnType("divide") == typeid(double)) {
            std::cout << " divide returns double" << std::endl;
        } else {
            std::cout << "divide should return double" << std::endl;
            passed = false;
        }
    } catch (const std::exception& e) {
        std::cout << "  error checking return types: " << e.what() << std::endl;
        passed = false;
    }
    
    if (passed) {
        std::cout << "\nArgument handling works correctly" << std::endl;
    } else {
        std::cout << "\nArgument handling test failed" << std::endl;
    }
}

void testErrorHandling(Engine& engine) {
    std::cout << "\n Error Handling Test " << std::endl;
    bool passed = true;

    std::cout << "\nTesting non-existent command:" << std::endl;
    try {
        engine.execute("Command", {});
        std::cout << "Expected exception for non-existent command" << std::endl;
        passed = false;
    } catch (const std::runtime_error& e) {
        std::cout << "  Exception caught: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unexpected exception type" << std::endl;
        passed = false;
    }
    
    std::cout << "\nTesting wrong argument type:" << std::endl;
    try {
        engine.execute("calculate", {{"x", "string instead of int"}});
        std::cout << "Expected exception for wrong argument type" << std::endl;
        passed = false;
    } catch (const std::bad_any_cast& e) {
        std::cout << "  bad_any_cast caught: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  Exception caught: " << e.what() << std::endl;
    } catch (...) {
        std::cout << " Unexpected exception type" << std::endl;
        passed = false;
    }
    
    std::cout << "\nTesting multiple calls:" << std::endl;
    try {
        std::any result1 = engine.execute("calculate", {{"x", 1}, {"y", 2}, {"z", 3}});
        int sum1 = std::any_cast<int>(result1);
        std::cout << "  First call: calculate(1,2,3) = " << sum1 << std::endl;
        
        std::any result2 = engine.execute("calculate", {{"x", 100}, {"y", 200}});
        int sum2 = std::any_cast<int>(result2);
        std::cout << "  Second call: calculate(100,200) = " << sum2 << std::endl;
        
        std::any result3 = engine.execute("calculate", {{"z", 5}});
        int sum3 = std::any_cast<int>(result3);
        std::cout << "  Third call: calculate(z=5) = " << sum3 << std::endl;
        
        if (sum1 == 6 && sum2 == 300 && sum3 == 5) {
            std::cout << "  Multiple calls work correctly" << std::endl;
        } else {
            std::cout << "Multiple calls gave wrong results" << std::endl;
            passed = false;
        }
    } catch (const std::exception& e) {
        std::cout << "  error: " << e.what() << std::endl;
        passed = false;
    }
    
    if (passed) {
        std::cout << "\nError handling works correctly" << std::endl;
    } else {
        std::cout << "\nError handling test failed" << std::endl;
    }
}

void testCommandManagement() {
    std::cout << "\n Command Management Test " << std::endl;
    bool passed = true;
    Engine engine;
    std::shared_ptr<Subject1> subject1 = std::make_shared<Subject1>();

    std::cout << "\nTesting command registration:" << std::endl;
    try {
        auto wrapper = makeWrapper(subject1, &Subject1::calculate, {{"x", 0}, {"y", 0}, {"z", 0}});
        engine.registerCommand("testCommand", wrapper);
        
        if (!engine.hasCommand("testCommand")) {
            std::cout << "Command not registered" << std::endl;
            passed = false;
        } else {
            std::cout << " Command registered successfully" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Registration error: " << e.what() << std::endl;
        passed = false;
    }
    
    if (engine.hasCommand("testCommand")) {
        std::cout << "\nTesting registered command:" << std::endl;
        try {
            std::any result = engine.execute("testCommand", {{"x", 1}, {"y", 2}, {"z", 3}});
            int sum = std::any_cast<int>(result);
            std::cout << "  testCommand(1,2,3) = " << sum << std::endl;
            
            if (sum != 6) {
                std::cout << "Expected 6, got " << sum << std::endl;
                passed = false;
            } else {
                std::cout << "  Command works correctly" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Execution error: " << e.what() << std::endl;
            passed = false;
        }
    }
    
    std::cout << "\nTesting command unregistration:" << std::endl;
    if (engine.unregister("testCommand")) {
        std::cout << "  Command unregistered" << std::endl;

        if (engine.hasCommand("testCommand")) {
            std::cout << "Command still exists after unregistration" << std::endl;
            passed = false;
        } else {
            std::cout << "  Command properly removed" << std::endl;
        }
    } else {
        std::cout << "Failed to unregister command" << std::endl;
        passed = false;
    }
    
    std::cout << "\nTesting clear() method:" << std::endl;
    
    auto wrapper2 = makeWrapper(subject1, &Subject1::printMessage, {{"msg", ""}});
    engine.registerCommand("tempCommand", std::move(wrapper2));

    engine.clear();
    
    try {
        engine.execute("tempCommand", {{"msg", "test"}});
        std::cout << "Command still works after clear()" << std::endl;
        passed = false;
    } catch (const std::runtime_error&) {
        std::cout << "  Engine cleared, commands don't work" << std::endl;
    }
    
    if (passed) {
        std::cout << "\nCommand management works correctly" << std::endl;
    } else {
        std::cout << "\nCommand management test failed" << std::endl;
    }
}

void runTests() {
    std::cout << "Engine tests\n" << std::endl;
    
    try {
        Engine engine;
        addWrappers(engine);
        
        testBasicOperations(engine);
        testArguments(engine);
        testErrorHandling(engine);
        testCommandManagement();
        std::cout << "ALL TESTS COMPLETED" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "\nERROR: " << e.what() << std::endl;
        throw;
    } catch (...) {
        std::cerr << "\nERROR: Unknown exception" << std::endl;
        throw;
    }
}