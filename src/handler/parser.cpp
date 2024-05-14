#include "handler/parser.hpp"

Parser::Parser(Queue<std::string>& queue, int numThreads)
    : m_queue(queue), m_numThreads(numThreads), m_running(false) {
        start();
    }

Parser::~Parser() {
    stop();
}

void Parser::start() {
    m_running = true;
    for (int i = 0; i < m_numThreads; ++i) {
        m_threads.emplace_back(&Parser::consume, this);
    }
}

/**
 * @brief Para o thread de processamento das requisições.
 *
 * Este método sinaliza para o thread parar de processar ao definir `m_running` como falso
 * e junta-se ao thread se ele for juntável, garantindo que todos os recursos sejam limpos adequadamente.
 */
void Parser::stop() {
    m_running = false; // Signal all threads to stop
    for (auto &thread : m_threads) {
        if (thread.joinable()) {
            thread.join(); // Wait for all threads to finish
        }
    }
}

void Parser::consume() {
    while (m_running || !m_queue.empty()) {
        auto data = m_queue.dequeue();
        if (data) {
            parse(data.value()); // Handle the data
        } else {
            std::this_thread::yield(); // Queue is empty, yield to reduce CPU usage
        }
    }

}


std::string Parser::detectFormat(const std::string& data) {
        for (char ch : data) {
            switch (ch) {
                case '{':
                    return "json";
                case ',':
                    return "csv";
                case '|':
                    return "log";
            }
        }
        return "unknown";
    }

void Parser::parse(const std::string& data) {
        if (data.empty()) {
            std::cout << "Received empty data to parse." << std::endl;
            return;
        }

        DataFrame df;
        //std::cout << "Processing data: " << data << std::endl;
        
        // Detect format of the data
        std::string format = detectFormat(data);
        
        // Call the appropriate processor based on the format
        if (format == "json") {
            df = processJson(data);
        } else if (format == "csv") {
            df = processCsvData(data);
        } //else if (format == "log") {
            //df = processLogs(data);
        //} 
        else {
            std::cout << "Unknown or unsupported data format." << std::endl;
        }

        std::cout << "Processing data: " << format << ":" << std::endl << data << std::endl;
        df.printDataFrame();

    }