set(PROTOFILES_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(MATRIX_PROTOLIB matrix-service-protolib)
file(GLOB PROTOFILES ${CMAKE_CURRENT_LIST_DIR}/*.proto)

asio_grpc_protobuf_generate(
    GENERATE_GRPC
    GENERATE_MOCK_CODE
    OUT_VAR MATRIX_SERVICE_PROTOSOURCES
    OUT_DIR ${PROTOFILES_BINARY_DIR}
    IMPORT_DIRS ${CMAKE_CURRENT_LIST_DIR}
    PROTOS ${PROTOFILES}
)

add_library(${MATRIX_PROTOLIB} OBJECT)

target_sources(${MATRIX_PROTOLIB} PRIVATE ${MATRIX_SERVICE_PROTOSOURCES})

target_link_libraries(${MATRIX_PROTOLIB} PUBLIC 
    protobuf::libprotobuf
    gRPC::grpc
    gRPC::grpc++
    gtest::gtest
)

target_include_directories(${MATRIX_PROTOLIB} 
    SYSTEM PUBLIC 
    $<BUILD_INTERFACE:${PROTOFILES_BINARY_DIR}>    
)