#pragma once
#include "types.hpp"
#include <stack>
#ifdef PLUGIN_EXPORTS
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT 
#endif

struct calc_stack {
    void* handle;
};

extern "C" {
    EXPORT void getCalculation(calculator::number* operands, size_t* count);
    EXPORT bool isOperation() noexcept;
    EXPORT bool isFunction() noexcept;
    EXPORT const char* getName() noexcept;
    EXPORT size_t getPrecedence() noexcept;
}