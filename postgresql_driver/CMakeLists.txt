set(POSTGRES_DRIVER postgres-driver-lib CACHE INTERNAL "")
set(POSTGRES_DRIVER_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include CACHE INTERNAL "")

file(GLOB POSTGRES_DRIVER_HEADERS ${POSTGRES_DRIVER_INCLUDE_DIR}/postgres/*.hpp)
file(GLOB POSTGRES_DRIVER_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_library(${POSTGRES_DRIVER} STATIC
    ${POSTGRES_DRIVER_HEADERS}
    ${POSTGRES_DRIVER_SOURCES}
)
target_include_directories(${POSTGRES_DRIVER} PUBLIC
    libpq::pq
    libpqxx::pqxx
    boost::boost
    ${POSTGRES_DRIVER_INCLUDE_DIR}
)
target_link_libraries(${POSTGRES_DRIVER} PUBLIC
    libpq::pq
    libpqxx::pqxx
    boost::boost
)
target_link_options(${POSTGRES_DRIVER} PUBLIC -fsanitize=address)

file(GLOB POSTGRES_DRIVER_TESTS_SOURCES tests/*.cpp)
add_executable(${POSTGRES_DRIVER}_tests ${POSTGRES_DRIVER_TESTS_SOURCES})
target_include_directories(${POSTGRES_DRIVER}_tests PRIVATE
    ${POSTGRES_DRIVER_INCLUDE_DIR}
    libpqxx::pqxx
    Catch2::Catch2
)
target_link_libraries(${POSTGRES_DRIVER}_tests PRIVATE
    ${POSTGRES_DRIVER}
    libpqxx::pqxx
    Catch2::Catch2
)
target_link_options(${POSTGRES_DRIVER}_tests PUBLIC -fsanitize=address)
catch_discover_tests(${POSTGRES_DRIVER}_tests)
