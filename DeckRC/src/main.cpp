#include <iostream>
#include <concepts>

template<typename T>
requires std::integral<T>
T add(T a, T b) {
    return a + b;
}

int main() {
    std::cout << "Hello from C++ 20 with Cmake on linux/Windows!" << std::endl;
    std::cout << "The sum is: " << add(5, 10) << std::endl;
    return 0;
} 