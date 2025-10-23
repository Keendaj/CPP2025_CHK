#pragma once
#include "MathException.hpp"

namespace calculator {
    class ZeroDivisionException : public MathException {
        public:
            ZeroDivisionException(crStr msg = "Division by Zero", crStr operation = "division", number num) 
            : MathException(msg, operation,{num, 0.0}) {}

            str getTypename() const override { return "ZeroDivisionException"; }
    };
}