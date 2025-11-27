#pragma once
#include <string>
#include <vector>

namespace calculator{
    using number = double;
    using str = std::string;
    using crStr = const std::string&;
    using vecNumber = std::vector<number>;

    inline number strToNumber(str s) {
        return std::stod(s);
    }
}