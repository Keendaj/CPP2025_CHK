#pragma once
#include "DLLException.hpp"

namespace calculator {
    class LoadExecption : public DLLException {
        public:
        LoadExecption(crStr dllPath) : DLLException("Couldn't load dll", "", "", dllPath){}

        crStr getTypename() const { return "LoadException"; }
    };
}