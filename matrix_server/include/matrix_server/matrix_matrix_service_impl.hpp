#ifndef MATRIX_SERVER_MATRIX_MATRIX_SERVICE_IMPL_HPP
#define MATRIX_SERVER_MATRIX_MATRIX_SERVICE_IMPL_HPP

#include "mm_multiply.grpc.pb.h"
#include <cstddef>
#include <grpcpp/support/status.h>
#include <range/v3/numeric/inner_product.hpp>

namespace mms = matrix_distributed_computing;

class MatrixMatrixServiceImpl final : public mms::MatrixMatrixDotProductService::Service {
public:
    grpc::Status Multiply(grpc::ServerContext* context,
                          const mms::MatrixMatrixDotRequest* request,
                          mms::MatrixMatrixDotReply* response) override;
    ~MatrixMatrixServiceImpl() override = default;
};

class MatrixMatrixAsyncServiceImpl final : public mms::MatrixMatrixDotProductService::AsyncService {
public:
    ::grpc::Status Multiply(::grpc::ServerContext* server_context,
                            const mms::MatrixMatrixDotRequest* request,
                            mms::MatrixMatrixDotReply* response) override;

    ~MatrixMatrixAsyncServiceImpl() override = default;
};

#endif
