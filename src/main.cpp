#include <iostream>

int main(int argc, const char* argv[]) try {
    std::cout << "Hello world" << std::endl;
} catch (...) {
    std::cout << "Exception!" << std::endl;
}