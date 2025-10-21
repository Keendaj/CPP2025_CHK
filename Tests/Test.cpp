#include "Test.hpp"
#include <cassert>
#include "../Calculator.hpp"

void Test::addCustomTest(testFunction func) {

    tests.push_back(func);
}

void Test::run() {
    int passCount = 0;
    int failCount = 0;

    for (size_t i = 0; i < tests.size(); ++i) {
        try {
            tests[i]();
            output << "Test " << i + 1 << ": \033[32mPASS\033[0m\n";
            ++passCount;
        } catch (const std::exception& e) {
            output << "Test " << i + 1 << ": \033[31mFAIL\033[0m - " << e.what() << "\n";
            ++failCount;
        } catch (...) {
            output << "Test " << i + 1 << ": \033[31mFAIL\033[0m - unokwn error\n";
            ++failCount;
        }
        output << "\n";
    }

    output << "\nSummary: " << passCount << " passed, " << failCount << " failed.\n";
}

void Test::addParserTests(){
    //Тест на обычную работу
    addCustomTest([]() 
    {
        Calculator calc;
        calc.calculate("2 + 3 * 4");
        std::string expected = "2 3 4 * + ";
        if (calc.getParsedString() != expected){
	        throw std::runtime_error("Неправильная RPN: ожидалось \"" + expected +
                                 "\", получили \"" + calc.getParsedString() + "\"");
        }
    });

    //Тест на функции
    addCustomTest([]()
    {
        Calculator calc;
        calc.calculate("sqrt(16) + cos(0)");
        std::string expected = "16 sqrt 0 cos + ";
        if (calc.getParsedString() != expected){
	        throw std::runtime_error("Неправильная RPN: ожидалось \"" + expected +
                                 "\", получили \"" + calc.getParsedString() + "\"");
        }
    });

    //Тест на унарный минус
    addCustomTest([]()
    {
        Calculator calc;
        calc.calculate("-3 + 5");
        std::string expected = "0 3 - 5 + ";
        if (calc.getParsedString() != expected){
	        throw std::runtime_error("Неправильная RPN: ожидалось \"" + expected +
                                 "\", получили \"" + calc.getParsedString() + "\"");
        }
    });

    //Тест на скобки
    addCustomTest([](){
        Calculator calc;
        calc.calculate("2 * (3 + (4 - 1))");
        std::string expected = "2 3 4 1 - + * ";
        if (calc.getParsedString() != expected){
	        throw std::runtime_error("Неправильная RPN: ожидалось \"" + expected +
                                 "\", получили \"" + calc.getParsedString() + "\"");
        }
    });

    //Тест на операцию из dll пока что ему плохо, т.к. парсер считает, что все не стандартные операции - унарные функции и приоритету плохо
    addCustomTest([](){
        Calculator calc;
        calc.calculate("2 @ 3 + 4");
        std::string expected = "2 3 @ 4 + "; 
        if (calc.getParsedString() != expected){
	        throw std::runtime_error("Неправильная RPN: ожидалось \"" + expected +
                                 "\", получили \"" + calc.getParsedString() + "\"");
        }
    });
}


void Test::addCalculationTests()
{
    //Тест на обычную работу
    addCustomTest([]() {
        Calculator calc;
        int result = calc.calculate("2 + 3 * 4");
        if (result != 14)
            throw std::runtime_error("Calculation failed: expected 14, got " + std::to_string(result));
    });

    //Тест на функции
    addCustomTest([]() {
        Calculator calc;
        int result = calc.calculate("sqrt(16) + cos(0)"); //Пока что заглушки, потому это 16 + 0
        if (result != 16)
            throw std::runtime_error("Calculation failed: expected 16, got " + std::to_string(result));
    });

    //Тест на унарный минус
    addCustomTest([]() {
        Calculator calc;
        int result = calc.calculate("-3 + 5");
        if (result != 2)
            throw std::runtime_error("Calculation failed: expected 2, got " + std::to_string(result));
    });

    //Тест на скобки
    addCustomTest([]() {
        Calculator calc;
        int result = calc.calculate("2 * (3 + 4)");
        if (result != 14)
            throw std::runtime_error("Calculation failed: expected 14, got " + std::to_string(result));
    });

    //Тест на операцию из dll пока что ему плохо, т.к. парсер считает, что все не стандартные операции - унарные функции и приоритету плохо
    addCustomTest([]() {
        Calculator calc;
        int result = calc.calculate("2 @ 3 + 4");
        if (result != 9)
            throw std::runtime_error("Calculation failed: expected 9, got " + std::to_string(result));
    });
}