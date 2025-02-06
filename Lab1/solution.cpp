#include "solution.h"

enum class TypesOfEquation {CONSTANT, LINEAR, QUADRATIC, SIZE};

TypesOfEquation DetermineType(float const * odds){
    if(odds[0] == 0){
        if(odds[1]==0){
            return TypesOfEquation::CONSTANT;
        }
        return TypesOfEquation::LINEAR;
    }
    return TypesOfEquation::QUADRATIC;
}

std::vector<float> SolveConstant(float c, RootsCount &num){
    num = RootsCount::ZERO;
    
    if(c == 0){
        num = RootsCount::INF;
    }
    
    return {};
}

std::vector<float> SolveLinear(float const* odds, RootsCount &num){
    num = RootsCount::ONE;
    return {-odds[2] / odds[1] };
}

std::vector<float> SolveQuadratic(float const* odds,RootsCount &num){
    float D = odds[1]*odds[1] - 4 * odds[0] * odds[2];
    std::vector<float> roots;
    if(D > 0){
        num = RootsCount::TWO;
        D = std::sqrt(D);

        roots.push_back( (-odds[1] - D) / (2 * odds[0]) );
        roots.push_back( (-odds[1] + D) / (2 * odds[0]) );
    }
    else if(D == 0){
        num = RootsCount::ONE;
        roots.push_back(-odds[1] / (2 * odds[0]));
    }
    else{
        num = RootsCount::ZERO;
    }
    return roots;
}

std::vector<float> SolveEquation(float const * odds, RootsCount &num){
    if(odds = nullptr){
        num = RootsCount::ERROR;
        return {};
    }
    TypesOfEquation type = DetermineType(odds);
    std::vector<float> roots; 
    switch (type)
    {
    case TypesOfEquation::CONSTANT:
        roots = SolveConstant(odds[2],num);
        break;
    case TypesOfEquation::LINEAR:
        roots = SolveLinear(odds,num);
        break;
    case TypesOfEquation::QUADRATIC:
        roots = SolveQuadratic(odds, num);
        break;
    default:
        break;
    }
    return roots;
}

void WriteRootsStream(std::ostream &output, std::vector<float> const &roots, RootsCount const &num){
    switch (num)
    {
    case RootsCount::ZERO:
        output << "The equation has no roots";
            break;
        case RootsCount::ONE:
            output << "The equation has one root: " << roots[0];
            break;
        case RootsCount::TWO:
            output << "The equation has two roots: " << roots[0] << ", " << roots[1];
            break;
        case RootsCount::INF:
            output << "The equaton has oo roots";
        default:
            std::cerr << "Something went wrong while solving equation";
            break;
    }
    output << "\n";
    if (output.fail()) {
        std::cerr << "Couldn`t write in this stream\n";
    }
}

float * ReadOddsStream(std::istream &input,  char const &separator = ';'){
    std::string line;
    float odds[3];
    
    
    if(!std::getline(input, line)){
        return nullptr;
    }

    std::stringstream ss(line);
    std::string odd;
    int i = 0;

    while (std::getline(ss,odd,separator)) {
        odds[i] = std::stof(odd);
        i++;
    }
    
    return odds;
}
