set(PINGPONG pingpong-service)
set(PINGPONG_PROTOBJ pingpong-service-proto)
set(PINGPONG_SERVICE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(PINGPONG_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include)
set(PINGPONG_SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(PINGPONG_PROTOFILES_DIR ${CMAKE_CURRENT_LIST_DIR}/)
set(PINGPONG_PROTO_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

file(GLOB PINGPONG_PROTOFILES ${PINGPONG_PROTOFILES_DIR}/protofiles/*.proto)
file(GLOB PINGPONG_HEADERS ${PINGPONG_INCLUDE_DIRS}/pingpong/*.hpp)
file(GLOB PINGPONG_SOURCES ${PINGPONG_SOURCES_DIR}/*.cpp)

find_package(Boost "1.81" REQUIRED)
find_package(Threads REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf "3.21.9" CONFIG REQUIRED)
find_package(asio-grpc CONFIG REQUIRED)
find_package(fmt "10" CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

asio_grpc_protobuf_generate(
    GENERATE_GRPC 
    GENERATE_MOCK_CODE
    OUT_VAR ASIO_GRPC_PINGPONGPROTO_SOURCES
    OUT_DIR ${PINGPONG_PROTO_BINARY_DIR}
    IMPORT_DIRS ${CMAKE_CURRENT_LIST_DIR}
    PROTOS ${PINGPONG_PROTOFILES})

add_library(${PINGPONG_PROTOBJ} OBJECT)
target_sources(${PINGPONG_PROTOBJ} PRIVATE ${ASIO_GRPC_PINGPONGPROTO_SOURCES})
target_link_libraries(${PINGPONG_PROTOBJ} PUBLIC protobuf::libprotobuf
    gRPC::grpc
)
target_include_directories(${PINGPONG_PROTOBJ} PUBLIC
    gRPC::grpc++
    gRPC::grpc
    absl::absl
    "$<BUILD_INTERFACE:${PINGPONG_PROTO_BINARY_DIR}>"
)

add_library(${PINGPONG} STATIC 
    ${PINGPONG_HEADERS}
    ${PINGPONG_SOURCES}
    ${PINGPONG_PROTOFILES}
    ${PINGPONG_PROTO_BINARY_DIR}
)
target_include_directories(${PINGPONG} PUBLIC
    boost::boost
    asio-grpc::asio-grpc
    fmt::fmt
    gRPC::grpc
    gRPC::grpc++
    abseil::abseil
    ${PINGPONG_INCLUDE_DIRS}
    ${PINGPONG_PROTO_BINARY_DIR}
)
target_link_libraries(${PINGPONG} PUBLIC
    Threads::Threads
    boost::boost
    fmt::fmt
    asio-grpc::asio-grpc
    protobuf::libprotobuf
    gRPC::grpc
    gRPC::grpc++
    abseil::abseil
    ${PINGPONG_PROTOBJ}
)

add_executable(${PINGPONG}_server server_main.cpp)
target_include_directories(${PINGPONG}_server  PUBLIC
    boost::boost
    asio-grpc::asio-grpc
    fmt::fmt
    gRPC::grpc
    gRPC::grpc++
    abseil::abseil
    ${PINGPONG_INCLUDE_DIRS}
    ${PINGPONG_PROTO_BINARY_DIR}
)
target_link_libraries(${PINGPONG}_server  PUBLIC
    Threads::Threads
    boost::boost
    fmt::fmt
    asio-grpc::asio-grpc
    protobuf::libprotobuf
    gRPC::grpc
    gRPC::grpc++
    abseil::abseil
    ${PINGPONG}
)

add_executable(${PINGPONG}_client client_main.cpp)
target_include_directories(${PINGPONG}_client  PUBLIC
    boost::boost
    asio-grpc::asio-grpc
    fmt::fmt
    gRPC::grpc
    gRPC::grpc++
    abseil::abseil
    ${PINGPONG_INCLUDE_DIRS}
    ${PINGPONG_PROTO_BINARY_DIR}
)
target_link_libraries(${PINGPONG}_client PUBLIC
    Threads::Threads
    boost::boost
    fmt::fmt
    asio-grpc::asio-grpc
    protobuf::libprotobuf
    gRPC::grpc
    gRPC::grpc++
    abseil::abseil
    ${PINGPONG}
)


# generate protobuf headers and sources
# add_library(${PINGPONG_PROTOBJ} OBJECT ${PINGPONG_PROTOFILES})
# target_link_libraries(${PINGPONG_PROTOBJ} PUBLIC 
#     CONAN_LIB::protobuf_protocd
#     CONAN_LIB::protobuf_protobufd
#     CONAN_LIB::grpc_grpc++
#     CONAN_LIB::grpc_grpc++_unsecure
#     CONAN_LIB::grpc_grpc
# )
# target_include_directories(${PINGPONG_PROTOBJ} PUBLIC 
#     $<BUILD_INTERFACE:${PINGPONG_PROTO_BINARY_DIR}>
#     CONAN_PKG::asio-grpc
#     CONAN_PKG::grpc
#     CONAN_PKG::abseil)
#     message(STATUS "PROTOFILES ${PINGPONG_PROTOFILES}")
# protobuf_generate(
#     TARGET ${PINGPONG_PROTOBJ}
#     OUT_VAR PINGPONG_PROTO_GENERATED_FILES
#     # IMPORT_DIRS ${PINGPONG_PROTOFILES_DIR}
#     PROTOC_OUT_DIR ${PINGPONG_PROTO_BINARY_DIR})
# protobuf_generate(
#     PROTOS ${PINGPONG_PROTIFILES}
#     TARGET ${PINGPONG_PROTOBJ}
#     OUT_VAR PINGPONG_PROTO_GENERATED_FILES
#     LANGUAGE grpc
#     GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
#     PLUGIN "protoc-gen-grpc=${CONAN_BIN_DIRS_GRPC}/grpc_cpp_plugin"
#     PLUGIN_OPTIONS "generate_mock_code=true"
#     # IMPORT_DIRS ${PINGPONG_PROTOFILES_DIR}
#     PROTOC_OUT_DIR ${PINGPONG_PROTO_BINARY_DIR})

# execute_process(
#     COMMAND 
#         protoc
#         ${PINGPONG_PROTOFILES}  
#         "--cpp_out=${PINGPONG_PROTO_BINARY_DIR}"
#     WORKING_DIRECTORY ${PINGPONG_PROTOFILES_DIR}
# )





# asio_grpc_protobuf_generate(
#     PROTOS ${PINGPONG_PROTOFILES}
#     OUT_DIR ${PINGPONG_PROTO_BINARY_DIR}
#     OUT_VAR PINGPONG_GENERATED_PROTOFILES
#     TARGET ${PINGPONG_PROTOBJ}
#     IMPORT_DIRS ${PINGPONG_PROTOFILES_DIR}
#     # GENERATE_GRPC
#     # GENERATE_MOCK_CODE
# )

# asio_grpc_protobuf_generate(
#     GENERATE_GRPC 
#     GENERATE_MOCK_CODE
#     OUT_VAR PINGPONG_PROTO_GENERATED_FILES
#     USAGE_REQUIREMENT PUBLIC
#     IMPORT_DIRS ${PINGPONG_PROTOFILES_DIR}
#     TARGET ${PINGPONG_PROTOBJ}
#     OUT_DIR ${PINGPONG_PROTO_BINARY_DIR}
#     PROTOS ${PINGPONG_PROTIFILES})

# protobuf_generate(
#     TARGET ${PINGPONG_PROTIFILES}
#     OUT_VAR PINGPONG_PROTO_GENERATED_FILES
#     LANGUAGE grpc
#     PROTOS ${PINGPONG_PROTIFILES}
#     GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
#     PLUGIN "protoc-gen-grpc=\$<TARGET_FILE:CONAN_LIB::grpc_grpc_plugin_support>" #gRPC::grpc_cpp_plugin
#     PLUGIN_OPTIONS "generate_mock_code=true"
#     IMPORT_DIRS ${PINGPONG_PROTOFILES_DIR}
#     PROTOC_OUT_DIR "${PINGPONG_PROTO_BINARY_DIR}")