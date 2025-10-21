#pragma once
#include <string>

class Calculator{
    using crStr = const std::string&;
    using str = std::string;
    using number = double;
    public:
        Calculator() = default;
        
        double calculate(crStr strToCalc);
        str getParsedString() const { return parsedString; }

    private:
        str parsedString;
        void parse(crStr strToCalc);

        str readOperation(crStr strToCalc, size_t pos, size_t& len);
        number readNumber(crStr strToCalc, size_t pos, size_t& len);

        static bool isOperator(crStr token);
        static bool isFunction(crStr token);
        static int getPrecedence(crStr token);
};