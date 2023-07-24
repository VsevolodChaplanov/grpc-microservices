#ifndef MATRIX_CLIENT_MATRIX_CLIENT_MATRIX_MULTIPLYER_CLIENT_HPP
#define MATRIX_CLIENT_MATRIX_CLIENT_MATRIX_MULTIPLYER_CLIENT_HPP

#include <agrpc/asio_grpc.hpp>
#include <bits/ranges_algo.h>
#include <bits/ranges_base.h>
#include <boost/asio/awaitable.hpp>
#include <concepts>
#include <cstddef>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/support/status.h>
#include <mm_multiply.grpc.pb.h>
#include <mm_multiply.pb.h>
#include <string_view>
#include <utility>

namespace mms = matrix_distributed_computing;

struct MultiplyResult {
    double Result = 0;
    std::size_t IRow = 0;
    std::size_t ICol = 0;
    grpc::Status RPCStatus = grpc::Status::CANCELLED;
};

template<std::ranges::viewable_range Vector, typename GRPCContext, typename RPC = agrpc::RPC<&mms::MatrixMatrixDotProductService::Stub::PrepareAsyncMultiply>>
agrpc::asio::awaitable<MultiplyResult> MultiplyTwoVectors(const std::string& address,
                                                          GRPCContext& grpc_context,
                                                          grpc::ClientContext& client_ctx,
                                                          Vector&& matrix_row, Vector&& matrix_column,
                                                          std::size_t irow, std::size_t icol) {
    mms::MatrixMatrixDotRequest request;
    mms::MatrixMatrixDotProductService::Stub mms_stub{
            grpc::CreateChannel(address, grpc::InsecureChannelCredentials())};

    request.set_matrix_row_ind(static_cast<int>(irow));
    request.set_matrix_column_ind(static_cast<int>(icol));

    for (const auto& elem: matrix_row) {
        request.add_matrix_row_element(elem);
    }

    for (const auto& elem: matrix_column) {
        request.add_matrix_column_element(elem);
    }

    mms::MatrixMatrixDotReply response;

    grpc::Status status = co_await RPC::request(grpc_context, mms_stub, client_ctx, request, response);

    co_return MultiplyResult{
            .Result = response.dot_result(),
            .IRow = static_cast<std::size_t>(response.matrix_row_ind()),
            .ICol = static_cast<std::size_t>(response.matrix_column_ind()),
            .RPCStatus = status};
}

#endif