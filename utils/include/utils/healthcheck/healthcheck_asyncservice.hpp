#ifndef UTILS_HEALTHCHECK_HEALTHCHECK_ASYNCSERVICE_HPP
#define UTILS_HEALTHCHECK_HEALTHCHECK_ASYNCSERVICE_HPP


#include <atomic>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/support/status.h>
#include <healthcheck.grpc.pb.h>
#include <healthcheck.pb.h>
#include <unordered_map>

namespace util::healthcheck {
    using namespace grpc::health::v1;

    class HealthCheckAsyncServiceImpl final : public Health::AsyncService {
        using ServiceName = std::string;
        using ServingStatus = HealthCheckResponse::ServingStatus;

    public:
        HealthCheckAsyncServiceImpl() = default;

        grpc::Status Check(grpc::ServerContext* context,
                           const HealthCheckRequest* request,
                           HealthCheckResponse* response) override;
        grpc::Status Watch(grpc::ServerContext* context,
                           const HealthCheckRequest* request,
                           grpc::ServerWriter<HealthCheckResponse>* writer) override;

        void SetStatus(const std::string& service_name,
                       HealthCheckResponse::ServingStatus status);

        void SetAll(HealthCheckResponse::ServingStatus status);

        void Shutdown();

        ~HealthCheckAsyncServiceImpl() override = default;

    private:
        std::mutex mutex_;
        std::atomic_bool shutdown_ = false;
        std::unordered_map<ServiceName, ServingStatus> status_map_;
    };

    class CustomHealthCheckService final : public grpc::HealthCheckServiceInterface {
    public:
        explicit CustomHealthCheckService(HealthCheckAsyncServiceImpl* impl) noexcept;

        void SetServingStatus(const std::string& service_name, bool serving) override;

        void SetServingStatus(bool serving) override;

        void Shutdown() override;

        ~CustomHealthCheckService() override = default;

    private:
        HealthCheckAsyncServiceImpl* impl_;
    };

}// namespace util::healthcheck

#endif//UTILS_HEALTHCHECK_HEALTHCHECK_ASYNCSERVICE_HPP