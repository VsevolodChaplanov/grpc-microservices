#ifndef MATRIX_SERVER_SERVER_HPP
#define MATRIX_SERVER_SERVER_HPP

#include "matrix_matrix_service_impl.hpp"
#include "matrix_vector_service_impl.hpp"
#include "shutdown_signal_listener.hpp"
#include "utils/healthcheck/healthcheck_asyncservice.hpp"
#include <agrpc/grpc_context.hpp>
#include <boost/asio/thread_pool.hpp>
#include <grpcpp/server.h>
#include <memory>
#include <string_view>
#include <thread>
#include <vector>

namespace net = boost::asio;
namespace mms = matrix_distributed_computing;

class MatrixMultiplyerServer final {
public:
    MatrixMultiplyerServer(std::string_view host, std::size_t port,
                           std::unique_ptr<grpc::HealthCheckServiceInterface> service,
                           std::size_t concurrency_hint = std::thread::hardware_concurrency());

    ~MatrixMultiplyerServer();

private:
    const std::size_t threads_amount_;
    MatrixMatrixServiceImpl matrix_matrix_service_;
    MatrixVectorServiceImpl matrix_vector_service_;
    util::healthcheck::HealthCheckAsyncServiceImpl healthcheck_service_;
    std::vector<std::unique_ptr<agrpc::GrpcContext>> grpc_contexts_;
    boost::asio::thread_pool thread_pool_;
    std::unique_ptr<grpc::Server> server_;
    std::unique_ptr<ShutdownListener> shutdown_listener_;
};

#endif
