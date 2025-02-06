#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

enum class RootsCount {ZERO, ONE, TWO, INF, SIZE};

std::vector<float> SolveEquation(float const * odds, RootsCount &num);