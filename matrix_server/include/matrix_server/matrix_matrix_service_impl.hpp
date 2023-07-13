#ifndef MATRIX_SERVER_MATRIX_MATRIX_SERVICE_IMPL_HPP
#define MATRIX_SERVER_MATRIX_MATRIX_SERVICE_IMPL_HPP

#include "mm_multiply.grpc.pb.h"

namespace mms = matrix_distributed_computing;

class MatrixMatrixServiceImpl final : public mms::MatrixMatrixDotProductService::Service {
public:
    grpc::Status Multiply(grpc::ServerContext* context,
                          const mms::MatrixMatrixDotRequest* request,
                          mms::MatrixMatrixDotReply* response) override;
    ~MatrixMatrixServiceImpl() override = default;
};

#endif
