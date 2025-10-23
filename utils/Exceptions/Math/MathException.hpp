#pragma once
#include "../CalculatorException.hpp"
#include "../../types.hpp"

namespace calculator {
    class MathException : public CalculatorException{
        protected:
            str operation;
            vecNumber operands;
        public:
            MathException(crStr msg, crStr operation, const vecNumber& operands)
            : CalculatorException(msg), operation(operation), operands(operands) {}

            const char* what() const noexcept override;

            crStr get_type_name() const override { return "MathException"; }
            crStr get_operation() const { return operation; }
            const vecNumber& get_operands() const { return operands; }
    };
}