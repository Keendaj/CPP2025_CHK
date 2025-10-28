#include "Test.hpp"
#include "../Calculator.hpp"
#include "../DllLoader.hpp"

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
        std::cout << "\033[33m[INFO]\033[0m Тест на обычную работу (парсинг 2 + 3 * 4)\n";
        calculator::Calculator calc;
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
        std::cout << "\033[33m[INFO]\033[0m Тест на функции (парсинг sqrt(16) + cos(0))\n";
        calculator::Calculator calc;
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
        std::cout << "\033[33m[INFO]\033[0m Тест на унарный минус (-3 + 5)\n";
        calculator::Calculator calc;
        calc.calculate("-3 + 5");
        std::string expected = "0 3 - 5 + ";
        if (calc.getParsedString() != expected){
	        throw std::runtime_error("Неправильная RPN: ожидалось \"" + expected +
                                 "\", получили \"" + calc.getParsedString() + "\"");
        }
    });

    //Тест на скобки
    addCustomTest([](){
        std::cout << "\033[33m[INFO]\033[0m Тест на скобки (2 * (3 + (4 - 1)))\n";
        calculator::Calculator calc;
        calc.calculate("2 * (3 + (4 - 1))");
        std::string expected = "2 3 4 1 - + * ";
        if (calc.getParsedString() != expected){
	        throw std::runtime_error("Неправильная RPN: ожидалось \"" + expected +
                                 "\", получили \"" + calc.getParsedString() + "\"");
        }
    });

    //Тест на операцию из dll
    addCustomTest([](){
        std::cout << "\033[33m[INFO]\033[0m Тест на нестандартную операцию из DLL (2 ^ 3 + 4)\n";
        calculator::Calculator calc;
        calc.calculate("2 ^ 3 + 4");
        std::string expected = "2 3 ^ 4 + "; 
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
        std::cout << "\033[33m[INFO]\033[0m Тест на обычную работу (2 + 3 * 4)\n";
        calculator::Calculator calc;
        int result = calc.calculate("2 + 3 * 4");
        if (result != 14)
            throw std::runtime_error("Calculation failed: expected 14, got " + std::to_string(result));
    });

    //Тест на функции
    addCustomTest([]() {
        std::cout << "\033[33m[INFO]\033[0m Тест на функции (sqrt(16) + cos(0))\n";
        calculator::Calculator calc;
        int result = calc.calculate("sqrt(16) + cos(0)");
        if (result != 5)
            throw std::runtime_error("Calculation failed: expected 5, got " + std::to_string(result));
    });

    //Тест на унарный минус
    addCustomTest([]() {
        std::cout << "\033[33m[INFO]\033[0m Тест на унарный минус (-3 + 5)\n";
        calculator::Calculator calc;
        int result = calc.calculate("-3 + 5");
        if (result != 2)
            throw std::runtime_error("Calculation failed: expected 2, got " + std::to_string(result));
    });

    //Тест на скобки
    addCustomTest([]() {
        std::cout << "\033[33m[INFO]\033[0m Тест на скобки (2 * (3 + 4))\n";
        calculator::Calculator calc;
        int result = calc.calculate("2 * (3 + 4)");
        if (result != 14)
            throw std::runtime_error("Calculation failed: expected 14, got " + std::to_string(result));
    });

    //Тест на операцию из dll
    addCustomTest([]() {
        std::cout << "\033[33m[INFO]\033[0m Тест на нестандартную операцию из DLL (2 ^ 3 + 4)\n";
        calculator::Calculator calc;
        int result = calc.calculate("2 ^ 3 + 4");
        if (result != 12)
            throw std::runtime_error("Calculation failed: expected 12, got " + std::to_string(result));
    });
}

