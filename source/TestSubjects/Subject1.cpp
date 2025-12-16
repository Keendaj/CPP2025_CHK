#include "Subject1.h"
#include <iostream>

void Subject1::doSomething(double a, const char* b)
{
    std::cout << "doSomething: a=" << a << ", b=" << b << std::endl;
}

int Subject1::calculate(int x, int y, int z)
{
    int result = x + y + z;
    std::cout << "calculate: x=" << x << ", y=" << y 
    << ", z=" << z << ", result=" << result << std::endl;
    return result;
}

void Subject1::printMessage(const std::string& msg)
{
    std::cout << "printMessage: " << msg << std::endl;
}

double Subject1::divide(double a, double b)
{
    if (b == 0) {
        std::cout << "divide: Error! Division by zero" << std::endl;
        return 0.0;
    }
    double result = a / b;
    std::cout << "divide: " << a << " / " << b << " = " << result << std::endl;
    return result;
}
