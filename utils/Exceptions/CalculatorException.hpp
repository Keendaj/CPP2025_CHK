#pragma once
#include <exception>
#include "../types.hpp"

namespace calculator{
    class CalculatorException : public std::exception
    {
        protected:
            str message;
        public:
            CalculatorException(crStr msg) : message(message) {}
            virtual ~CalculatorException() = default;

            virtual crStr getTypename() const { return "CalculatorException"; }
            crStr get_message() const { return message; }
    };
}