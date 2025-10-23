#include "DLLException.hpp"
#include <sstream>

using namespace calculator;

const char* DLLException::what() const noexcept{
    std::ostringstream oss;
    oss << "[CALCULATOR] " << getTypename() << ": " << message;
                
    if (!pluginName.empty()) {
        oss << " [Plugin: " << pluginName<< "]";
    }
                
    if (!functionName.empty()) {
        oss << " [Function: " << functionName << "]";
    }
                
    if (!dllPath.empty()) {
        oss << " [DLL: " << dllPath<< "]";
    }

    return oss.str().c_str();
}
