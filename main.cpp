#include "Tests/Test.hpp"
#include "Calculator.hpp"
#include <iostream>
#include <windows.h>

int main(){
    SetConsoleOutputCP(CP_UTF8);
    
    calculator::Calculator calc("Plugins");

    std::cout << "Калькулятор RPN" << std::endl;
    std::cout << "Вводите числа и операции (+, -, *, /, ^, sin, cos, sqrt...)" << std::endl;
    std::cout << "Введите 'help' для справки, 'exit' для выхода" << std::endl;

    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "exit") break;
        if (input == "help") {
            std::cout << "Помощь: числа добавляются в стек, операции выполняются над числами из стека" << std::endl;
            continue;
        }

        try {
            calc.calculate(input);
            calc.getParsedString();
        } catch (const std::exception& e) {
            
            std::cerr << e.what();
        }
    }

    return 0;
}