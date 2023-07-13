#ifndef POSTGRESQL_DRIVER_CONNECTION_POOL_HPP
#define POSTGRESQL_DRIVER_CONNECTION_POOL_HPP

#include "connection_factory_concept.hpp"
#include <algorithm>
#include <condition_variable>
#include <coroutine>
#include <cstddef>
#include <execution>
#include <memory>
#include <mutex>
#include <pqxx/connection>
#include <string_view>
#include <type_traits>
#include <utility>

namespace postgres {

    class ConnectionPool {
    private:
        using PoolType = ConnectionPool;
        using ConnectionPtr = std::shared_ptr<pqxx::connection>;

    public:
        class ConnectionWrapper {
        public:
            ConnectionWrapper(ConnectionPtr&& conn, PoolType& pool) noexcept;

            ConnectionWrapper(const ConnectionWrapper& conn_w) = delete;
            ConnectionWrapper& operator=(const ConnectionWrapper& conn_w) = delete;

            ConnectionWrapper(ConnectionWrapper&& conn_w) = default;
            ConnectionWrapper& operator=(ConnectionWrapper&& conn_w) = default;

            pqxx::connection& operator*() const& noexcept;
            pqxx::connection& operator*() const&& = delete;
            pqxx::connection* operator->() const& noexcept;

            ~ConnectionWrapper();

        private:
            ConnectionPtr conn_;
            PoolType* pool_;
        };

        ConnectionWrapper GetConnection();

        template<concepts::ConnectionFactoryConcept ConnectionFactory>
        ConnectionPool(size_t capacity, ConnectionFactory&& connection_factory) {
            pool_.reserve(capacity);
            std::generate_n(std::execution::par_unseq,
                            std::back_inserter(pool_),
                            capacity,
                            connection_factory);
        }

    private:
        void ReturnConnection(ConnectionPtr&& conn);

        std::mutex mutex_;
        std::condition_variable cond_var_;
        std::vector<ConnectionPtr> pool_;
        size_t used_connections_ = 0;
    };
}// namespace postgres

#endif//POSTGRESQL_DRIVER_CONNECTION_POOL_HPP