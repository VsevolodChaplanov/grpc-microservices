#include <exception>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <matrix_server/request_handler_impl.hpp>
#include <matrix_server/server.hpp>

int main(int argc, const char* argv[]) try {
    MatrixMultiplyerServer server{"0.0.0.0", 8080};
} catch (const std::exception& ex) {
    fmt::println("error: {}", ex.what());
}