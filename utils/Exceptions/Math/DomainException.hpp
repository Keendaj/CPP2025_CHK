#pragma once
#include "MathException.hpp"

namespace calculator {
    class DomainException : public MathException {
        public:
            DomainException(crStr msg, crStr operation, const vecNumber& operands)
            : MathException(msg, operation, operands) {}

            crStr getTypename() const override { return "DomainException"; }
    };
}