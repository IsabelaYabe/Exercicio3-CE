#include "http/http_session.hpp"

http_session::http_session(tcp::socket socket, RequestTrigger& requestTrigger)
    : socket_(std::move(socket)), requestTrigger_(requestTrigger) {}


void http_session::start() {
    do_read();
}

void http_session::do_read() {
    auto self = shared_from_this();
    boost::beast::http::async_read(socket_, buffer_, request_,
        [self](beast::error_code ec, std::size_t bytes_transferred) {
            self->on_read(ec, bytes_transferred);
        });
}


void http_session::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    if (!ec) {
        std::string requestData = request_.body();
        // parserQueue_.enqueue(request_.body()); // Assuming trigger handles the processing
        requestTrigger_.trigger(requestData);
        do_write();  // Simplified response for clarity
    } else {
        // Handle error, possibly logging or cleaning up resources
        std::cerr << "Error on read: " << ec.message() << std::endl;
    }
}


void http_session::do_write() {
    auto res = std::make_shared<boost::beast::http::response<boost::beast::http::string_body>>(
    boost::beast::http::status::ok, request_.version());
    res->set(boost::beast::http::field::server, "Boost Beast Simplified Server");
    res->set(boost::beast::http::field::content_type, "text/plain");
    res->keep_alive(request_.keep_alive());
    res->body() = "Request received successfully!";
    res->prepare_payload();

    boost::beast::http::async_write(socket_, *res,
        [self = shared_from_this(), res](beast::error_code ec, std::size_t) {
            if (ec == boost::beast::http::error::end_of_stream) {
                // The client closed the connection, do nothing
                return;
            }

            if (ec) {
                std::cerr << "Error: " << ec.message() << std::endl;
                return;
            }

            if (self->request_.keep_alive()) {
                self->do_read();
            }
        });
}
