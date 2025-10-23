#pragma once
#include <vector>
#include <iostream>
#include <functional>

class Test
{
    using testFunction = std::function<void()>;
    private:
        std::vector<testFunction> tests;
        std::ostream& output;
    public:
        Test(std::ostream& outputStream) : output(outputStream) {}
        ~Test() = default;

        void addCustomTest(testFunction func);
        
        void addParserTests();
        void addCalculationTests();
        void addDllLoaderTests();
        void run();
};
