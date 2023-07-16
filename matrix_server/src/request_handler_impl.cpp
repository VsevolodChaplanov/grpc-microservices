#include "matrix_server/request_handler_impl.hpp"
#include "matrix_server/matrix_matrix_service_impl.hpp"
#include "mm_multiply.grpc.pb.h"
#include "mm_multiply.pb.h"
#include "mv_multiply.grpc.pb.h"
#include "mv_multiply.pb.h"
#include <agrpc/asio_grpc.hpp>
#include <agrpc/repeatedly_request.hpp>
#include <agrpc/rpc.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/co_spawn.hpp>
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
}


void RequestHandlerImpl::handle() {
}