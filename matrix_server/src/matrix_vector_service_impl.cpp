#include "matrix_server/matrix_vector_service_impl.hpp"
#include "mv_multiply.pb.h"
#include <cstdint>
#include <grpcpp/support/status.h>
#include <range/v3/numeric/inner_product.hpp>
#include <ranges>

grpc::Status MatrixVectorServiceImpl::Multiply(grpc::ServerContext* context,
                                               const mms::MatrixVectorDotRequest* request,
                                               mms::MatrixVectorDotReply* response) {
    const std::size_t matrix_row_ind = request->matrix_row_ind();

    const std::size_t row_size = request->matrix_row_element_size();
    const std::size_t rhs_size = request->vector_row_element_size();

    if (row_size != rhs_size) {
        response->set_matrix_row_ind(static_cast<std::int32_t>(matrix_row_ind));
        response->set_dot_result(0.);
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


grpc::Status MatrixVectorAsyncServiceImpl::Multiply(::grpc::ServerContext* server_context,
                                                    const mms::MatrixVectorDotRequest* request,
                                                    mms::MatrixVectorDotReply* response) {

    const std::size_t matrix_row_index = request->matrix_row_ind();

    const std::size_t matrix_row_size = request->matrix_row_element_size();
    const std::size_t vector_column_size = request->vector_row_element_size();

    if (matrix_row_size != vector_column_size) {
        return grpc::Status{
                grpc::StatusCode::INVALID_ARGUMENT,
                "Sizes of matrix row and vector are different"};
    }

    const auto& row_elements_repeater = request->matrix_row_element();
    const auto& vector_elements_repeater = request->vector_row_element();

    const auto result_porduct = ranges::inner_product(
            row_elements_repeater, vector_elements_repeater, 0.);

    response->set_dot_result(result_porduct);
    response->set_matrix_row_ind(static_cast<std::int32_t>(matrix_row_index));

    return grpc::Status::OK;
}
