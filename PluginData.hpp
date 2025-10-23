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
    };
}
