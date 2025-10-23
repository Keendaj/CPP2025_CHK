#pragma once
#include "../CalculatorException.hpp"

namespace calculator {
    class DLLException : public CalculatorException {
        protected:
            std::string pluginName;
            std::string functionName;
            std::string dllPath;

        public:
            DLLException(crStr message, crStr pluginName = "", crStr functionName = "", crStr dllPath = "")
            : CalculatorException(message), pluginName(pluginName), functionName(functionName), dllPath(dllPath) {}

            const char* what() const noexcept override;

            crStr getTypename() const override { return "PluginException"; }
            crStr getPluginName() const { return pluginName; }
            crStr getFunctionName() const { return functionName; }
            crStr getDllPath() const { return dllPath; }
    };
}