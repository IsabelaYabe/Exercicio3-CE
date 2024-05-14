#ifndef DATAFRAMEBUFFERMANAGER_HPP
#define DATAFRAMEBUFFERMANAGER_HPP

#include "DataFrame.h"
#include <unordered_map>
#include <memory>
#include <mutex>

/**
 * @class DataFrameBufferManager
 * @brief Gerencia buffers para armazenar objetos DataFrame de forma segura em ambientes multithread.
 *
 * Esta classe usa um std::unordered_map para armazenar buffers nomeados de DataFrames. Cada operação
 * sobre o mapa é protegida por um mutex para garantir que as operações sejam thread-safe.
 */
class DataFrameBufferManager {
private:
    std::unordered_map<std::string, std::shared_ptr<DataFrame>> dataFrameBuffers;
    mutable std::mutex bufferMutex;

public:
    /**
    * @brief Atualiza ou insere um novo DataFrame no buffer.
    * @param bufferName Nome do buffer para atualizar ou criar.
    * @param newDataFrame DataFrame a ser inserido ou atualizado no buffer.
    *
    * A operação é protegida por um mutex para evitar condições de corrida em um ambiente multithread.
    */
    void updateBuffer(const std::string& bufferName, std::shared_ptr<DataFrame> newDataFrame) {
        std::lock_guard<std::mutex> lock(bufferMutex);
        dataFrameBuffers[bufferName] = newDataFrame;
    }

    /**
    * @brief Recupera um DataFrame de um buffer, se ele existir.
    * @param bufferName Nome do buffer a ser acessado.
    * @return Um ponteiro compartilhado para o DataFrame, ou nullptr se o buffer não existir.
    *
    * A busca é protegida por um mutex para garantir thread-safety durante o acesso ao mapa.
    */
    std::shared_ptr<DataFrame> getBuffer(const std::string& bufferName) const {
        std::lock_guard<std::mutex> lock(bufferMutex);
        auto it = dataFrameBuffers.find(bufferName);
        if (it != dataFrameBuffers.end()) {
            return it->second;
        }
        return nullptr;
    }

    /**
    * @brief Limpa um buffer especificado, removendo o DataFrame associado.
    * @param bufferName Nome do buffer a ser limpo.
    *
    * Esta operação também é protegida por um mutex para manter a segurança em ambientes multithread.
    */
    void clearBuffer(const std::string& bufferName) {
        std::lock_guard<std::mutex> lock(bufferMutex);
        dataFrameBuffers.erase(bufferName);
    }
};

#endif // DATAFRAMEBUFFERMANAGER_HPP