void Test::addDllLoaderTests() {

    // Тест на загрузку всех плагинов
    addCustomTest([]() {
        std::cout << "\033[33m[INFO]\033[0m Тест на загрузку всех плагинов\n";
        calculator::DllLoader loader("plugins");
        bool loaded = loader.loadPlugins();

        if (!loaded) {
            throw std::runtime_error("Failed to load plugins from directory");
        }
    });

    // Тест на проверку типа функции
    addCustomTest([]() {
        std::cout << "\033[33m[INFO]\033[0m Тест на проверку типа функции\n";
        calculator::DllLoader loader("plugins");

        if (!loader.isFunction("sin")) {
            throw std::runtime_error("'sin' should be a function");
        }

        if (loader.isOperation("sin")) {
            throw std::runtime_error("'sin' should not be an operation");
        }
    });

    // Тест на проверку типа операции
    addCustomTest([]() {
        std::cout << "\033[33m[INFO]\033[0m Тест на проверку типа операции\n";
        calculator::DllLoader loader("plugins");

        if (!loader.isOperation("^")) {
            throw std::runtime_error("'^' should be an operation");
        }

        if (loader.isFunction("^")) {
            throw std::runtime_error("'^' should not be a function");
        }
    });

    // Тест на выполнение функции cos(0) = 1
    addCustomTest([]() {
        std::cout << "\033[33m[INFO]\033[0m Тест на выполнение функции cos(0) = 1\n";
        calculator::DllLoader loader("plugins");

        std::stack<calculator::number> stack;
        stack.push(0);

        calculator::number result = loader.execute("cos", stack);

        if (abs(result - 1) >= 0.01) {
            throw std::runtime_error("cos(0) failed: expected 1, got " + std::to_string(result));
        }

        if (stack.size() != 1) {
            throw std::runtime_error("Stack should contain one element after execution");
        }
    });

    // Тест на выполнение операции возведения в степень 2^3 = 8
    addCustomTest([]() {
        std::cout << "\033[33m[INFO]\033[0m Тест на выполнение операции возведения в степень 2^3 = 8\n";
        calculator::DllLoader loader("plugins");

        std::stack<calculator::number> stack;
        stack.push(2);
        stack.push(3);

        calculator::number result = loader.execute("^", stack);

        if (result != 8) {
            throw std::runtime_error("Power failed: expected 8, got " + std::to_string(result));
        }

        if (stack.size() != 1) {
            throw std::runtime_error("Stack should contain one element after execution");
        }
    });

    // Тест на выполнение функции sqrt(16) = 4
    addCustomTest([]() {
        std::cout << "\033[33m[INFO]\033[0m Тест на выполнение функции sqrt(16) = 4\n";
        calculator::DllLoader loader("plugins");

        std::stack<calculator::number> stack;
        stack.push(16);

        calculator::number result = loader.execute("sqrt", stack);

        if (result != 4) {
            throw std::runtime_error("Square root failed: expected 4, got " + std::to_string(result));
        }

        if (stack.size() != 1) {
            throw std::runtime_error("Stack should contain one element after execution");
        }
    });

    // Тест на выполнение функции log(1) = 0
    addCustomTest([]() {
        std::cout << "\033[33m[INFO]\033[0m Тест на выполнение функции log(1) = 0\n";
        calculator::DllLoader loader("plugins");

        std::stack<calculator::number> stack;
        stack.push(1);

        calculator::number result = loader.execute("log", stack);

        if (abs(result - 0.0) >= 0.01) {
            throw std::runtime_error("Log failed: expected 0, got " + std::to_string(result));
        }

        if (stack.size() != 1) {
            throw std::runtime_error("Stack should contain one element after execution");
        }
    });

    // Тест на приоритет операции
    addCustomTest([]() {
        std::cout << "\033[33m[INFO]\033[0m Тест на приоритет операции\n";
        calculator::DllLoader loader("plugins");

        size_t precedence = loader.getPrecedence("^");
        std::cout << "Precedence for '^': " << precedence << std::endl;

        if (precedence == 0) {
            throw std::runtime_error("Precedence for '^' should not be 0");
        }
    });

    // Тест на смену пути плагинов
    addCustomTest([]() {
        std::cout << "\033[33m[INFO]\033[0m Тест на смену пути плагинов\n";
        calculator::DllLoader loader("plugins");
        loader.setPluginsPath("custom_plugins");

        if (loader.getPluginsPath() != "custom_plugins") {
            throw std::runtime_error("Plugins path was not changed correctly");
        }
    });

    // Тест на обработку несуществующего плагина
    addCustomTest([]() {
        std::cout << "\033[33m[INFO]\033[0m Тест на обработку несуществующего плагина\n";
        calculator::DllLoader loader("plugins");

        try {
            loader.isFunction("nonexistent_plugin");
        }
        catch (const calculator::DLLException&) {
            return;
        }

        throw std::runtime_error("Should not find nonexistent plugin");
    });

    // Тест на выгрузку всех плагинов
    addCustomTest([]() {
        std::cout << "\033[33m[INFO]\033[0m Тест на выгрузку всех плагинов\n";
        calculator::DllLoader loader("plugins");

        loader.unloadPlugins();

        try {
            loader.isFunction("sin");
        }
        catch (const calculator::DLLException&) {
            return;
        }

        throw std::runtime_error("Plugin should be unloaded after unloadPlugins()");
    });
}
