#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <queue>
#include <mutex>

#include "semaphore.h"

using namespace std;

template <typename T>
class Queue {
    private:
        // Fila de DataFrames 
        queue<T> queue_dataframe;
        // Semáforo para indicar se a fila está vazia
        Semaphore emptySemaphore;
        // Semáforo para indicar se a fila está cheia
        Semaphore fullSemaphore;
        
    public:
        // Construtor da fila com tamanho máximo 
        explicit Queue(int max_size = 10): emptySemaphore(max_size,max_size), fullSemaphore(0,max_size) {};
        // Mutex para controlar o acesso à fila
        mutex mtx;
        // Método para adicionar um DataFrame na fila
        void push(T dataframe){
            emptySemaphore.wait();
            mtx.lock();
            queue_dataframe.push(dataframe);
            mtx.unlock();   
            fullSemaphore.notify();
        };
        // Método para remover um DataFrame da fila
        T pop(){
            fullSemaphore.wait();
            mtx.lock();
            T dataframe = queue_dataframe.front();
            queue_dataframe.pop();
            mtx.unlock();
            emptySemaphore.notify();
            return dataframe;
        };
        // Método para verificar se a fila está vazia
        bool empty(){
            return queue_dataframe.empty();
        };
        // Método para obter o tamanho da fila
        int size(){
            return queue_dataframe.size();
        };
};
#endif //QUEUE_H