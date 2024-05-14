#ifndef ROW_H
#define ROW_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <variant>
#include "Time.h"

using namespace std;


/**
 * @class Row
 * @brief Classe para representar e manipular linhas de dados variáveis.
 *
 * Esta classe permite criar e manipular linhas de dados, onde cada coluna pode
 * conter diferentes tipos de dados (int, double, string, vector<int>, Time). Fornece
 * métodos para adicionar, remover, acessar e imprimir os dados da linha.
 */
using RowVariant = variant< int, double, string, vector<int>, Time>;

class Row {
    private:
        unordered_map<string, RowVariant> row;  ///< Mapa para armazenar dados de cada coluna.
    public:
    // Construtores
    Row() {}
    Row(const Row&) = default;
    Row(Row&&) = default;

    /**
     * @brief Construtor que inicializa uma linha com chaves e valores correspondentes.
     * @param keys Vetor de strings contendo as chaves.
     * @param values Vetor de RowVariant contendo os valores.
     */
    Row(const vector<string>& keys, const vector<RowVariant>& values) {
        if (keys.size() != values.size()) {
            throw invalid_argument("Number of keys must match number of values.");
        }
        for (size_t i = 0; i < keys.size(); ++i) {
            row[keys[i]] = values[i];
        }
    }
        
    /**
     * @brief Adiciona uma coluna com o nome e valor especificados.
     * @param name Nome da coluna.
     * @param value Valor da coluna.
     */
    void addColRow(const string& name, const RowVariant& value) {
            row[name] = value;
    }

    /**
     * @brief Retorna o número de colunas na linha.
     * @return Tamanho da linha.
     */
    size_t size(){
        return row.size();
    }


    /**
     * @brief Remove uma coluna pelo nome.
     * @param name Nome da coluna a ser removida.
     */
    void removeCol(const string& name) {
        auto it = row.find(name);
        if (it == row.end()) {
            throw invalid_argument("Column not found");
        }
        row.erase(it);
    }

    /**
     * @brief Acessa o valor de uma coluna pelo nome.
     * @param name Nome da coluna.
     * @return Referência para o valor da coluna.
     */
    RowVariant& getCol(const string& name) {
        auto it = row.find(name);
        if (it == row.end()) {
            throw invalid_argument("Column not found");
        }
        return it->second;
    }

    /**
     * @brief Retorna uma referência constante ao mapa de dados.
     * @return O mapa de colunas.
     */
    const unordered_map<string, RowVariant>& getData() const {
        return row;
    }

    /**
     * @brief Imprime os tipos de dados de cada coluna.
     */
    void printTypes() const {
        for (const auto& col : row) {
            cout << col.first << " type: " << visit([](auto&& arg) -> string {
                return typeid(decltype(arg)).name();
            }, col.second) << endl;
        }
    }

    /**
     * @brief Imprime a linha com os valores formatados de cada coluna.
     */
    void printRow() const {
        for (const auto& [key, value] : row) {
            cout << key << ": ";
            visit([](auto&& arg) {
                using T = decay_t<decltype(arg)>;
                if constexpr (is_same_v<T, int> || is_same_v<T, double> || is_same_v<T, vector<int>>) {
                    if constexpr (is_same_v<T, vector<int>>) {
                        for (int v : arg) cout << v << " ";
                    } else {
                        cout << arg;
                    }
                } else if constexpr (is_same_v<T, string>) {
                    cout << '"' << arg << '"';
                } else if constexpr (is_same_v<T, Time>) {
                    arg.print();
                }
                cout << " ";
            }, value);
        }
        cout << endl;
    }

    /**
     * @brief Verifica se uma coluna específica existe.
     * @param name Nome da coluna.
     * @return Verdadeiro se a coluna existe, falso caso contrário.
     */
    string getTypeCol(const string& name) {
        auto it = row.find(name);
        if (it == row.end()) {
            throw invalid_argument("Column not found");
        }
        return visit([](auto&& arg) -> string {
            using T = decay_t<decltype(arg)>;
            return typeid(T).name();
        }, it->second);
    }

    /**
     * @brief Verifica se uma coluna específica existe na linha.
     * @param name Nome da coluna a ser verificada.
     * @return true se a coluna existe, false caso contrário.
     */
    bool containsColumn(const string& name) const {
        return row.find(name) != row.end();
    }

    /**
     * @brief Retorna o valor de uma coluna especificada, se ela existir.
     * @param name Nome da coluna.
     * @return Valor da coluna como RowVariant.
     * @throws invalid_argument se a coluna especificada não for encontrada.
     */
    RowVariant getCol_(const string& name) const {
        auto it = row.find(name);
        if (it == row.end()) {
            throw invalid_argument("Column not found");
        }
        return it->second;
    }


    /**
     * @brief Mescla duas linhas baseando-se em uma coluna específica que deve ter valores iguais nas duas linhas.
     * @param otherRow Outra instância de Row que será mesclada com esta.
     * @param mergeColumn O nome da coluna usada para verificar a igualdade dos valores como base para a mesclagem.
     * @return true se a mesclagem for bem-sucedida, false se os valores da coluna de mesclagem não corresponderem.
     * @throws invalid_argument se a coluna de mesclagem não for encontrada em uma ou ambas as linhas.
     * 
     * Este método verifica se ambas as linhas têm uma coluna com o mesmo nome e valores idênticos. Se verdadeiro,
     * as colunas da outra linha que não estão presentes nesta linha são adicionadas. Este método garante que 
     * a linha resultante contenha todas as colunas das duas linhas originais, com prioridade para os valores 
     * desta linha em caso de sobreposição.
     */
    bool mergeRows(const Row& otherRow, const string& mergeColumn) {
        if (!containsColumn(mergeColumn) || !otherRow.containsColumn(mergeColumn)) {
            throw invalid_argument("Merge column not found in one or both rows");
        }
    
        if (getCol_(mergeColumn) != otherRow.getCol_(mergeColumn)) {
            throw invalid_argument("Merge column values do not match");
        }
        // Adicione colunas de otherRow que não estão nesta linha.
        for (const auto& [key, value] : otherRow.getData()) {
            if (!containsColumn(key) && !containsColumn(key)) {
                addColRow(key, value);
            }
        }
        return true;
    }
  

    };

#endif // ROW_H