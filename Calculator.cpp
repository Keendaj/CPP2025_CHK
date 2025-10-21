#include "Calculator.hpp"
#include <sstream>
#include <stack>
#include <stdexcept>
#include <unordered_map>
#include <iostream>

#define TESTING

bool Calculator::isFunction(crStr token) {
    if (token.empty()) {
        return false;
    }
    
    if (isdigit(token[0])){
        return false;
    }
    if (token.size() == 1 && (token == "+" || token == "-" || token == "*" || token == "/")){
        return false;
    }

    return true;
}

bool Calculator::isOperator(crStr token) {
    if(token == "+" || token == "-" || token == "*" || token == "/"){
        return true;
    }
    //Тут будет подгрузка dll для операций, когда разберусь со структурой dll
    return false;
}

int Calculator::getPrecedence(crStr token) {
    static const std::unordered_map<str, int> precedenceMap = {
        {"*", 2},
        {"/", 2},
        {"+", 1},
        {"-", 1}
    };

    auto it = precedenceMap.find(token);
    if (it != precedenceMap.end())
        return it->second;
    
    //Тут будет подгрузка dll для приоритета, когда разберусь со структурой dll
    return 0;
}

Calculator::number Calculator::readNumber(crStr strToCalc, size_t pos, size_t& len) {
    size_t start = pos;
    bool hasDot = false;

    while  (pos < strToCalc.size() &&
           (isdigit(strToCalc[pos]) ||
           (strToCalc[pos] == '.' &&
            !hasDot))) {

        if (strToCalc[pos] == '.') {
            hasDot = true;
        }

        ++pos;
    }

    len = pos - start;
    if (len == 0)
        throw std::runtime_error("Ошибка: ожидалось число, но не найдено");

    str token = strToCalc.substr(start, len);

    try {
        return std::stod(token);
    } catch (...) {
        throw std::runtime_error("Ошибка: некорректное число: " + token);
    }
}

Calculator::str Calculator::readOperation(crStr strToCalc, size_t pos, size_t& len) {
    size_t start = pos;

    if (isalpha(strToCalc[pos])) {
        while (pos < strToCalc.size() && isalpha(strToCalc[pos])){
            ++pos;
        }
    }
    else if (ispunct(strToCalc[pos])) {
        ++pos;
    }

    len = pos - start;
    if (len == 0)
        throw std::runtime_error("Ошибка: оператор не найден");

    str op = strToCalc.substr(start, len);

    return op;
}


void Calculator::parse(crStr strToCalc) {
    parsedString.clear();
    std::ostringstream output;
    std::stack<str> opStack;
    bool expectUnary = true;

    for (size_t pos = 0; pos < strToCalc.size();) {
        char c = strToCalc[pos];

        if (isspace(c)) {
            ++pos;
            continue;
        }

        if (isdigit(c)) {
            size_t length = 0;
            number num = readNumber(strToCalc, pos, length);
            output << num << ' ';
            pos += length;
            expectUnary = false;
            continue;
        }

        if (c == '(') {
            opStack.push("(");
            ++pos;
            expectUnary = true;
            continue;
        }

        if (c == ')') {
            while (!opStack.empty() && opStack.top() != "(") {
                output << opStack.top() << ' ';
                opStack.pop();
            }
            
            if (opStack.empty()){
                throw std::runtime_error("Ошибка: несбалансированные скобки");
            }

            opStack.pop();

            if (!opStack.empty() && isFunction(opStack.top())) {
                output << opStack.top() << ' ';
                opStack.pop();
            }

            ++pos;
            expectUnary = false;
            continue;
        }

        if (isalpha(c) || ispunct(c)) {
            size_t length = 0;
            str op = readOperation(strToCalc, pos, length);

            if (op.empty()) {
                throw std::runtime_error("Не удалось распознать оператор");
            }
            pos += length;

            if (isFunction(op)) {
                opStack.push(op);
                continue;
            }

            if (expectUnary && (op == "+" || op == "-")) {
                if (op == "-") {
                    output << "0 ";
                }
                expectUnary = false;
            }

            if (isOperator(op)) {
                while (!opStack.empty() && 
                       isOperator(opStack.top()) &&
                       getPrecedence(op) <= getPrecedence(opStack.top())) {
                    output << opStack.top() << ' ';
                    opStack.pop();
                }

                opStack.push(op);
                expectUnary = false;
                continue;
            }

            throw std::runtime_error("Неизвестная операция: " + op);
        }

        throw std::runtime_error(str("Неизвестный символ: ") + c);
    }
    
    while (!opStack.empty()) {
        if (opStack.top() == "(")
            throw std::runtime_error("Ошибка: несбалансированные скобки");
        output << opStack.top() << ' ';
        opStack.pop();
    }

    parsedString = output.str();
}


Calculator::number Calculator::calculate(crStr strToCalc){
    parse(strToCalc);
    std::istringstream iss(parsedString);
    std::stack<number> stack;
    std::string token;
    #ifdef TESTING
        std::cout << "Подсчёт для строки: "<< strToCalc << std::endl;
    #endif
    while (iss >> token) {
        if (isdigit(token[0]) || 
            (token.size() > 1 && token[0] == '-' && isdigit(token[1]))) {
            stack.push(std::stod(token));
        }
        else if (isOperator(token)) {
            if (stack.size() < 2){
                throw std::runtime_error("Ошибка: недостаточно операндов для бинарной операции");
            }
            number b = stack.top(); 
            stack.pop();
            number a = stack.top();
            stack.pop();
            number res = 0;

            if (token == "+") {
                res = a + b;
            }
            else if (token == "-") {
                res = a - b;
            }
            else if (token == "*") {
                res = a * b;
            }
            else if (token == "/") {
                if (b == 0) {
                    throw std::runtime_error("Деление на ноль");
                }
                res = a / b;
            }
            else {
                // Заглушка для операции из DLL
                #ifdef TESTING
                    std::cout << "Операция: "<< token << std::endl;
                #endif
                res = a + b;
                
            }

            stack.push(res);
        }
        else if (isFunction(token)) {
            if (stack.empty())
                throw std::runtime_error("Ошибка: недостаточно операндов для функции");

            number x = stack.top(); stack.pop();
            number res = 0;

            
            #ifdef TESTING
                std::cout << "Функция: "<< token << std::endl;
            #endif
            // Заглушка для функций
            res = x;

            stack.push(res);
        }
        else {
            throw std::runtime_error("Неизвестный токен: " + token);
        }
    }

    if (stack.size() != 1)
        throw std::runtime_error("Ошибка: неверное количество элементов после вычислений");
    
    #ifdef TESTING
        std::cout << "Результат: "<< stack.top() << std::endl;
    #endif
    return stack.top();
}