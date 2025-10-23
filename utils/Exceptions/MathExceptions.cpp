#include "MathExceptions.hpp"
#include <sstream>

const char* calculator::MathException::what() const {
    str return_msg = "";
    std::ostringstream oss;
    oss << "[CALCULATOR] " << get_type_name() << ": " << message;
            
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