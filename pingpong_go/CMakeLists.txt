set(GO_PROTOC_COMPILER ${Protobuf_PROTOC_EXECUTABLE})
set(GO_CLIENT go-math-client)

file(GLOB PROTO_SOURCE RELATIVE ${CMAKE_CURRENT_LIST_DIR} protofiles/*.proto)
file(GLOB GO_SOURCE main.go)

go_protobuf_generate(
    PROTO_SOURCES ${PROTO_SOURCE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    PROTOC_COMPILER ${GO_PROTOC_COMPILER})
go_mod_tidy()
add_go_executable(${GO_CLIENT} ${GO_SOURCE})
