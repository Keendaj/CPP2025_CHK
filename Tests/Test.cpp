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
        calculator::Calculator calc;
        calc.calculate("2 * (3 + (4 - 1))");
        std::string expected = "2 3 4 1 - + * ";
        if (calc.getParsedString() != expected){
	        throw std::runtime_error("Неправильная RPN: ожидалось \"" + expected +
                                 "\", получили \"" + calc.getParsedString() + "\"");
        }
    });

    //Тест на операцию из dll пока что ему плохо, т.к. парсер считает, что все не стандартные операции - унарные функции и приоритету плохо
    addCustomTest([](){
        calculator::Calculator calc;
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
        calculator::Calculator calc;
        int result = calc.calculate("2 + 3 * 4");
        if (result != 14)
            throw std::runtime_error("Calculation failed: expected 14, got " + std::to_string(result));
    });

    //Тест на функции
    addCustomTest([]() {
        calculator::Calculator calc;
        int result = calc.calculate("sqrt(16) + cos(0)"); //Пока что заглушки, потому это 16 + 0
        if (result != 16)
            throw std::runtime_error("Calculation failed: expected 16, got " + std::to_string(result));
    });

    //Тест на унарный минус
    addCustomTest([]() {
        calculator::Calculator calc;
        int result = calc.calculate("-3 + 5");
        if (result != 2)
            throw std::runtime_error("Calculation failed: expected 2, got " + std::to_string(result));
    });

    //Тест на скобки
    addCustomTest([]() {
        calculator::Calculator calc;
        int result = calc.calculate("2 * (3 + 4)");
        if (result != 14)
            throw std::runtime_error("Calculation failed: expected 14, got " + std::to_string(result));
    });

    //Тест на операцию из dll пока что ему плохо, т.к. парсер считает, что все не стандартные операции - унарные функции и приоритету плохо
    addCustomTest([]() {
        calculator::Calculator calc;
        int result = calc.calculate("2 @ 3 + 4");
        if (result != 9)
            throw std::runtime_error("Calculation failed: expected 9, got " + std::to_string(result));
    });
}

