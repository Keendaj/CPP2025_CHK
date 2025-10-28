#pragma once
#include "../CalculatorException.hpp"

namespace calculator {
    class DLLException : public CalculatorException {
        protected:
            str pluginName;
            str functionName;
            str dllPath;

        public:
            DLLException(crStr message, crStr pluginName = "", crStr functionName = "", crStr dllPath = "")
            : CalculatorException(message), pluginName(pluginName), functionName(functionName), dllPath(dllPath) {}

            const char* what() const noexcept override;

            str getTypename() const override { return "PluginException"; }
            str getPluginName() const { return pluginName; }
            str getFunctionName() const { return functionName; }
            str getDllPath() const { return dllPath; }
    };
}