#include "RepositorySaver/RepositorySaver.hpp"
#include <sstream>
#include <iostream>

// Construtor: Inicializa a fila de entrada, o caminho do banco de dados e o nome da tabela.
RepositorySaver::RepositorySaver(Queue<DataFrame>& queue, const std::string& dbPath, const std::string& table)
    : inputQueue(queue), tableName(table) {
    // Abre a conexão com o banco de dados SQLite.
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Falha ao abrir o banco de dados: " + std::string(sqlite3_errmsg(db)));
    }
    std::cout << "Banco de dados aberto com sucesso.\n";
}

// Destruidor: Fecha a conexão com o banco de dados.
RepositorySaver::~RepositorySaver() {
    sqlite3_close(db);
    std::cout << "Banco de dados fechado.\n";
}

// Cria a tabela se ela não existir no banco de dados com base nos tipos fornecidos pelo DataFrame.
void RepositorySaver::createTableIfNotExists(const DataFrame& df) {
    std::string createTableSQL = "CREATE TABLE IF NOT EXISTS " + tableName + " (";
    const auto& types = df.getTypes();
    for (const auto& [colName, colType] : types) {
        createTableSQL += colName + " " + colType + ", ";
    }
    createTableSQL.pop_back(); // Remove o último espaço.
    createTableSQL.pop_back(); // Remove a última vírgula.
    createTableSQL += ");";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, createTableSQL.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error = "Erro SQLite durante a criação da tabela: " + std::string(errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
    std::cout << "Tabela " << tableName << " criada ou verificada com sucesso.\n";
}

// Constrói a query SQL de inserção de dados com suporte à atualização de registros existentes.
std::string RepositorySaver::buildInsertQuery(const DataFrame& df) const {
    if (df.getRows().empty()) {
        throw std::runtime_error("DataFrame está vazio, sem dados para inserir.");
    }

    std::stringstream sql;
    sql << "INSERT OR REPLACE INTO " << tableName << " (";
    const auto& columns = df.getColumns();
    for (const auto& col : columns) {
        sql << col << ", ";
    }
    sql.seekp(-2, std::ios_base::end);
    sql << ") VALUES ";

    for (const auto& rowPtr : df.getRows()) {
        sql << "(";
        for (const auto& col : columns) {
            auto valueVariant = rowPtr->getCol(col);
            std::string valueStr = std::visit([](auto&& arg) -> std::string {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::string>) {
                    return "'" + arg + "'";
                } else if constexpr (std::is_arithmetic_v<T>) {
                    return std::to_string(arg);
                } else {
                    throw std::runtime_error("Tipo de dado não suportado para inserção no SQL.");
                }
            }, valueVariant);
            sql << valueStr << ", ";
        }
        sql.seekp(-2, std::ios_base::end);
        sql << "), ";
    }
    sql.seekp(-2, std::ios_base::end);
    sql << ";";
    
    return sql.str();
}

// Salva os DataFrames na fila para o banco de dados.
void RepositorySaver::saveToDatabase() {
    while (!inputQueue.empty()) {
        DataFrame df = inputQueue.pop();
        createTableIfNotExists(df); // Garante a existência da tabela.
        std::string sql = buildInsertQuery(df);
        std::cout << "Tentando executar SQL: " << sql << "\n";
        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::cerr << "Erro SQLite: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        } else {
            std::cout << "Dados inseridos com sucesso.\n";
        }
    }
}
