#include "custom_literal.h"
#include <iostream>
#include <thread>

using namespace Chena;

int main() {

    using namespace std::literals;
    std::cout << "i * i = " << 1i * 1i << std::endl;
    std::cout << "Waiting for 500ms" << std::endl;
    std::this_thread::sleep_for(500ms);
    std::cout << "Hello world"s.substr(0, 5) << std::endl;

    // length literal
    length l1(1.0, length::metre);
    length l2(100.0, length::centimetre);
    length l3 = l1 + l2;

    std::cout << "Length 1: " << l1.value << " metres" << std::endl;
    std::cout << "Length 2: " << l2.value << " metres" << std::endl;
    std::cout << "Length 3: " << l3.value << " metres" << std::endl;

    return 0;
}