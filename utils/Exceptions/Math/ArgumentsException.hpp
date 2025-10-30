#pragma once
#include "MathException.hpp"

namespace calculator {
    class ArgumentsException : public MathException {
        public:
            ArgumentsException(crStr msg, crStr operation, vecNumber operands)
            : MathException(msg, operation, operands) {}

            str getTypename() const override { return "ArgumentsException"; }
    };
}