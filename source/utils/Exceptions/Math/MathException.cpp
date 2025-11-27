#include "MathException.hpp"
#include <sstream>

const char* calculator::MathException::what() const noexcept{
    std::ostringstream oss;
    oss << "[CALCULATOR] " << getTypename() << ": " << message;
            
    if (!operation.empty()) {
        oss << " [Operation: " << operation << "]";
    }
            
    if (!operands.empty()) {
        oss << " [Operands: ";
        for (size_t i = 0; i < operands.size(); ++i) {
            oss << operands[i];
            if (i < operands.size() - 1) oss << ", ";
        }
        oss << "]";
    }
            
    return oss.str().c_str();
}