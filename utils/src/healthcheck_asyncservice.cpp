#include <agrpc/rpc.hpp>
#include <atomic>
#include <boost/asio/detached.hpp>
#include <fmt/format.h>
#include <grpcpp/impl/codegen/call_op_set.h>
#include <grpcpp/support/status.h>
#include <healthcheck.pb.h>
#include <mutex>
#include <pthread.h>
#include <utils/healthcheck/healthcheck_asyncservice.hpp>

namespace util::healthcheck {

    grpc::Status HealthCheckAsyncServiceImpl::Check(grpc::ServerContext* context,
                                                    const HealthCheckRequest* request,
                                                    HealthCheckResponse* response) {
        std::lock_guard lock{mutex_};

        const std::string& service_name = request->service();

        if (auto iter = status_map_.find(service_name); iter != status_map_.cend()) {
            response->set_status(iter->second);
            return grpc::Status::OK;
        }

        return grpc::Status{grpc::StatusCode::NOT_FOUND,
                            fmt::format("Service {} not found", service_name)};
    }

    grpc::Status HealthCheckAsyncServiceImpl::Watch(grpc::ServerContext* context,
                                                    const HealthCheckRequest* request,
                                                    grpc::ServerWriter<HealthCheckResponse>* writer) {
        auto last_state = HealthCheckResponse::UNKNOWN;
        while (!context->IsCancelled()) {
            {
                std::lock_guard lock{mutex_};
                HealthCheckResponse response;

                const auto& service_name = request->service();

                if (auto iter = status_map_.find(service_name); iter != status_map_.cend()) {
                    response.set_status(HealthCheckResponse::SERVICE_UNKNOWN);
                } else {
                    response.set_status(iter->second);
                }

                if (response.status() != last_state) {
                    writer->Write(response);
                    last_state = response.status();
                }
            }

            gpr_sleep_until(gpr_time_add(gpr_now(GPR_CLOCK_MONOTONIC),
                                         gpr_time_from_millis(1000, GPR_TIMESPAN)));
        }

        return grpc::Status::OK;
    }

    void HealthCheckAsyncServiceImpl::SetStatus(const std::string& service_name,
                                                HealthCheckResponse::ServingStatus status) {
        std::lock_guard lock{mutex_};

        if (shutdown_.load(std::memory_order_acquire)) {
            status = HealthCheckResponse::NOT_SERVING;
        }

        status_map_.emplace(service_name, status);
    }

    void HealthCheckAsyncServiceImpl::SetAll(HealthCheckResponse::ServingStatus status) {
        std::lock_guard lock{mutex_};

        if (shutdown_.load(std::memory_order_acquire)) {
            return;
        }

        for (auto& pair: status_map_) {
            pair.second = status;
        }
    }

    void HealthCheckAsyncServiceImpl::Shutdown() {
        std::lock_guard lock{mutex_};

        if (shutdown_.load(std::memory_order_acquire)) {
            return;
        }

        shutdown_.store(false, std::memory_order_release);

        for (auto& pair: status_map_) {
            pair.second = HealthCheckResponse::NOT_SERVING;
        }
    }


    CustomHealthCheckService::CustomHealthCheckService(HealthCheckAsyncServiceImpl* impl) noexcept
        : impl_{impl} {}

    void CustomHealthCheckService::SetServingStatus(const std::string& service_name, bool serving) {
        impl_->SetStatus(service_name, serving ? HealthCheckResponse::SERVING : HealthCheckResponse::NOT_SERVING);
    }

    void CustomHealthCheckService::SetServingStatus(bool serving) {
        impl_->SetAll(serving ? HealthCheckResponse::SERVING : HealthCheckResponse::NOT_SERVING);
    }

    void CustomHealthCheckService::Shutdown() {
        impl_->Shutdown();
    }
}// namespace util::healthcheck