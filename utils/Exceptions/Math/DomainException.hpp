#pragma once
#include "MathException.hpp"

namespace calculator {
    class DomainException : public MathException {
        public:
            DomainException(crStr msg, crStr operation, const vecNumber& operands)
            : MathException(msg, operation, operands) {}

            crStr get_type_name() const override { return "DomainException"; }
    };
}