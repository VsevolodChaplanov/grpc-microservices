#ifndef GRPC_PINGPONG_PINGPONG_SERVER_PINGPONG_HPP
#define GRPC_PINGPONG_PINGPONG_SERVER_PINGPONG_HPP

#include "protofiles/ping-pong.grpc.pb.h"
#include <boost/asio.hpp>
#include <fmt/core.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_context.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/support/status.h>
#include <memory>
#include <protofiles/ping-pong.pb.h>
#include <string_view>
#include <fmt/format.h>
// #include <agrpc/asio-grpc.hpp>

namespace net = boost::asio;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using mathtest::MathTest;
using mathtest::MathRequest;
using mathtest::MathReply;

class MathTestImpl final : public MathTest::Service {
public:
    Status sendRequest(ServerContext* context, const MathRequest* request, MathReply* reply) noexcept override {
        int a = request->a();
        int b = request->b();

        reply->set_result(a * b);

        return Status::OK;
    }
};

void Run(std::string_view host, std::size_t port) try {
    const auto address = fmt::format("{}:{}", host, port);

    MathTestImpl service;

    ServerBuilder builder;

    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server{builder.BuildAndStart()};
    fmt::print("Listen of port {}", port);

    server->Wait();
} catch (...) { }

#endif