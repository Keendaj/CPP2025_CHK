#pragma once
#include "../CalculatorException.hpp"

namespace calculator {
    class MathException : public CalculatorException{
        protected:
            str operation;
            vecNumber operands;
        public:
            MathException(crStr msg, crStr operation, vecNumber operands)
            : CalculatorException(msg), operation(operation), operands(operands) {}

            const char* what() const noexcept override;

            str getTypename() const override { return "MathException"; }
            str get_operation() const { return operation; }
            vecNumber get_operands() const { return operands; }
    };
}