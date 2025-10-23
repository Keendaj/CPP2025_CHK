#pragma once
#include "../CalculatorException.hpp"

namespace calculator {
    class MathException : public CalculatorException{
        protected:
            str operation;
            vecNumber operands;
        public:
            MathException(crStr msg, crStr operation, const vecNumber& operands)
            : CalculatorException(msg), operation(operation), operands(operands) {}

            const char* what() const noexcept override;

            crStr getTypename() const override { return "MathException"; }
            crStr get_operation() const { return operation; }
            const vecNumber& get_operands() const { return operands; }
    };
}