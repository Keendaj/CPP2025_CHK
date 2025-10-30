#pragma once
#include "types.hpp"

namespace calculator {
    enum class OperationType{
        OPERATION,
        FUNCTION
    };

    struct PluginData
    {
        str name;
        OperationType type;
        size_t precedence = 0;
        str path;
        HMODULE handle = nullptr;
    };
}
