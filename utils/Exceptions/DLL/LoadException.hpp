#pragma once
#include "DLLException.hpp"

namespace calculator {
    class LoadExecption : public DLLException {
        public:
        LoadExecption(crStr msg = "Couldn't load dll", crStr dllPath = "") : DLLException(msg, "", "", dllPath){}

        str getTypename() const { return "LoadException"; }
    };
}