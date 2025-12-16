#include <iostream>
#include "wrapper.h"
#include "TestSubjects/Subject1.h"

int main()
{
    std::shared_ptr<Subject1> subject = std::make_shared<Subject1>();
    auto wrap = makeWrapper(subject, &Subject1::doSomething, {{"a", 5.0}, {"b", "Hello"}});
    wrap.execute({});
    return 0;
}