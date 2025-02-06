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

