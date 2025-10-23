#pragma once
#include "DLLException.hpp"

namespace calculator {
    class LoadExecption : public DLLException {
        public:
        LoadExecption(crStr dllPath) : DLLException("Couldn't load dll", "", "", dllPath){}

        str getTypename() const { return "LoadException"; }
    };
}