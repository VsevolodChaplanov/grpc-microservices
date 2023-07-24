#ifndef MATRIX_CLIENT_MATRIX_CLIENT_SEND_HEALTCHCHECK_REQUEST_HPP
#define MATRIX_CLIENT_MATRIX_CLIENT_SEND_HEALTCHCHECK_REQUEST_HPP

#include <agrpc/high_level_client.hpp>
#include <agrpc/repeatedly_request.hpp>
#include <agrpc/rpc.hpp>
#include <agrpc/rpc_type.hpp>
#include <agrpc/use_awaitable.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <chrono>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/status.h>
#include <healthcheck.grpc.pb.h>
#include <healthcheck.pb.h>
#include <memory>
#include <string>
#include <variant>

template<typename GRPCContext>
agrpc::asio::awaitable<grpc::Status> DoServiceCheck(const std::string& address,
                                                    const std::string& service_name,
                                                    GRPCContext& grpc_context,
                                                    grpc::ClientContext& client_ctx) {
    using namespace grpc::health::v1;
    using RPC = agrpc::RPC<&Health::Stub::PrepareAsyncCheck>;

    Health::Stub healthcheck_stub{grpc::CreateChannel(
            address, grpc::InsecureChannelCredentials())};

    HealthCheckRequest request;
    request.set_service("service_name");

    HealthCheckResponse response;

    co_return RPC::request(grpc_context, healthcheck_stub, client_ctx, request, response);
}

template<typename GRPCContext>
agrpc::asio::awaitable<grpc::Status> DoServiceWatch(const std::string& address,
                                                    const std::string& service_name,
                                                    GRPCContext& grpc_context,
                                                    grpc::ClientContext& client_ctx,
                                                    std::chrono::milliseconds time_interval) {
    using namespace grpc::health::v1;
    using RPC = agrpc::RPC<&Health::Stub::PrepareAsyncWatch>;

    Health::Stub stub{
            grpc::CreateChannel(address, grpc::InsecureChannelCredentials())};

    HealthCheckRequest request;
    request.set_service(service_name);

    HealthCheckResponse response;


    auto reader = stub.Watch(&client_ctx, request);

    do {
        auto service_status = co_await agrpc::read(reader, response);
        co_await time_interval;
    } while (HealthCheckResponse::SERVING == response.status());

    client_ctx.TryCancel();
}

#endif
