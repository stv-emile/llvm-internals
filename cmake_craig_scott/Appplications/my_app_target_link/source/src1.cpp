#include "src1.hpp"
#include "src2.hpp"
#include "src3.hpp"
#include "src4.hpp"
#include <iostream>

void CollectorFunction() {
    std::cout << "Collector is running, using:\n";
    AlgoFunction();
    EngineFunction();
    UiFunction();
}