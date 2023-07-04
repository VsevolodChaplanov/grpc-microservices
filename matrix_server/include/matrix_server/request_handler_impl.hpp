#ifndef MATRIX_SERVER_REQUEST_HANDLER_IMPL_HPP
#define MATRIX_SERVER_REQUEST_HANDLER_IMPL_HPP

#include "matrix_server/matrix_matrix_service_impl.hpp"
#include "matrix_server/matrix_vector_service_impl.hpp"
#include <agrpc/grpc_context.hpp>
#include <agrpc/repeatedly_request.hpp>
#include <tuple>


class RequestHandlerImpl final {
public:
    
    RequestHandlerImpl(agrpc::GrpcContext& context, 
        MatrixMatrixServiceImpl& matrix_matrix_service,
        MatrixVectorServiceImpl& matrix_vector_service);


private:
    agrpc::GrpcContext& context_;
    MatrixMatrixServiceImpl& matrix_matrix_service_;
    MatrixVectorServiceImpl& matrix_vector_service_;
};

#endif
