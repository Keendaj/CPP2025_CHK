#pragma once
#include "DLLException.hpp"

namespace calculator {
    class FunctionNotFoundException : public DLLException {
        public:
            FunctionNotFoundException(crStr pluginName, crStr functionName, crStr dllPath)
            : DLLException("Function not found in plugin", pluginName, functionName, dllPath) {}

            crStr getTypename() const { return "FunctionNotFoundException"; }
    };
}
