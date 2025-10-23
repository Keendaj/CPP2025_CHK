#pragma once
#include "MathException.hpp"

namespace calculator {
    class ZeroDivisionException : public MathException {
        public:
            ZeroDivisionException(crStr operation = "division", number num) 
            : MathException("Division by Zero", operation,{num, 0.0}) {}

            crStr get_type_name() const override { return "ZeroDivisionException"; }
    };
}