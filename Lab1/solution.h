#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

enum class RootsCount {ZERO, ONE, TWO, INF, ERROR, SIZE};

std::vector<float> SolveEquation(float const * odds, RootsCount &num);
void WriteRootsToStream(std::ostream &output, std::vector<float> const &roots, RootsCount const &num);
float * ReadOddsFromStream(std::istream &input,  char const &separator = ';');