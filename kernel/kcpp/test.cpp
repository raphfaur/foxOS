#include "test.hpp"
#include <memory>

Tester::Tester() {
    a = 4;
    std::unique_ptr<int> b;
    b = std::make_unique<int>(3);
}

extern "C" int test() {
    Tester a;
    auto T = new Tester();

    int * value = new int;
    *value = 0xffffffff;

    return a.a;
}