#include "Tests/Test.hpp"
#include <iostream>
#include <windows.h>

int main(){
    SetConsoleOutputCP(CP_UTF8);
    
    Test t(std::cout);
    t.addDllLoaderTests();
    t.run();

    return 0;
}