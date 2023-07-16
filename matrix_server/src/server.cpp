#include "matrix_server/server.hpp"
#include "matrix_server/request_handler_impl.hpp"
#include <algorithm>
#include <memory>

MatrixMultiplyerServer::MatrixMultiplyerServer(std::string_view host, std::size_t port,
                                               std::size_t concurrency_hint)
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
            grpc_context.run();
        });
    }
}

MatrixMultiplyerServer::~MatrixMultiplyerServer() {
    thread_pool_.join();
    server_->Shutdown();
}