#include <iostream>
#include <vector>
#include <memory>
#include "../DataFrame/DataFrame.h"
#include "../queue/Queue.hpp"
#include "../RepositorySaver/RepositorySaver.hpp"

using namespace std;

int main() {
    std::string dbPath = "test.db";
    std::string tableName = "Products";

    // Criar a fila de DataFrames
    Queue<DataFrame> queue;

    // Criar DataFrames e adicionar à fila
    std::vector<std::string> columns = {"ID", "Quantidade"};
    std::vector<std::string> colTypes = {"int", "int"};

    DataFrame df1(columns, colTypes);
    auto row1 = std::make_shared<Row>();
    row1->addColRow("ID", 1);
    row1->addColRow("Quantidade", 100);
    df1.insertRow(row1);

    auto row2 = std::make_shared<Row>();
    row2->addColRow("ID", 2);
    row2->addColRow("Quantidade", 200);
    df1.insertRow(row2);

    auto row3 = std::make_shared<Row>();
    row3->addColRow("ID", 3);
    row3->addColRow("Quantidade", 300);
    df1.insertRow(row3);

    auto row4 = std::make_shared<Row>();
    row4->addColRow("ID", 4);
    row4->addColRow("Quantidade", 400);
    df1.insertRow(row4);

    DataFrame df2(columns, colTypes);
    auto row5 = std::make_shared<Row>();
    row5->addColRow("ID", 5);
    row5->addColRow("Quantidade", 500);
    df2.insertRow(row5);

    auto row6 = std::make_shared<Row>();
    row6->addColRow("ID", 6);
    row6->addColRow("Quantidade", 600);
    df2.insertRow(row6);

    auto row7 = std::make_shared<Row>();
    row7->addColRow("ID", 7);
    row7->addColRow("Quantidade", 700);
    df2.insertRow(row7);

    auto row8 = std::make_shared<Row>();
    row8->addColRow("ID", 8);
    row8->addColRow("Quantidade", 800);
    df2.insertRow(row8);


    queue.push(df1);
    queue.push(df2);

    // Criar o RepositorySaver
    RepositorySaver saver(queue, dbPath, tableName);

    // Simular o processo de salvar os DataFrames na fila para o banco de dados
    while (!queue.empty()) {
        saver.saveToDatabase();
    }

    return 0;
}

// Para compilar este código, você pode usar o seguinte comando:
// g++ -o testRepository include/RepositorySaver/testRepository.cpp src/RepositorySaver/RepositorySaver.cpp -Iinclude -lsqlite3
