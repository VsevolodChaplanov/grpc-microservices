#ifndef POSTGRESQL_DRIVER_POSTGRES_ASYNC_CONNECTION_HPP
#define POSTGRESQL_DRIVER_POSTGRES_ASYNC_CONNECTION_HPP

#include <coroutine>
#include <exception>
#include <memory>
#include <pqxx/connection>
#include <variant>

namespace coro_postgres {

    using DBConnection = pqxx::connection;
    using DBConnectionPtr = std::shared_ptr<DBConnection>;

    struct [[nodiscard]] AsyncConnection final {
    public:
        struct ConnectionAwaiter;
        struct ConnectionPromise;

        using promise_type = ConnectionPromise;
        using CoroHandle = std::coroutine_handle<promise_type>;

        struct [[nodiscard]] ConnectionPromise final {
            std::variant<std::monostate, DBConnectionPtr, std::exception_ptr> db_connection_;

            // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
            [[nodiscard]] ConnectionAwaiter await_transform(const std::string& connection_string) {
                return {connection_string};
            }
            [[nodiscard]] AsyncConnection get_return_object() {
                return {CoroHandle::from_promise(*this)};
            }
            // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
            [[nodiscard]] constexpr std::suspend_never initial_suspend() noexcept { return {}; }
            // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
            [[nodiscard]] constexpr std::suspend_always final_suspend() noexcept { return {}; }
            void return_value(DBConnectionPtr connection) {
                db_connection_ = std::move(connection);
            }
            void unhandled_exception() { db_connection_ = std::current_exception(); }
        };
        struct [[nodiscard]] ConnectionAwaiter final {
            DBConnectionPtr db_connection_;

            [[nodiscard]] bool await_ready() const noexcept(noexcept(std::declval<DBConnection>().is_open())) {
                return db_connection_->is_open();
            }
            [[nodiscard]] DBConnectionPtr await_resume() const noexcept {
                return db_connection_;
            }
            void await_suspend(std::coroutine_handle<ConnectionPromise>) const noexcept {}
            // NOLINTNEXTLINE(google-explicit-constructor)
            ConnectionAwaiter(const std::string& connection_string)
                : db_connection_{std::make_shared<DBConnection>(connection_string)} {};
        };


        // NOLINTNEXTLINE(google-explicit-constructor)
        AsyncConnection(CoroHandle handle) noexcept(std::is_nothrow_copy_constructible_v<CoroHandle>)
            : handle_{handle} {}

        AsyncConnection(const AsyncConnection&) = delete;
        AsyncConnection& operator=(const AsyncConnection&) = delete;

        AsyncConnection(AsyncConnection&& other) noexcept
            : handle_{std::exchange(other.handle_, nullptr)} {}
        AsyncConnection& operator=(AsyncConnection&& other) noexcept {
            if (this != std::addressof(other)) {
                handle_ = std::exchange(other.handle_, nullptr);
            }
            return *this;
        }

        DBConnectionPtr operator()() const noexcept(false) {
            if (handle_) {
                const auto& coro_promise = handle_.promise();
                if (std::holds_alternative<DBConnectionPtr>(coro_promise.db_connection_)) {
                    return std::get<DBConnectionPtr>(coro_promise.db_connection_);
                } else if (std::holds_alternative<std::exception_ptr>(coro_promise.db_connection_)) {
                    std::rethrow_exception(std::get<std::exception_ptr>(coro_promise.db_connection_));
                }
            }
            return nullptr;
        }

        ~AsyncConnection() {
            if (handle_)
                handle_.destroy();
        }

    private:
        CoroHandle handle_;
    };

}// namespace coro_postgres

#endif