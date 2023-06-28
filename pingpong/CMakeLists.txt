add_subdirectory(protofiles)

set(PINGPONG pingpong-service)
set(PINGPONG_SERVICE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(PINGPONG_HEADERS ${PINGPONG}_HEADERS)
set(PINGPONG_SOURCES ${PINGPONG}_SRC)
set(PINGPONG_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include)
set(PINGPONG_SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

file(GLOB PINGPONG_HEADERS ${PINGPONG_INCLUDE_DIRS}/pingpong/*.hpp)
file(GLOB PINGPONG_SOURCES ${PINGPONG_SOURCES_DIR}/*.cpp)


add_library(${PINGPONG} STATIC 
    ${PINGPONG_HEADERS}
    ${PINGPONG_SOURCES}
)
target_include_directories(${PINGPONG} PUBLIC
    CONAN_PKG::boost
    CONAN_PKG::asio-grpc
    ${PINGPONG_INCLUDE_DIRS}
)
target_link_libraries(${PINGPONG} PUBLIC
    Threads::Threads
)

add_library(${PINGPONG}-so SHARED 
    ${PINGPONG_HEADERS}
    ${PINGPONG_SOURCES}
)
target_include_directories(${PINGPONG}-so PUBLIC
    CONAN_PKG::boost
    CONAN_PKG::asio-grpc
    ${PINGPONG_INCLUDE_DIRS}
)
target_link_libraries(${PINGPONG}-so PUBLIC
    Threads::Threads
)