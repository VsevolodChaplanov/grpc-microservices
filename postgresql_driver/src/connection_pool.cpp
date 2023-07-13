#include "postgres/connection_pool.hpp"
#include <coroutine>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>

namespace postgres {
    ConnectionPool::ConnectionWrapper ConnectionPool::GetConnection() {
        std::unique_lock lock{mutex_};

        cond_var_.wait(lock, [this] {
            return used_connections_ < pool_.size();
        });

        return {std::move(pool_[used_connections_++]), *this};
    }

    void ConnectionPool::ReturnConnection(ConnectionPool::ConnectionPtr&& conn) {
        {
            std::lock_guard lock{mutex_};
            assert(used_connections_ != 0);
            pool_[--used_connections_] = std::move(conn);
        }

        cond_var_.notify_one();
    }

    ConnectionPool::ConnectionWrapper::ConnectionWrapper(ConnectionPtr&& conn, PoolType& pool) noexcept
        : conn_{conn},
          pool_{&pool} {}

    pqxx::connection& ConnectionPool::ConnectionWrapper::operator*() const& noexcept {
        return *conn_;
    }

    pqxx::connection* ConnectionPool::ConnectionWrapper::operator->() const& noexcept {
        return conn_.get();
    }

    ConnectionPool::ConnectionWrapper::~ConnectionWrapper() {
        if (conn_) {
            pool_->ReturnConnection(std::move(conn_));
        }
    }
}// namespace postgres
