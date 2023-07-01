#include <boost/version.hpp>
#include <iostream>
#include <boost/config.hpp>
#include <agrpc/asio_grpc.hpp>
#include <grpcpp/grpcpp.h>

int main(int argc, const char* argv[]) try {
    std::cout << "Boost version: " << BOOST_VERSION << std::endl;
    std::cout << "Hello world: " << std::endl;
    std::cout << "gRCP version: " << grpc::Version() << std::endl;

} catch (...) {
    std::cout << "Exception!" << std::endl;
}