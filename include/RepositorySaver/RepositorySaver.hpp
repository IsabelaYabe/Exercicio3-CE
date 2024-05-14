#ifndef REPOSITORYSAVER_HPP
#define REPOSITORYSAVER_HPP

#include "../DataFrame/DataFrame.h"
#include "../queue/queue.hpp"
#include <sqlite3.h>
#include <string>

// Classe RepositorySaver gerencia a persistência de dados de DataFrames em um banco de dados SQLite.
class RepositorySaver {
protected:
    Queue<DataFrame>& inputQueue;  // Fila de DataFrames para serem salvos no banco de dados.
    sqlite3* db;                   // Ponteiro para o banco de dados SQLite.
    std::string tableName;         // Nome da tabela onde os dados serão inseridos.

    // Cria a tabela no banco de dados se ela não existir.
    void createTableIfNotExists(const DataFrame& df);

    // Constrói uma query SQL para inserir os dados de um DataFrame na tabela.
    std::string buildInsertQuery(const DataFrame& df) const;

public:
    // Construtor: inicializa a fila de entrada, o caminho do banco e o nome da tabela.
    RepositorySaver(Queue<DataFrame>& queue, const std::string& dbPath, const std::string& table);

    // Destruidor: fecha a conexão com o banco de dados.
    ~RepositorySaver();

    // Executa a inserção dos DataFrames da fila no banco de dados.
    void saveToDatabase();
};

#endif // REPOSITORYSAVER_HPP
