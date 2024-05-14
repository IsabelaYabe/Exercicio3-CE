#include "queue/queue.hpp"
#include "utils/watchdog.hpp"
#include "utils/file_processor.hpp"
#include "handler/parser.hpp"
#include "triggers/request_trigger.hpp"
#include "http/http_server.hpp"


int main() {
    boost::asio::io_context ioc{1};
    Queue<std::string> fileQueue;
    Queue<std::string> parserQueue;
    std::vector<std::string> dirs = {"../../mock/ContaVerde", "../../mock/DataCat"};
    Watchdog watchdog(dirs, fileQueue);
    FileProcessor processor(fileQueue, parserQueue);
    Parser parser(parserQueue, 4);
    RequestTrigger trigger(parserQueue);
    http_server server(ioc, 8080, trigger);
    ioc.run();
    

    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

    }
    parser.stop();
}