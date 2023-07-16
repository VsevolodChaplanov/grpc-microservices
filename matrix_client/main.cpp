#include <agrpc/asio_grpc.hpp>
#include <agrpc/detail/high_level_client_sender.hpp>
#include <agrpc/grpc_context.hpp>
#include <agrpc/rpc.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <cstdint>
#include <exception>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <google/protobuf/repeated_field.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/support/status.h>
#include <mv_multiply.grpc.pb.h>
#include <mv_multiply.pb.h>
#include <range/v3/iterator/operations.hpp>
#include <range/v3/view/enumerate.hpp>
#include <ranges>
#include <string_view>
#include <vector>

namespace mms = matrix_distributed_computing;
namespace pbf = google::protobuf;

int main(int argc, const char* argv[]) try {
    constexpr std::size_t port = 50432;
    constexpr std::string_view host = "0.0.0.0";
    const auto address = fmt::format("{}:{}", host, port);

    grpc::Status status;

    mms::MatrixVectorDotProductService::Stub stub{
            grpc::CreateChannel(address, grpc::InsecureChannelCredentials())};
    agrpc::GrpcContext grpc_context;

    const std::vector<double> row{1, 2, 3};
    const std::vector<double> column{1, 2, 3};

    agrpc::asio::co_spawn(
            grpc_context,
            [&]() -> agrpc::asio::awaitable<void> {
                using RPC = agrpc::RPC<&mms::MatrixVectorDotProductService::Stub::PrepareAsyncMultiply>;

                grpc::ClientContext client_context;

                mms::MatrixVectorDotRequest request;

                request.set_matrix_row_ind(0);


                for (const auto elem: row) {
                    request.add_matrix_row_element(static_cast<float>(elem));
                }

                for (const auto elem: column) {
                    request.add_vector_row_element(static_cast<float>(elem));
                }

                mms::MatrixVectorDotReply response;

                status = co_await RPC::request(grpc_context, stub, client_context, request, response, boost::asio::use_awaitable);

                if (!status.ok()) {
                    std::cout << "Error: " << status.error_code()
                              << "; message: " << status.error_message()
                              << std::endl;

                    co_return;
                }

                std::cout << "Status: " << status.ok()
                          << "; index: " << response.matrix_row_ind()
                          << "; value: " << response.dot_result()
                          << std::endl;
            },
            boost::asio::detached);

    grpc_context.run();

} catch (const std::exception& ex) {
    fmt::println("error: {}", ex.what());
}