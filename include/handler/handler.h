#ifndef HANDLER_H
#define HANDLER_H

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include "math.h"
#include "../queue/Queue.hpp"
#include "../dataframe/dataframe.h"

using namespace std;

/**
 * @class Handler
 * @brief Classe base genérica para tratar objetos DataFrame.
 *
 * Esta classe é projetada para servir como base para diferentes tipos de manipulação
 * de DataFrames, gerenciando uma fila de DataFrames de entrada e múltiplas filas de saída.
 */
class Handler {
public:
    // Fila de DataFrames de entrada
    Queue<DataFrame*> *dataframes_in;
    // Vetor de queues de saída
    vector<Queue<DataFrame*>> *dataframes_out;


    /**
     * @brief Construtor que inicializa as filas de entrada e saída.
     * @param dataframes_in Ponteiro para a fila de DataFrames de entrada.
     * @param dataframes_out Ponteiro para o vetor de filas de saída.
     */
    Handler(Queue<DataFrame*> *dataframes_in, vector<Queue<DataFrame*>> *dataframes_out)
        : dataframes_in(dataframes_in), dataframes_out(dataframes_out) {}
};

/**
 * @class BalanceHandler
 * @brief Subclasse de Handler para balancear o processamento de DataFrames.
 *
 * Esta classe é responsável por distribuir a carga de trabalho de processamento
 * de DataFrames de maneira equilibrada entre várias threads ou processos.
 */
class BalanceHandler: public Handler {
    public:
        /**
        * @brief Construtor que inicializa as filas de entrada e saída.
        * @param dataframes_in Ponteiro para a fila de DataFrames de entrada.
        * @param dataframes_out Ponteiro para o vetor de filas de saída.
        */
        BalanceHandler(Queue<DataFrame*> *dataframes_in, vector<Queue<DataFrame*>> *dataframes_out): Handler(dataframes_in, dataframes_out){};
        /**
        * @brief Função que executa o balanceamento dos DataFrames.
        */
        void BalancerFunction();
    private:
        /**
         * @brief Processa um segmento específico de um DataFrame.
         * @param df DataFrame a ser processado.
         * @param start_index Índice inicial do segmento.
         * @param end_index Índice final do segmento.
         */
        void ProcessChunk(DataFrame df,int start_index, int end_index);
};

// Classe para filtrar o DataFrame por horário
class CleanCache: public Handler {
    public:
        // Construtor da classe
        CleanCache(Queue<DataFrame*> *dataframes_in, vector<Queue<DataFrame*>> *dataframes_out): Handler(dataframes_in, dataframes_out){};
        // Método para filtrar o DataFrame por horário
        void CleanCacheFunction();
};

// Classe para filtrar as ações do usuário e conectá-las com os produtos
class FilterHandler: public Handler {
    public:
        // Construtor da classe
        FilterHandler(Queue<DataFrame*> *dataframes_in, vector<Queue<DataFrame*>> *dataframes_out): Handler(dataframes_in, dataframes_out){};
        // Método para filtrar as ações do usuário e conectá-las com os produtos
        void FilterFunction(DataFrame df_products);
};


#endif // HANDLER_H
