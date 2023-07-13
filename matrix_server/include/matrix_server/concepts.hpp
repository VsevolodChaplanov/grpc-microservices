#ifndef MATRIX_SERVER_CONCEPTS_HPP
#define MATRIX_SERVER_CONCEPTS_HPP

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <concepts>
#include <functional>
#include <type_traits>

namespace http {

    namespace {

        using request = boost::beast::http::request<
                boost::beast::http::string_body,
                boost::beast::http::fields>;
        using response = boost::beast::http::response<
                boost::beast::http::string_body,
                boost::beast::http::fields>;

    }// namespace

    template<typename Handler>
    concept RequestHandlerConcept = std::is_nothrow_copy_constructible_v<Handler> &&
                                    std::is_nothrow_move_constructible_v<Handler> &&
                                    std::invocable<Handler, request, std::function<response&&>>;

}// namespace http

#endif