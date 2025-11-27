#include "Calculator.hpp"
#include <sstream>
#include <stack>
#include <unordered_map>
#include <filesystem>
#include <windows.h>
#include "utils/Exceptions/DLL/DLLException.hpp"
#include "utils/Exceptions/Math/ZeroDivisionException.hpp"

#ifdef TESTING
    #include <iostream>
#endif

using namespace calculator;

bool Calculator::isFunction(crStr token) {
    if(token.empty() || token == "(" || token == ")"){
        return false;
    }
    try
    {
        #ifdef TESTING
            std::cout << "Проверка на функцию: " << token << std::endl;
        #endif
        return loader.isFunction(token);
    }
    catch(const DLLException& e)
    {
        return false;
    }
}

bool Calculator::isOperator(crStr token) {
    
    if(token.empty() || token == "(" || token == ")"){
        return false;
    }

    if(token == "+" || token == "-" || token == "*" || token == "/"){
        return true;
    }
    try
    {
        #ifdef TESTING
            std::cout << "Проверка на операцию: " << token << std::endl;
        #endif
        
        return loader.isOperation(token);
    }
    catch(const DLLException& e)
    {
        return false;
    }
}

int Calculator::getPrecedence(crStr token) {
    static const std::unordered_map<str, int> precedenceMap = {
        {"(", 256},
        {"*", 2},
        {"/", 2},
        {"+", 1},
        {"-", 1}
    };

    auto it = precedenceMap.find(token);
    if (it != precedenceMap.end())
        return it->second;
    
    try
    {
        return loader.getPrecedence(token);
    }
    catch(const DLLException& e)
    {
        return 255;
    }
}

str Calculator::readNumber(crStr strToCalc, size_t pos, size_t& len) {
    if (pos >= strToCalc.size()) {
        throw std::runtime_error("Incorrect number position");
    }

    size_t start = pos;
    bool hasDot = false;

    while (pos < strToCalc.size() &&
          (isdigit(strToCalc[pos]) ||
          (strToCalc[pos] == '.' && !hasDot))) {
        if (strToCalc[pos] == '.') {
            hasDot = true;
        }
        ++pos;
    }

    len = pos - start;
    if (len == 0 || start + len > strToCalc.size()) {
        throw std::runtime_error("Incorrect number length");
    }

    str token = strToCalc.substr(start, len);

    try {
        return token;
    } catch (...) {
        throw std::runtime_error("Error: incorrect number: " + token);
    }
}

str Calculator::readOperation(crStr strToCalc, size_t pos, size_t& len) {
    if (pos >= strToCalc.size()) {
        throw std::runtime_error("Incorrect operation position");
    }

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
    if (len == 0 || start + len > strToCalc.size()) {
        throw std::runtime_error("Incorrect operation length");
    }
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
            str num = readNumber(strToCalc, pos, length);
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
                throw std::runtime_error("Error with brackets");
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
                throw std::runtime_error("Couldn't parse operation");
            }
            pos += length;

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

            if (isFunction(op)) {
                opStack.push(op);
                continue;
            } 

            throw std::runtime_error("Unknown operation: " + op);
        }

        throw std::runtime_error("Unknown symbol: " + c);
    }
    
    while (!opStack.empty()) {
        if (opStack.top() == "("){
            throw std::runtime_error("Error with brackets");
        }
        output << opStack.top() << ' ';
        opStack.pop();
    }

    parsedString = output.str();
}


number Calculator::calculate(crStr strToCalc){
    parse(strToCalc);
    #ifdef TESTING
        std::cout << "Строка: "<< parsedString << std::endl;
    #endif
    std::istringstream iss(parsedString);
    std::stack<number> stack;
    std::string token;
    #ifdef TESTING
        std::cout << "Подсчёт для строки: "<< parsedString << std::endl;
    #endif
    while (iss >> token) {
        #ifdef TESTING
            std::cout << "Токен: "<< token << std::endl;
        #endif
        if (isdigit(token[0]) || 
            (token.size() > 1 && token[0] == '-' && isdigit(token[1]))) {
                try
                {
                    stack.push(strToNumber(token));
                }
                catch(const std::exception& e)
                {
                    throw std::runtime_error("Can't convert " + token);
                }
                
            
        }
        else if (isOperator(token)) {
            if (stack.size() < 2){
                throw std::runtime_error("Error: too few operands for binary function");
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
                    throw ZeroDivisionException();
                }
                res = a / b;
            }
            else {
                #ifdef TESTING
                    std::cout << "Операция: "<< token << std::endl;
                #endif
                stack.push(a);
                stack.push(b);
                loader.execute(token, stack);
                
                continue;
                
            }

            stack.push(res);
        }
        else if (isFunction(token)) {
            if (stack.empty())
                throw std::runtime_error("Error: too few operands for unary function");
                
            number t = loader.execute(token, stack);

            
            #ifdef TESTING
                std::cout << "Функция: "<< token << " Число: " << t << std::endl;
            #endif
        }
        else {
            throw std::runtime_error("Unknown token: " + token);
        }
    }

    if (stack.size() != 1)
        throw std::runtime_error("Error: false number of elements after calculation");
    
        #ifdef TESTING
            std::cout << "Результат: "<< stack.top() << std::endl;
        #endif
    return stack.top();
}
