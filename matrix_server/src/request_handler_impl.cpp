#include "matrix_server/request_handler_impl.hpp"
#include "mm_multiply.grpc.pb.h"
#include "mm_multiply.pb.h"
#include "mv_multiply.grpc.pb.h"
#include "mv_multiply.pb.h"
#include <agrpc/repeatedly_request.hpp>
#include <agrpc/rpc.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/bind_executor.hpp>
#include <fmt/core.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/async_unary_call.h>

using namespace matrix_distributed_computing;
namespace net = boost::asio;

RequestHandlerImpl::RequestHandlerImpl(agrpc::GrpcContext& context,
                                       MatrixMatrixServiceImpl& matrix_matrix_service,
                                       MatrixVectorServiceImpl& matrix_vector_service)
    : context_{context},
      matrix_matrix_service_{matrix_matrix_service},
      matrix_vector_service_{matrix_vector_service} {
    fmt::print("Initializing listener for matrix vector multiplyer service");

    agrpc::repeatedly_request(
            &MatrixVectorDotProductService::AsyncService::RequestMultiply,
            matrix_vector_service_,
            net::bind_executor(
                    context_,
                    [&](grpc::ServerContext& server_context, MatrixVectorDotRequest& request,
                        grpc::ServerAsyncResponseWriter<MatrixVectorDotReply>& writer) -> net::awaitable<void> {
                        MatrixVectorDotReply response;
                        auto status = matrix_vector_service_.Multiply(&server_context, &request, &response);
                        co_await agrpc::finish(writer, response, status);
                    }));

    agrpc::repeatedly_request(
            &MatrixMatrixDotProductService::AsyncService::RequestMultiply,
            matrix_matrix_service_,
            net::bind_executor(
                    context_,
                    [&](grpc::ServerContext& server_context, MatrixMatrixDotRequest& request,
                        grpc::ServerAsyncResponseWriter<MatrixMatrixDotReply>& writer) -> net::awaitable<void> {
                        MatrixMatrixDotReply response;
                        auto status = matrix_matrix_service_.Multiply(&server_context, &request, &response);
                        co_await agrpc::finish(writer, response, status);
                    }));

    fmt::print("Initializing listener for matrix matrix multiplyer service");
}
