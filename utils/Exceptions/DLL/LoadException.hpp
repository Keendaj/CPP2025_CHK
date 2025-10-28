#pragma once
#include "DLLException.hpp"

namespace calculator {
    class LoadException : public DLLException {
        public:
        LoadException(crStr msg = "Couldn't load dll", crStr dllPath = "") : DLLException(msg, "", "", dllPath){}

        str getTypename() const noexcept { return "LoadException"; }
    };
}