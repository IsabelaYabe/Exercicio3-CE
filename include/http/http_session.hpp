#ifndef HTTP_SESSION_HPP
#define HTTP_SESSION_HPP

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "queue/queue.hpp"
#include "triggers/request_trigger.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;  
using tcp = net::ip::tcp;

class http_session : public std::enable_shared_from_this<http_session> {
public:
    // Constructor with all necessary components
    explicit http_session(tcp::socket socket, RequestTrigger& requestTrigger);
    void start();

private:
    boost::asio::ip::tcp::socket socket_;
    boost::beast::flat_buffer buffer_; 
    boost::beast::http::request<boost::beast::http::string_body> request_;
    RequestTrigger& requestTrigger_;

    std::string body_to_string(const beast::http::request<beast::http::dynamic_body>& req);
    void do_read();
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void process_request();
    void do_write();
};

#endif // HTTP_SESSION_HPP
