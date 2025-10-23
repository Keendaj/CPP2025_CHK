#pragma once
#include "types.hpp"
#include <stack>
#ifdef PLUGIN_EXPORTS
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT 
#endif

extern "C" {
    EXPORT void getCalculation(std::stack<calculator::number>& operands);
    EXPORT bool isOperation() noexcept;
    EXPORT bool isFunction() noexcept;
    EXPORT calculator::str getName() noexcept;
    EXPORT size_t getPrecedence() noexcept;
}