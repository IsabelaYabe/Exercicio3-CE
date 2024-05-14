#ifndef HTTP_SERVER_HPP  
#define HTTP_SERVER_HPP

#include <boost/asio.hpp>  
#include <boost/beast/core.hpp>  
#include <boost/beast/http.hpp>  
#include "http_session.hpp"  
#include "queue/queue.hpp"
#include "triggers/request_trigger.hpp"

namespace http = boost::beast::http;

class http_server {  
public:  
    http_server(boost::asio::io_context& ioc, short port, RequestTrigger& trigger);

private:  
    boost::asio::ip::tcp::acceptor acceptor_;
    RequestTrigger& requestTrigger_;
    void do_accept();  
};

#endif // HTTP_SERVER_HPP