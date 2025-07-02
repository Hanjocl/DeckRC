#include "test.h"
#include <iostream>

int adder(int a, int b) {
    int result = a + b;
    std::cout << "Adder result with STD: " << result << std::endl;

    return result;
}