void Test::addDllLoaderTests() {

    // Тест на загрузку плагина функции
    addCustomTest([]() {
        calculator::DllLoader loader("plugins");
        calculator::PluginData data;
        data.name = "sin";
        data.type = calculator::OperationType::FUNCTION;
        
        bool loaded = loader.load(data);
        if (!loaded) {
            throw std::runtime_error("Failed to load plugin 'add'");
        }
        
        if (!loader.isPluginLoaded()) {
            throw std::runtime_error("Plugin should be loaded but isPluginLoaded() returns false");
        }
        
        if (loader.getCurrentPluginName() != "add") {
            throw std::runtime_error("Current plugin name should be 'add'");
        }
    });

    // Тест на загрузку плагина операции
    addCustomTest([]() {
        calculator::DllLoader loader("plugins");
        calculator::PluginData data;
        data.name = "^";
        data.type = calculator::OperationType::OPERATION;
        
        bool loaded = loader.load(data);
        if (!loaded) {
            throw std::runtime_error("Failed to load plugin 'add'");
        }
        
        if (!loader.isPluginLoaded()) {
            throw std::runtime_error("Plugin should be loaded but isPluginLoaded() returns false");
        }
        
        if (loader.getCurrentPluginName() != "add") {
            throw std::runtime_error("Current plugin name should be 'add'");
        }
    });

    // Тест на выполнение функции через плагин
    addCustomTest([]() {
        calculator::DllLoader loader("plugins");
        calculator::PluginData data;
        data.name = "cos";
        data.type = calculator::OperationType::FUNCTION;
        
        loader.load(data);
        
        std::stack<calculator::number> stack;
        stack.push(0);
        
        calculator::number result = loader.execute(data, stack);
        
        if (abs(result - 1) >= 0.01) {
            throw std::runtime_error("Addition failed: expected 0, got " + std::to_string(result));
        }
        
        if (!stack.empty()) {
            throw std::runtime_error("Stack should be empty after execution");
        }
    });

    // Тест на выполнение операции через плагин
    addCustomTest([]() {
        calculator::DllLoader loader("plugins");
        calculator::PluginData data;
        data.name = "^";
        data.type = calculator::OperationType::OPERATION;
        
        loader.load(data);
        
        std::stack<calculator::number> stack;
        stack.push(2);
        stack.push(3);
        
        calculator::number result = loader.execute(data, stack);
        
        if (result != 8) {
            throw std::runtime_error("Addition failed: expected 8, got " + std::to_string(result));
        }
        
        if (!stack.empty()) {
            throw std::runtime_error("Stack should be empty after execution");
        }
    });

    // Тест на проверку типа операции
    addCustomTest([]() {
        calculator::DllLoader loader("plugins");
        calculator::PluginData opData;
        opData.name = "^";
        opData.type = calculator::OperationType::OPERATION;
        
        loader.load(opData);
        
        if (!loader.isOperation()) {
            throw std::runtime_error("add should be an operation");
        }
        
        if (loader.isFunction()) {
            throw std::runtime_error("add should not be a function");
        }
    });

    // Тест на проверку типа функции
    addCustomTest([]() {
        calculator::DllLoader loader("plugins");
        calculator::PluginData funcData;
        funcData.name = "sqrt";
        funcData.type = calculator::OperationType::FUNCTION;
        
        loader.load(funcData);
        
        if (!loader.isFunction()) {
            throw std::runtime_error("sqrt should be a function");
        }
        
        if (loader.isOperation()) {
            throw std::runtime_error("sqrt should not be an operation");
        }
    });

    // Тест на приоритет операции
    addCustomTest([]() {
        calculator::DllLoader loader("plugins");
        calculator::PluginData data;
        data.name = "^";
        data.type = calculator::OperationType::OPERATION;
        
        loader.load(data);
        
        size_t precedence = loader.getPrecedence();
        std::cout << "Precedence for 'add': " << precedence << std::endl;
    });

    // Тест на смену пути плагинов
    addCustomTest([]() {
        calculator::DllLoader loader("plugins");
        loader.setPluginsPath("custom_plugins");
        
        if (loader.getPluginsPath() != "custom_plugins") {
            throw std::runtime_error("Plugins path was not changed correctly");
        }
    });

    // Тест на выгрузку плагина
    addCustomTest([]() {
        calculator::DllLoader loader("plugins");
        calculator::PluginData data;
        data.name = "^";
        data.type = calculator::OperationType::OPERATION;
        
        loader.load(data);
        
        if (!loader.isPluginLoaded()) {
            throw std::runtime_error("Plugin should be loaded before unload");
        }
        
        loader.unload();
        
        if (loader.isPluginLoaded()) {
            throw std::runtime_error("Plugin should be unloaded after unload()");
        }
        
        if (!loader.getCurrentPluginName().empty()) {
            throw std::runtime_error("Plugin name should be empty after unload");
        }
    });

    // Тест на обработку несуществующего плагина
    addCustomTest([]() {
        calculator::DllLoader loader("plugins");
        calculator::PluginData data;
        data.name = "nonexistent_plugin";
        data.type = calculator::OperationType::OPERATION;
        
        bool loaded = loader.load(data);
        
        if (loaded) {
            throw std::runtime_error("Should not load nonexistent plugin");
        }
    });

    // Тест на выполнение функции
    addCustomTest([]() {
        calculator::DllLoader loader("plugins");
        calculator::PluginData data;
        data.name = "sqrt";
        data.type = calculator::OperationType::FUNCTION;
        
        loader.load(data);
        
        std::stack<calculator::number> stack;
        stack.push(16);
        
        calculator::number result = loader.execute(data, stack);
        
        if (result != 4) {
            throw std::runtime_error("Square root failed: expected 4, got " + std::to_string(result));
        }
    });
}