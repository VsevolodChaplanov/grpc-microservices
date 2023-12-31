#ifndef MATRIX_SERVER_MATRIX_VECTOR_SERVICE_IMPL_HPP
#define MATRIX_SERVER_MATRIX_VECTOR_SERVICE_IMPL_HPP


#include "mm_multiply.pb.h"
#include "mv_multiply.grpc.pb.h"
#include <cstdint>
#include <grpcpp/support/status.h>
#include <range/v3/numeric/inner_product.hpp>

namespace mms = matrix_distributed_computing;

class MatrixVectorServiceImpl final : public mms::MatrixVectorDotProductService::Service {
public:
    grpc::Status Multiply(grpc::ServerContext* context,
                          const mms::MatrixVectorDotRequest* request,
                          mms::MatrixVectorDotReply* response) override;
    ~MatrixVectorServiceImpl() override = default;
};


class MatrixVectorAsyncServiceImpl final : public mms::MatrixVectorDotProductService::AsyncService {
public:
    ::grpc::Status Multiply(::grpc::ServerContext* server_context,
                            const mms::MatrixVectorDotRequest* request,
                            mms::MatrixVectorDotReply* response) override;

    ~MatrixVectorAsyncServiceImpl() override = default;
};

#endif
