#pragma once
#include "MathException.hpp"

namespace calculator {
    class ZeroDivisionException : public MathException {
        public:
            ZeroDivisionException(crStr msg = "Division by Zero", crStr operation = "division", vecNumber num = {0}) 
            : MathException(msg, operation, num) {}

            str getTypename() const override { return "ZeroDivisionException"; }
    };
}