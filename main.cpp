#include "Tests/Test.hpp"
#include <iostream>
#include <windows.h>

int main(){
    SetConsoleOutputCP(CP_UTF8);
    
    Test test(std::cout);
    test.addParserTests();
    test.addCalculationTests();
    test.run();

    return 0;
}