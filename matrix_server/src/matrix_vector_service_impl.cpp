#include "matrix_server/matrix_vector_service_impl.hpp"
#include "mv_multiply.pb.h"
#include <range/v3/numeric/inner_product.hpp>
#include <ranges>

grpc::Status MatrixVectorServiceImpl::Multiply(grpc::ServerContext* context,
                                               const mms::MatrixVectorDotRequest* request,
                                               mms::MatrixVectorDotReply* response) {
    const std::size_t matrix_row_ind = request->matrix_row_ind();

    const std::size_t row_size = request->matrix_row_element_size();
    const std::size_t rhs_size = request->vector_row_element_size();

    if (row_size != rhs_size) {
        return grpc::Status{
                grpc::StatusCode::INVALID_ARGUMENT,
                "Rhs and matrix row has different sizes"};
    }

    const auto& row_elements = request->matrix_row_element();
    const auto& column_elements = request->vector_row_element();

    auto result_value = ranges::inner_product(
            row_elements, column_elements, 0.);
    response->set_matrix_row_ind(static_cast<int>(matrix_row_ind));
    response->set_dot_result(result_value);

    return grpc::Status::OK;
}