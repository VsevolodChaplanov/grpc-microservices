#include <exception>
#include <fmt/core.h>
#include <matrix_server/server.hpp>
#include <fmt/printf.h>

int main(int argc, const char* argv[]) try {
    // MatrixMultiplyerServer server{"0.0.0.0", 8080};
} catch(const std::exception& ex) {
    fmt::println("error: {}", ex.what());
}