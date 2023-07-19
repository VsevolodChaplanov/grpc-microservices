#include "utils/healthcheck/healthcheck_asyncservice.hpp"
#include <exception>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <matrix_server/request_handler_impl.hpp>
#include <matrix_server/server.hpp>
#include <memory>

int main(int argc, const char* argv[]) try {
    util::healthcheck::HealthCheckAsyncServiceImpl healthcheck;
    util::healthcheck::CustomHealthCheckService custom{&healthcheck};
    MatrixMultiplyerServer server{"0.0.0.0", 50432,
                                  //   std::make_unique<decltype(custom)>(std::move(custom)),
                                  nullptr,
                                  2};

} catch (const std::exception& ex) {
    fmt::println("error: {}", ex.what());
}