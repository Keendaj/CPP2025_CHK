#pragma once
#include "types.hpp"

#ifdef PLUGIN_EXPORTS
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT 
#endif

extern "C" {
    EXPORT calculator::number getCalculation(const calculator::vecNumber& operands);
    EXPORT bool isOperation() noexcept;
    EXPORT bool isFunction() noexcept;
    EXPORT calculator::str getName() noexcept;
}