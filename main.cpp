#include "Tests/Test.hpp"
#include "Calculator.hpp"
#include <iostream>
#include <windows.h>


void startCalculations() {
    calculator::Calculator* calc = nullptr;

    try {
        calc = new calculator::Calculator("plugins");
    } catch (const std::exception& e) {
        std::cerr << "Ошибка инициализации калькулятора: " << e.what() << std::endl;
        return;
    }

    std::cout << "Калькулятор RPN" << std::endl;
    std::cout << "Вводите числа и операции (+, -, *, / ...)" << std::endl;
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
            std::cout << "Результат вычисления: "<< calc->calculate(input) << std::endl;
        } catch (const std::exception& e) {
            
            std::cerr << e.what();
            std::cout << std::endl;
        }
    }

    delete calc;
}


void runTests() {
    Test t(std::cout);

    t.addDllLoaderTests();
    t.addParserTests();
    t.addCalculationTests();

    t.run();
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);

    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--test" || arg == "-t") {
            runTests();
            return 0;
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Использование: " << argv[0] << " [--test|-t] [--help|-h]" << std::endl;
            std::cout << "Без аргументов: запускается интерактивный режим калькулятора" << std::endl;
            std::cout << "--test, -t: запуск всех тестов" << std::endl;
            std::cout << "--help, -h: вывод этой справки" << std::endl;
            return 0;
        } else {
            std::cerr << "Неизвестный аргумент: " << arg << std::endl;
            return 1;
        }
    }

    startCalculations();
    return 0;
}