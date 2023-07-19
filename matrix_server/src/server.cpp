#include "matrix_server/server.hpp"
#include "matrix_server/request_handler_impl.hpp"
#include "utils/healthcheck/healthcheck_asyncservice.hpp"
#include <agrpc/asio_grpc.hpp>
#include <agrpc/repeatedly_request.hpp>
#include <algorithm>
#include <cassert>
#include <fmt/core.h>
#include <grpc/grpc.h>
#include <grpcpp/ext/health_check_service_server_builder_option.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/impl/server_builder_option.h>
#include <grpcpp/server_builder.h>
#include <memory>
#include <ranges>
#include <thread>

MatrixMultiplyerServer::MatrixMultiplyerServer(std::string_view host, std::size_t port,
                                               std::unique_ptr<grpc::HealthCheckServiceInterface> service,
                                               std::size_t concurrency_hint)
    : threads_amount_{std::max(1ul, concurrency_hint)} {

    {
        grpc::EnableDefaultHealthCheckService(true);

        grpc::ServerBuilder builder;

        if (service) {
            std::unique_ptr<grpc::ServerBuilderOption> healthcheckOption{
                    new grpc::HealthCheckServiceServerBuilderOption{std::move(service)}};
            builder.SetOption(std::move(healthcheckOption));
        }
        for (const std::size_t i: std::views::iota(0ul, threads_amount_)) {
            grpc_contexts_.emplace_back(
                    std::make_unique<agrpc::GrpcContext>(builder.AddCompletionQueue()));
        }

        builder.AddListeningPort(
                       fmt::format("{}:{}", host, port),
                       grpc::InsecureServerCredentials())
                .RegisterService(&matrix_matrix_service_)
                .RegisterService(&matrix_vector_service_);

        if (service) {
            builder.RegisterService(&healthcheck_service_);
        }

        server_ = builder.BuildAndStart();

        auto* hservice = server_->GetHealthCheckService();

        hservice->SetServingStatus("MatrixMatrixDotProductService", true);
        hservice->SetServingStatus("MatrixVectorDotProductService", true);
    }

    shutdown_listener_ = std::make_unique<ShutdownListener>(*server_, *grpc_contexts_.front());
    for (const std::size_t i: std::views::iota(0ul, threads_amount_)) {
        auto thread_pool_ex = thread_pool_.executor();
        net::post(thread_pool_ex, [&, i]() {
            auto& grpc_context = *grpc_contexts_[i];
            grpc_context.run();
        });
    }
}

MatrixMultiplyerServer::~MatrixMultiplyerServer() {
    thread_pool_.join();
    server_->Shutdown();
}