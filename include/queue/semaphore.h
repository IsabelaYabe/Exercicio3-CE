#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;

/**
 * @class Semaphore
 * @brief Implementa um semáforo para controle de concorrência entre threads.
 *
 * Esta classe fornece uma implementação de semáforo que pode ser usada para controlar o acesso a recursos
 * compartilhados em um ambiente de programação multithreaded. Ela utiliza um contador para gerenciar quantas
 * threads podem acessar um recurso simultaneamente.
 */
class Semaphore {
    private:
        // Contador do semáforo
        int count;
        // Contador máximo do semáforo
        int max_count;
        // Mutex para controlar o acesso ao semáforo    
        mutex mtx;
        // Variável de condição para aceitar notificações
        condition_variable cv;
    public:
        
        /**
         * @brief Constrói um novo objeto Semaphore.
         * 
         * @param count Valor inicial do contador do semáforo.
         * @param max_count Valor máximo que o contador do semáforo pode atingir.
         */
        explicit Semaphore(int count = 0, int max_count = 1): count(count), max_count(max_count) {};
        
        /**
         * @brief Notifica o semáforo de que um recurso foi liberado.
         * 
         * Incrementa o contador do semáforo e notifica uma das threads que estão esperando, se houver.
         * Se o contador atinge o valor máximo, a thread que chama este método bloqueia até que um recurso seja consumido.
         */
        void notify(){
            unique_lock<mutex> lck(mtx);
            while (count == max_count){
                cv.wait(lck);
            }
            count++;
            cv.notify_one();
        };

        /**
         * @brief Aguarda até que um recurso esteja disponível.
         * 
         * Decrementa o contador do semáforo quando um recurso está disponível. Se nenhum recurso está disponível,
         * a thread que chama este método bloqueia até que o método notify() seja chamado por outra thread.
         */
        void wait(){
            unique_lock<mutex> lck(mtx);
            while (count == 0){
                cv.wait(lck);
            }
            count--;
        };
};

#endif //SEMAPHORE_H