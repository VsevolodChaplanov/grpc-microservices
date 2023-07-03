function(go_protobuf_generate)
    include(CMakeParseArguments)

    cmake_parse_arguments(
        PARSED_ARGS
        ""
        "PROTOC_COMPILER;WORKING_DIRECTORY"
        "PROTO_SOURCES"
        ${ARGN}
    )
    # note: if it remains unparsed arguments, here, they can be found in variable PARSED_ARGS_UNPARSED_ARGUMENTS
    if(NOT PARSED_ARGS_PROTO_SOURCES)
        message(FATAL_ERROR "You must provide a protobuf files list to generate pb and grpc go files")
    endif(NOT PARSED_ARGS_PROTO_SOURCES)

    if(NOT PARSED_ARGS_WORKING_DIRECTORY)
        message(FATAL_ERROR "You must provide a working directory to generate pb and grpc go files")
    endif(NOT PARSED_ARGS_WORKING_DIRECTORY)

    if(NOT PARSED_ARGS_PROTOC_COMPILER)
        message(FATAL_ERROR "You must provide a protoc compiler path to generate pb and grpc go files")
    endif(NOT PARSED_ARGS_PROTOC_COMPILER)

    execute_process(
        COMMAND ${PARSED_ARGS_PROTOC_COMPILER} --go_out=. --go_opt=paths=source_relative --go-grpc_out=. --go-grpc_opt=paths=source_relative ${PARSED_ARGS_PROTO_SOURCES}
        WORKING_DIRECTORY ${PARSED_ARGS_WORKING_DIRECTORY}
        COMMAND_ECHO STDOUT
        ERROR_VARIABLE GENERATION_ERR
    )    

    if(${GENERATION_ERR})
        message(STATUS "go_protobuf_generate error: ${GENERATION_ERR}")
    endif()
endfunction(go_protobuf_generate)

