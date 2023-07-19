set(UTILS_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/ CACHE INTERNAL "")
set(UTILITY_LIB utility-lib CACHE INTERNAL "")

file(GLOB HEALTCHECK_UTILS_HPP ${UTILS_INCLUDE_DIR}/utils/healthcheck/*.hpp)
file(GLOB UTILS_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_library(${UTILITY_LIB} STATIC ${HEALTCHECK_UTILS_HPP} ${UTILS_SOURCES})
target_include_directories(${UTILITY_LIB} PUBLIC
    ${HEALTHCHECK_SERVICE_PROTOFILES_BINARY_DIR}
    ${UTILS_INCLUDE_DIR}
    boost::boost
    asio-grpc::asio-grpc
    gRPC::grpc++
    gtest::gtest
    fmt::fmt
)
target_link_libraries(${UTILITY_LIB} PUBLIC
    ${HEALTHCHECK_PROTOLIB}
    boost::boost
    asio-grpc::asio-grpc
    gRPC::grpc++
    gtest::gtest
    fmt::fmt
)