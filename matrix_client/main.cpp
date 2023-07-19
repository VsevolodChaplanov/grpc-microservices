#include <agrpc/asio_grpc.hpp>
#include <agrpc/detail/high_level_client_sender.hpp>
#include <agrpc/grpc_context.hpp>
#include <agrpc/rpc.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/stubs/port.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/support/status.h>
#include <healthcheck.grpc.pb.h>
#include <healthcheck.pb.h>
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

    grpc::EnableDefaultHealthCheckService(true);

    grpc::Status status;

    mms::MatrixVectorDotProductService::Stub stub{
            grpc::CreateChannel(address, grpc::InsecureChannelCredentials())};
    agrpc::GrpcContext grpc_context;

    const std::vector<double> row{1, 2, 3};
    const std::vector<double> column{1, 2, 3};


    auto grpc_strand = boost::asio::make_strand(grpc_context);


    agrpc::asio::co_spawn(
            grpc_strand,
            [&]() -> agrpc::asio::awaitable<void> {
                using namespace grpc::health::v1;
                using RPC = agrpc::RPC<&Health::Stub::PrepareAsyncCheck>;

                grpc::ClientContext client_ctx;
                Health::Stub healthcheck_stub{grpc::CreateChannel(
                        address, grpc::InsecureChannelCredentials())};

                HealthCheckRequest request;
                request.set_service("MatrixMatrixDotProductService");

                HealthCheckResponse response;

                const auto status = co_await RPC::request(
                        grpc_context, healthcheck_stub, client_ctx, request, response);

                fmt::print("Response: {}\n", static_cast<int>(response.status()));

                if (!status.ok()) {
                    fmt::print("Error: {}, message {}, code {}, MatrixVectorDotProductService don't serve\n",
                               status.ok(), status.error_message(),
                               static_cast<int>(status.error_code()));
                }

                co_return;
            },
            boost::asio::detached);


    agrpc::asio::co_spawn(
            grpc_strand,
            [&]() -> agrpc::asio::awaitable<void> {
                using namespace grpc::health::v1;
                using RPC = agrpc::RPC<&Health::Stub::PrepareAsyncCheck>;

                grpc::ClientContext client_ctx;
                Health::Stub healthcheck_stub{grpc::CreateChannel(
                        address, grpc::InsecureChannelCredentials())};

                HealthCheckRequest request;
                request.set_service("MatrixVectorDotProductService");

                HealthCheckResponse response;

                const auto status = co_await RPC::request(
                        grpc_context, healthcheck_stub, client_ctx, request, response, boost::asio::use_awaitable);

                if (!status.ok()) {
                    fmt::print("Error: {}, code {}, MatrixVectorDotProductService don't serve\n",
                               status.ok(), static_cast<int>(status.error_code()));
                }

                co_return;
            },
            boost::asio::detached);

    for (const std::size_t _: std::views::iota(0, 100)) {
        auto coro_func = [&]() -> agrpc::asio::awaitable<void> {
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
        };

        agrpc::asio::co_spawn(grpc_context, coro_func, boost::asio::detached);
        agrpc::asio::co_spawn(grpc_context, coro_func, boost::asio::detached);
    }
    grpc_context.run();

} catch (const std::exception& ex) {
    fmt::println("error: {}", ex.what());
}