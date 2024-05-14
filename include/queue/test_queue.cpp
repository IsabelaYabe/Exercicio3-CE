#include "queue.hpp"
#include "../DataFrame/DataFrame.h"

template<typename T>
Queue<T>* createQueue(DataFrame df) {
    Queue<T> *queue = new Queue<T>();
    queue->enqueue(df); // Correção: Use -> para chamar métodos em um ponteiro
    return queue;
}

int main() {
    // Exemplo de uso da função para criar uma fila 
    DataFrame df;
    DataFrame df2;
    Queue<DataFrame> *myQueue = createQueue<DataFrame>(df);
    myQueue->enqueue(df2);
    cout<<myQueue->size()<<endl;

    // Lembre-se de liberar a memória alocada para o ponteiro
    delete myQueue;

    return 0;
}
