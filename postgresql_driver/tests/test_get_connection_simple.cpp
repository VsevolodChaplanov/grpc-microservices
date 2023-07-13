#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <postgres/async_connection.hpp>

struct ConnectionProperties {
    static inline constexpr char conn_string[] = "postgres://postgres:Mys3Cr3t@localhost:5432/";
};

using namespace coro_postgres;

DBConnectionPtr conn;

AsyncConnection GetAsyncConnection(std::string connection_string) {
    const auto res = co_await connection_string;
    // A place for possible preparation of queries
    co_return res;
}

SCENARIO_METHOD(ConnectionProperties, "Get Postgresql connection asynchronously using coroutines") {
    auto connectionWrapper = GetAsyncConnection(conn_string);
    auto realConnection = connectionWrapper();
    CHECK(realConnection->is_open());
}