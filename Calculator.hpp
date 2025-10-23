#pragma once
#include "types.hpp"
#include "DllLoader.hpp"

namespace calculator {
    class Calculator {
        public:
            Calculator(str pluginsPath = "plugins") : loader(pluginsPath) {}
            
            double calculate(crStr strToCalc);
            str getParsedString() const { return parsedString; }

        private:
            DllLoader loader;
            str parsedString;

            void parse(crStr strToCalc);

            str readOperation(crStr strToCalc, size_t pos, size_t& len);
            number readNumber(crStr strToCalc, size_t pos, size_t& len);

            bool isOperator(crStr token);
            bool isFunction(crStr token);
            int getPrecedence(crStr token);

            str getPluginsPath() const { return loader.getPluginsPath(); }
            void setPluginsPath(crStr path) { loader.setPluginsPath(path); }
    };
}