#ifndef MATRIX_SERVER_GENERIC_SERVER_HTTP_HPP
#define MATRIX_SERVER_GENERIC_SERVER_HTTP_HPP

#include "concepts.hpp"
#include "matrix_server/request_handler_impl.hpp"
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/socket_base.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core/bind_handler.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/fields.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/system/detail/error_code.hpp>
#include <memory>
#include <utility>

namespace net = boost::asio;
namespace beast = boost::beast;
namespace sys = boost::system;

namespace generic {

    using tcp = net::ip::tcp;

    class SessionBase {
    protected:
        using HttpRequest = http::request<http::string_body, http::fields>;
        using Buffer = beast::flat_buffer;

    public:
        explicit SessionBase(tcp::socket&& socket) noexcept;

        net::awaitable<void> Run();

        SessionBase& operator=(const SessionBase&) = delete;
        SessionBase(const SessionBase&) = delete;

    protected:
        Buffer buffer_;
        HttpRequest request_;

        virtual void HandleRequest(HttpRequest&& request) = 0;
    };

    template<http::RequestHandlerConcept RequestHandler>
    class Session final : public SessionBase, public std::enable_shared_from_this<Session<RequestHandler>> {
    public:
        Session(tcp::socket&& socket, RequestHandler&& handler)
            : SessionBase(std::move(socket)), request_handler_{std::forward<RequestHandler>(handler)} {
        }

    private:
        RequestHandler request_handler_;

        void HandleRequest(HttpRequest&& request) override;
    };

    template<http::RequestHandlerConcept RequestHandler>
    class Listener final : public std::enable_shared_from_this<Listener<RequestHandler>> {
    public:
        template<typename Handler>
        Listener(net::io_context& io_context, const tcp::endpoint& endpoint, Handler&& request_handler)
            : io_context_{io_context},
              acceptor_{net::make_strand(io_context_)},
              request_handler_{std::forward<Handler>(request_handler)} {
            acceptor_.open(endpoint.protocol());
            acceptor_.set_option(net::socket_base::reuse_address(true));
            acceptor_.bind(endpoint);
            acceptor_.listen(net::socket_base::max_listen_connections);
        }

        net::awaitable<void> Run() {
            sys::error_code ec{};

            co_await acceptor_.async_accept(
                    net::make_strand(io_context_),
                    beast::bind_front_handler(&Listener::DoAccept, this->shared_from_this()));

            while (!ec) {
            }

            co_return;
        }

    private:
        net::io_context& io_context_;
        tcp::acceptor acceptor_;
        RequestHandler request_handler_;

        net::awaitable<void> DoAccept() {
        }

        net::awaitable<void> OnAccept(sys::error_code ec, tcp::socket socket) {
        }
    };

}// namespace generic

#endif