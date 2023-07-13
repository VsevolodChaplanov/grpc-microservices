#include <agrpc/asio_grpc.hpp>
#include <atomic>
#include <boost/config.hpp>
#include <boost/version.hpp>
#include <chrono>
#include <condition_variable>
#include <coroutine>
#include <cstddef>
#include <ctime>
#include <exception>
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>
#include <utility>

#include <boost/asio/awaitable.hpp>
#include <boost/asio/coroutine.hpp>

struct ModelVar final {
    int value_;
    bool is_ready_;
};

struct ValueWaiter final {
    struct Awaitable;

    struct [[nodiscard]] Promise final {
        ModelVar value_;

        [[nodiscard]] static Awaitable await_transform(ModelVar&& variable) { return Awaitable{variable}; }
        [[nodiscard]] constexpr auto get_return_object() { return coro_handle::from_promise(*this); }
        [[nodiscard]] static constexpr std::suspend_never initial_suspend() noexcept { return {}; }
        [[nodiscard]] static constexpr std::suspend_never final_suspend() noexcept { return {}; }
        void return_value(ModelVar varaible) { value_ = varaible; }
        static void unhandled_exception() { std::terminate(); }
    };
    struct [[nodiscard]] Awaitable final {
        ModelVar value_;

        [[nodiscard]] bool await_ready() const noexcept { return value_.is_ready_; }
        [[nodiscard]] auto await_resume() const noexcept { return value_.value_; }
        void await_suspend(std::coroutine_handle<Promise>) const noexcept {}
        explicit Awaitable(ModelVar variable) : value_{variable} {};
    };

    using promise_type = Promise;
    using coro_handle = std::coroutine_handle<promise_type>;

    [[nodiscard]] int value() const noexcept {
        if (handle_) {
            return handle_.promise().value_.value_;
        }
        return 0;
    }

    //NOLINTNEXTLINE(google-explicit-constructor)
    ValueWaiter(coro_handle handle) : handle_{handle} {}
    ValueWaiter(const ValueWaiter&) = delete;
    ValueWaiter& operator=(const ValueWaiter&) = delete;
    ValueWaiter(ValueWaiter&& other) noexcept : handle_{std::exchange(other.handle_, nullptr)} {}
    ValueWaiter& operator=(ValueWaiter&& other) noexcept {
        if (this != std::addressof(other)) {
            handle_ = std::exchange(other.handle_, nullptr);
        }
        return *this;
    }

private:
    coro_handle handle_;
};


ValueWaiter WaitValue() {
    co_return ModelVar{.value_ = 42, .is_ready_ = true};
}

int main(int argc, const char* argv[]) try {
    const auto value = WaitValue();
    std::cout << value.value();
} catch (...) {
}