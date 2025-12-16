#include "Test.h"
#include "../wrapper.h"
#include "../engine.h"
#include "../TestSubjects/Subject1.h"
#include <iostream>

void addWrappers(const Engine& engine)
{
    std::shared_ptr<Subject1> subject1 = std::make_shared<Subject1>();

    auto wrapper1 = makeWrapper(subject1, &Subject1::doSomething, { {"a", 0.0}, {"b", ""} });
    engine.registerCommand("doSomething", wrapper1);

    auto wrapper2 = makeWrapper(subject1, &Subject1::calculate, { {"x", 0}, {"y", 0}, {"z", 0} });
    engine.registerCommand("calculate", wrapper2);

    auto wrapper3 = makeWrapper(subject1, &Subject1::printMessage, { {"msg", ""} });
    engine.registerCommand("printMessage", wrapper3);

    auto wrapper4 = makeWrapper(subject1, &Subject1::divide, { {"a", 1.0}, {"b", 1.0} });
    engine.registerCommand("divide", wrapper4);
}

void runTests()
{
    Engine engine;
    addWrappers(engine);
}