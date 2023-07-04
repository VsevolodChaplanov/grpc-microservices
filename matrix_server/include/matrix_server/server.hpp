#ifndef MATRIX_SERVER_SERVER_HPP
#define MATRIX_SERVER_SERVER_HPP

#include "matrix_matrix_service_impl.hpp"
#include "matrix_vector_service_impl.hpp"
#include "shutdown_signal_listener.hpp"
#include <agrpc/asio_grpc.hpp>
#include <agrpc/grpc_context.hpp>
#include <boost/asio/thread_pool.hpp>
#include <fmt/format.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <memory>
#include <ranges>
#include <string_view>
#include <thread>
#include <vector>

namespace net = boost::asio;
namespace mms = matrix_distributed_computing;

class MatrixMultiplyerServer final {
public:
    template<typename RequestHandler>
    MatrixMultiplyerServer(std::string_view host, std::size_t port, RequestHandler&& handler,
                           std::size_t concurrency_hint = std::thread::hardware_concurrency());

    ~MatrixMultiplyerServer();

private:
    const std::size_t threads_amount_;
    MatrixMatrixServiceImpl matrix_matrix_service_;
    MatrixVectorServiceImpl matrix_vector_service_;
    std::vector<std::unique_ptr<agrpc::GrpcContext>> grpc_contexts_;
    boost::asio::thread_pool thread_pool_;
    std::unique_ptr<grpc::Server> server_;
    std::unique_ptr<ShutdownListener> shutdown_listener_;
};

template<typename RequestHandler>
MatrixMultiplyerServer::MatrixMultiplyerServer(std::string_view host, std::size_t port,
                                               RequestHandler&& handler, std::size_t concurrency_hint)
    : threads_amount_{std::max(1ul, concurrency_hint)} {

    grpc::ServerBuilder builder;

    for (const std::size_t i: std::views::iota(0ul, threads_amount_)) {
        grpc_contexts_.emplace_back(
                std::make_unique<agrpc::GrpcContext>(builder.AddCompletionQueue()));
    }

    builder.AddListeningPort(
            fmt::format("{}:{}", host, port),
            grpc::InsecureServerCredentials());

    builder.RegisterService(&matrix_matrix_service_);
    builder.RegisterService(&matrix_vector_service_);

    server_ = builder.BuildAndStart();
    shutdown_listener_ = std::make_unique<ShutdownListener>(*server_, *grpc_contexts_.front());

    for (const std::size_t i: std::views::iota(0ul, threads_amount_)) {
        net::post(thread_pool_, [&, i]() {
            auto& grpc_context = *grpc_contexts_[i];
            handler(grpc_context, matrix_matrix_service_, matrix_vector_service_);
            grpc_context.run();
        });
    }
}

#endif
