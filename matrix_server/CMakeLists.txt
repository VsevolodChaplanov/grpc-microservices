set(MATRIX_SERVER_LIB matrix-server-lib)
set(MATRIX_SERVER matrix-server)
set(MATRIX_SERVER_MAIN ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
set(MATRIX_SERVER_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/)
file(GLOB MATRIX_SERVER_INCLUDE ${MATRIX_SERVER_INCLUDE_DIR}/matrix_server/*.hpp)
file(GLOB MATRIX_SERVER_SRC ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_library(${MATRIX_SERVER_LIB} STATIC
    ${MATRIX_SERVER_INCLUDE} ${MATRIX_SERVER_SRC}
)
target_include_directories(${MATRIX_SERVER_LIB} PUBLIC
    ${MATRIX_SERVER_INCLUDE_DIR}
    ${MATRIX_SERVER_PROTOFILES_BINARY_DIR}
    ${HEALTHCHECK_SERVICE_PROTOFILES_BINARY_DIR}
    ${UTILS_INCLUDE_DIR}
    asio-grpc::asio-grpc
    boost::boost
    fmt::fmt
    range-v3::range-v3
)
target_link_libraries(${MATRIX_SERVER_LIB} PUBLIC
    ${MATRIX_PROTOLIB}
    ${HEALTHCHECK_PROTOLIB}
    ${UTILITY_LIB}
    asio-grpc::asio-grpc
    boost::boost
    fmt::fmt
    range-v3::range-v3
)

add_executable(${MATRIX_SERVER} ${MATRIX_SERVER_MAIN})
target_include_directories(${MATRIX_SERVER} PRIVATE
    ${MATRIX_SERVER_INCLUDE_DIR}
    gRPC::grpc
    gRPC::grpc++
    gtest::gtest
)
target_link_libraries(${MATRIX_SERVER} PRIVATE
    ${MATRIX_PROTOLIB}
    ${MATRIX_SERVER_LIB}
    asio-grpc::asio-grpc
    gRPC::grpc
    gRPC::grpc++
    gtest::gtest
)