#pragma once
#include "DLLException.hpp"

namespace calculator {
    class FunctionNotFoundException : public DLLException {
        public:
            FunctionNotFoundException(crStr pluginName, crStr functionName, crStr dllPath)
            : DLLException("Function not found in plugin", pluginName, functionName, dllPath) {}

            str getTypename() const { return "FunctionNotFoundException"; }
    };
}
