#pragma once
#include "MathException.hpp"

namespace calculator {
    class DomainException : public MathException {
        public:
            DomainException(crStr msg, crStr operation, vecNumber operands)
            : MathException(msg, operation, operands) {}

            str getTypename() const override { return "DomainException"; }
    };
}