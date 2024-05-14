#include "http/http_server.hpp"
#include "http/http_session.hpp"

http_server::http_server(boost::asio::io_context& ioc, short port, RequestTrigger& trigger)
    : acceptor_(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), requestTrigger_(trigger) {
    do_accept();
}

void http_server::do_accept() {
    acceptor_.async_accept(
        [this](boost::beast::error_code ec, boost::asio::ip::tcp::socket socket) {
            if (!ec) {
                // Pass both queue and threadPool to the session
                auto session = std::make_shared<http_session>(std::move(socket), requestTrigger_);
                session->start();
            }
            do_accept();
        });
}

