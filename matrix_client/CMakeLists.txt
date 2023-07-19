set(MATRIX_CLIENT_LIB matrix-client-lib)
set(MATRIX_CLIENT matrix-client)
set(MATRIX_CLIENT_MAIN ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
set(MATRIX_CLIENT_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/)
file(GLOB MATRIX_CLIENT_INCLUDE ${MATRIX_CLIENT_INCLUDE_DIR}/matrix_client/*.hpp)
file(GLOB MATRIX_CLIENT_SRC ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_library(${MATRIX_CLIENT_LIB} STATIC
    ${MATRIX_CLIENT_INCLUDE} ${MATRIX_CLIENT_SRC}
)
target_include_directories(${MATRIX_CLIENT_LIB} PUBLIC
    ${MATRIX_CLIENT_INCLUDE_DIR}
    ${MATRIX_CLIENT_PROTOFILES_BINARY_DIR}
    asio-grpc::asio-grpc
    boost::boost
    fmt::fmt
    range-v3::range-v3
    ${HEALTHCHECK_SERVICE_PROTOFILES_BINARY_DIR}
)
target_link_libraries(${MATRIX_CLIENT_LIB} PUBLIC
    ${MATRIX_PROTOLIB}
    ${HEALTHCHECK_PROTOLIB}
    asio-grpc::asio-grpc
    boost::boost
    fmt::fmt
    range-v3::range-v3
)

add_executable(${MATRIX_CLIENT} ${MATRIX_CLIENT_MAIN})
target_include_directories(${MATRIX_CLIENT} PRIVATE
    ${MATRIX_CLIENT_INCLUDE_DIR}
    gRPC::grpc
    gRPC::grpc++
    gtest::gtest
)
target_link_libraries(${MATRIX_CLIENT} PRIVATE
    ${MATRIX_PROTOLIB}
    ${MATRIX_CLIENT_LIB}
    asio-grpc::asio-grpc
    gRPC::grpc
    gRPC::grpc++
    gtest::gtest
)