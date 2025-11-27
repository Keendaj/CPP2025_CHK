#include "CalculatorException.hpp"
#include <sstream>

namespace calculator {
    const char* CalculatorException::what() const noexcept {
        std::ostringstream oss;
        oss << "[CALCULATOR] " << getTypename() << ": " << message;
                
        return oss.str().c_str();
    }
}
