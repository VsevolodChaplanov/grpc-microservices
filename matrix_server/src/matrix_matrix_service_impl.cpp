#include "matrix_server/matrix_matrix_service_impl.hpp"
#include <cstdint>
#include <range/v3/view/zip.hpp>
#include <range/v3/numeric/inner_product.hpp>
#include <grpcpp/support/status.h>
#include <type_traits>

grpc::Status MatrixMatrixServiceImpl::Multiply(grpc::ServerContext* context,
                          const mms::MatrixMatrixDotRequest* request,
                          mms::MatrixMatrixDotReply* response) {
    const std::size_t matrix_column_ind = request->matrix_column_ind();
    const std::size_t matrix_row_ind = request->matrix_row_ind();

    const std::size_t row_size = request->matrix_row_element_size();
    const std::size_t column_size = request->matrix_column_element_size();

    if (row_size != column_size) {
        return grpc::Status{
            grpc::StatusCode::INVALID_ARGUMENT, 
            "Row and column has different sizes"
        };
    }

    const auto& row_elements = request->matrix_row_element();
    const auto& column_elements = request->matrix_column_element();

    auto result_value = ranges::inner_product(
        row_elements, column_elements, 0. 
    );
    response->set_matrix_column_ind(static_cast<int>(matrix_column_ind));
    response->set_matrix_row_ind(static_cast<int>(matrix_row_ind));
    response->set_dot_result(result_value);

    return grpc::Status::OK;
}