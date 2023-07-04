#ifndef MATRIX_SERVER_SHUTDOWN_SIGNAL_LISTENER_HPP
#define MATRIX_SERVER_SHUTDOWN_SIGNAL_LISTENER_HPP

#include <agrpc/grpc_context.hpp>
#include <agrpc/grpc_executor.hpp>
#include <atomic>
#include <grpcpp/server_context.h>
#include <thread>
#include <boost/asio/basic_signal_set.hpp>
#include <boost/asio/signal_set.hpp>
#include <grpcpp/server.h>

namespace net = boost::asio;

class ShutdownListener final {
public:

    ShutdownListener(grpc::Server& server, agrpc::GrpcContext& listen_context) noexcept;

    void Shutdown();

    ~ShutdownListener();

private:
    grpc::Server& server_;  
    net::basic_signal_set<agrpc::GrpcExecutor> signals_;
    std::atomic_bool is_shutdown_;
    std::thread shutdown_thread_;
};

#endif
