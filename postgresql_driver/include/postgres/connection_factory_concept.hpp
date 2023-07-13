#ifndef POSTGRESQL_DRIVER_POSTGRES_CONNECTION_FACTORY_CONCEPT
#define POSTGRESQL_DRIVER_POSTGRES_CONNECTION_FACTORY_CONCEPT

#include <concepts>
#include <memory>
#include <pqxx/connection>

namespace postgres::concepts {

    template<typename Factory>
    concept ConnectionFactoryConcept = requires(Factory f) {
        { f() } -> std::convertible_to<std::shared_ptr<pqxx::connection>>;
    };
}

#endif//POSTGRESQL_DRIVER_POSTGRES_CONNECTION_FACTORY_CONCEPT