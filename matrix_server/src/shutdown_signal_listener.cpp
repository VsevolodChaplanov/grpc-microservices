#include "matrix_server/shutdown_signal_listener.hpp"
#include <agrpc/grpc_context.hpp>
#include <boost/asio/error.hpp>
#include <thread>

ShutdownListener::ShutdownListener(grpc::Server& server, agrpc::GrpcContext& listener_context) noexcept
    : server_{server}, signals_(listener_context, SIGINT, SIGTERM) {
        signals_.async_wait([&] (auto&& ex, [[maybe_unused]] auto&&) {
            if (ex != boost::asio::error::operation_aborted) {
                Shutdown();
            }
        });
    }

void ShutdownListener::Shutdown() {
    if (!is_shutdown_.exchange(true)) {
        shutdown_thread_ = std::thread{
            [&] {
                signals_.cancel();
                server_.Shutdown();
            }
        };
    }
}

ShutdownListener::~ShutdownListener() {
    if (shutdown_thread_.joinable()) {
        shutdown_thread_.join();
    } else {
        server_.Shutdown();
    }
}