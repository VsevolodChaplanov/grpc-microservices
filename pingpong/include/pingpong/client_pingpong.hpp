#ifndef GRPC_PINGPONG_PINGPONG_CLIENT_PINGPONG_HPP
#define GRPC_PINGPONG_PINGPONG_CLIENT_PINGPONG_HPP

#include "protofiles/ping-pong.pb.h"
#include <cstddef>
#include <fmt/core.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/channel.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/support/status.h>
#include <memory>
#include <string>
#include <protofiles/ping-pong.grpc.pb.h>
#include <string_view>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using mathtest::MathTest;
using mathtest::MathRequest;
using mathtest::MathReply;

class MathTestClientImpl final {
public:
    explicit MathTestClientImpl(std::shared_ptr<Channel> channel)
        : stub_{MathTest::NewStub(channel)} { }

    int sendRequest(int a, int b) const {
        MathRequest request;

        request.set_a(a);
        request.set_b(b);

        MathReply reply;

        ClientContext context;
        
        const Status status = stub_->sendRequest(&context, request, &reply);

        if (status.ok()) {
            return reply.result();
        } else {
            fmt::print("error: {} : {}", 
                static_cast<int>(status.error_code()), 
                status.error_message());
            return -1;
        }
    }

private:
    std::unique_ptr<MathTest::Stub> stub_;
};

void Run(std::string_view host, std::size_t port) try {
    const auto address = fmt::format("{}:{}", host, port);
    MathTestClientImpl client{
        grpc::CreateChannel(
            address, 
            grpc::InsecureChannelCredentials())
    };

    int a = 5;
    int b = 10;

    const auto response = client.sendRequest(a, b);

    fmt::print("Answer is: {} * {} = {}", a, b, response);
} catch(...) { }

#endif