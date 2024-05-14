#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <typeinfo>
#include <typeindex>
#include <map>

#include "Row.h"

using namespace std;


/**
 * @class DataFrame
 * @brief Classe para representar e manipular uma coleção de dados organizados em colunas.
 *
 * O DataFrame é uma estrutura de dados bidimensional, semelhante a uma tabela, que contém uma
 * coleção de colunas. Cada coluna pode ser de um tipo diferente de dado (int, double, string, etc.),
 * e é identificada por um nome. Esta classe fornece métodos para manipulação e consulta dos dados.
 */
class DataFrame {
private:
    vector<string> columns;     
    unordered_map<string, string> types;    
    vector<shared_ptr<Row>> rows;   

public:
    // Construtor padrão
    DataFrame() {}


    /**
     * @brief Construtor para inicializar com nomes de colunas e tipos.
     * @param colNames Vetor contendo os nomes das colunas.
     * @param colTypes Vetor contendo os tipos das colunas.
     */
    DataFrame(const vector<string>& colNames, const vector<string>& colTypes) {
        if (colNames.size() != colTypes.size()) {
            throw invalid_argument("Number of column names must match number of column types.");
        }

        columns = colNames;
        for (size_t i = 0; i < columns.size(); ++i) {
            types[columns[i]] = colTypes[i];
        }
    }
    
    // Getters 
    const vector<string>& getColumns() const { return columns; }
    const unordered_map<string, string>& getTypes() const { return types; }
    const vector<shared_ptr<Row>>& getRows() const{ return rows; }
    size_t getRowSize() const {return rows.size();}
    size_t getColumnSize() const {return columns.size();}
    size_t getRowCount() const {return rows.size();}
    size_t getColumnCount() const {return columns.size();}
    const shared_ptr<Row>& getRow(int index) const {
        if (index < 0 || index >= static_cast<int>(rows.size())) throw out_of_range("Index out of range");
        return rows[index];
    }
    
    // Setters
    void setColumns(const vector<string>& newColumns) { columns = newColumns;}
    void setRows(const vector<shared_ptr<Row>>& newRows) {rows = newRows;}
    void setTypes(const vector<string>& newTypes) { 
        if (columns.size() != newTypes.size()) {
            throw invalid_argument("Number of column names must match number of column types.");
        }
        for (size_t i = 0; i < columns.size(); ++i) {
            types[columns[i]] = newTypes[i];
        }
    }

    /**
    * @brief Verifica se uma coluna específica existe no DataFrame.
    * @param name Nome da coluna a ser verificada.
    * @return true se a coluna existir, false caso contrário.
    */
    bool containsColumn(const string& name) const {
        return find(columns.begin(), columns.end(), name) != columns.end();
    }

    /**
    * @brief Valida se uma linha (Row) é compatível com o DataFrame.
    * Verifica se todos os campos da linha estão presentes nas colunas do DataFrame e se os tipos de dados são compatíveis.
    * @param row A linha a ser validada.
    * @return true se a linha for compatível, false caso contrário.
    */
    bool validateRow(const Row& row) const {
        // Verifica se todos os campos da Row estão nas colunas do DataFrame e se os tipos são compatíveis
        const auto& row_data = row.getData(); // Supõe que há um método getData() que retorna a map da Row

        for (const auto& [key, value] : row_data) {
            auto col_it = types.find(key);
            if (col_it == types.end()) {
                cout << "Column '" << key << "' not found in DataFrame." << endl;
                return false;
            }

            // Verifica o tipo usando type_index
            const type_index typeIndexDataframe(typeid(value));
            const type_index typeIndexRow(typeid(decltype(value))); 

            if (typeIndexDataframe != typeIndexRow) {
                cout << "Type mismatch for column '" << key << "'. Expected " << col_it->second << " but found " << typeid(value).name() << endl;
                return false;
            }
        }

        return true;
    }
    /**
    * @brief Insere uma nova linha no DataFrame após validar sua compatibilidade.
    * @param row A linha a ser inserida, encapsulada em um ponteiro inteligente.
    */
    void insertRow(const shared_ptr<Row>& row) {
       try {
            if (!validateRow(*row)) {
                throw invalid_argument("Row is not compatible with DataFrame.");
            }
        } catch (const invalid_argument& e) {
            cerr << "Error: " << e.what() << endl;
            return;
        }
        rows.push_back(row);
    }
    
    /**
    * @brief Remove uma linha do DataFrame com base em seu índice.
    * @param index O índice da linha a ser removida.
    * @throws out_of_range se o índice estiver fora do intervalo válido.
    */
    void removeRow(int index) {
        if (index < 0 || index >= static_cast<int>(rows.size())) throw out_of_range("Index out of range");
        rows.erase(rows.begin() + index);
    }

    /**
    * @brief Mescla o conteúdo de outro DataFrame com este, se tiverem colunas e tipos idênticos.
    * @param other Outro DataFrame a ser mesclado.
    * @throws invalid_argument se os DataFrames tiverem colunas ou tipos diferentes e não puderem ser mesclados.
    */
    void merge(const DataFrame& other) {
        if (columns != other.columns || types != other.types) {
            throw invalid_argument("DataFrames have different columns or types and cannot be merged.");
        }
        rows.insert(rows.end(), other.rows.begin(), other.rows.end());
    }

    /**
    * @brief Exibe as primeiras 'n' linhas do DataFrame.
    *
    * Este método imprime as primeiras 'n' linhas do DataFrame, útil para uma visualização rápida dos dados iniciais.
    *
    * @param n Número de linhas a serem exibidas.
    */
    void head(int n) const {
        if (n > static_cast<int>(rows.size())) {
            n = rows.size(); // Limita n ao tamanho do vetor se for maior que o número de linhas
        }
        for (int i = 0; i < n; i++) {
            rows[i]->printRow();
        }
    }

    
    /**
    * @brief Exibe as últimas 'n' linhas do DataFrame.
    *
    * Este método imprime as últimas 'n' linhas do DataFrame, útil para uma visualização rápida dos dados mais recentes.
    *
    * @param n Número de linhas a serem exibidas.
    */
    void tail(int n) const {
        int start = max(0, static_cast<int>(rows.size()) - n);
        for (int i = start; i < static_cast<int>(rows.size()); i++) {
            rows[i]->printRow();
        }
    }

    /**
    * @brief Imprime os tipos de dados das colunas do primeiro Row, se houver.
    *
    * Este método é útil para verificar os tipos de dados das colunas contidas no DataFrame.
    */
    void printTypes() const {
        if (!rows.empty()) {
            rows[0]->printTypes(); 
        }
    }

    /**
    * @brief Imprime detalhes completos do DataFrame, incluindo os nomes das colunas seguidos por todas as linhas.
    *
    * Este método fornece uma maneira fácil de visualizar todo o conteúdo do DataFrame de forma textual,
    * incluindo cabeçalhos de colunas e valores de linha.
    */
    void printDataFrame() const {
        for (const auto& col : columns) {
            cout << col << " ";
        }
        cout << endl;

        for (const auto& row : rows) {
            row->printRow();
        }
    }

    /**
    * @brief Realiza uma operação de junção interna (inner join) entre este DataFrame e outro, com base em uma coluna especificada.
    *
    * O método `innerJoin` combina linhas de dois DataFrames baseado no valor de uma coluna em comum (join column). 
    * Apenas as linhas que possuem um valor correspondente na coluna especificada nos dois DataFrames são mantidas.
    * Caso uma linha deste DataFrame não encontre uma linha correspondente no outro DataFrame, ela é removida.
    *
    * @param other O outro DataFrame com o qual realizar a junção.
    * @param joinColumn A coluna base para a junção. Esta coluna deve existir em ambos os DataFrames.
    *
    * @throws invalid_argument Se a coluna especificada não estiver presente em ambos os DataFrames.
    *
    * @note O método modifica o DataFrame atual, removendo as linhas que não têm correspondentes no outro DataFrame.
    * 
    * Exemplo de uso:
    * ```
    * DataFrame df1, df2;
    * // Supõe que df1 e df2 foram preenchidos com dados e ambos contêm a coluna "ID"
    * df1.innerJoin(df2, "ID");
    * // Agora df1 contém apenas as linhas que têm IDs correspondentes em df2
    * ```
    *
    * Considerações:
    * - Este método não adiciona novas colunas ao DataFrame original, apenas verifica correspondências e remove as linhas não correspondentes.
    * - A operação é realizada in-place, modificando diretamente o DataFrame que chama o método.
    * - Deve-se ter cuidado com a ordem dos DataFrames na operação de junção, pois afetará quais linhas são consideradas para remoção.
    */
    void innerJoin(DataFrame& other, const string& joinColumn) {
        // Verifica se a coluna especificada existe em ambos os DataFrames
        if (!containsColumn(joinColumn) || !other.containsColumn(joinColumn)) {
            throw invalid_argument("Join column must be present in both DataFrames.");
        }

        vector<int> rowsToRemove;

        // Itera sobre cada linha do DataFrame atual
        for (int i = 0; i < static_cast<int>(rows.size()); ++i) {
            bool matchFound = false;// Flag para verificar se encontrou uma correspondência

            // Itera sobre cada linha do outro DataFrame
            for (const auto& otherRow : other.rows) {
                try {
                    // Tenta mesclar as linhas com base na coluna de junção
                    // mergeRows retorna true se as linhas puderem ser mescladas (ou seja, se elas combinam na coluna de junção)
                    if (rows[i]->mergeRows(*otherRow, joinColumn)) {
                        matchFound = true; // Marca que uma correspondência foi encontrada
                        break; // Sai do loop interno pois a correspondência já foi encontrada
                    }
                } catch (const invalid_argument& e) {
                    cerr << "Error: " << e.what() << endl;
                }
            }
            // Se nenhuma correspondência foi encontrada, adiciona o índice da linha à lista de linhas a serem removidas
            if (!matchFound) {
                rowsToRemove.push_back(i);
            }
        }

        // Remove as linhas que não encontraram correspondência
        // A remoção é feita de trás para frente para evitar problemas com mudanças nos índices das linhas
        for (auto it = rowsToRemove.rbegin(); it != rowsToRemove.rend(); ++it) {
            removeRow(*it);
        }
    }
    
    /**
    * @brief Agrupa linhas por frequência de valores em uma coluna especificada.
    *
    * Este método conta a frequência de cada valor único na coluna especificada e cria um novo DataFrame
    * com o resultado. A nova tabela terá duas colunas: o valor do grupo e sua contagem.
    *
    * @param groupColumn Nome da coluna para agrupar os dados e contar frequências.
    * @return DataFrame Resultante contendo duas colunas, uma para o valor do grupo e outra para a soma.
    *
    * @throws invalid_argument Se a coluna especificada não existir no DataFrame.
    *
    * Exemplo de Uso:
    * - Suponha que você tenha um DataFrame com uma coluna "Categoria". Usar `groupByFreq("Categoria")`
    *   retornaria um novo DataFrame mostrando cada categoria e quantas vezes cada uma aparece.
    */
    DataFrame groupByFreq(const string& groupColumn) {
        if (!containsColumn(groupColumn)) {
            throw invalid_argument("Both columns must exist in the DataFrame.");
        }

        // Mapa para armazenar as somas, com os valores da coluna de agrupamento como chaves.
        map<RowVariant, double> sumMap;

        // Itera sobre todas as linhas do DataFrame para acumular as somas.
        for (const auto& row : rows) {
            RowVariant groupValue = row->getCol_(groupColumn);  // Obtém o valor da coluna de agrupamento da linha atual.
            double sumValue = 1;  // Assume-se que estamos contando ocorrências, então cada contagem é 1.

            // Verifica se o valor já existe no mapa. Se sim, incrementa; se não, inicializa com o valor 1.
            if (sumMap.find(groupValue) != sumMap.end()) {
                sumMap[groupValue] += sumValue;
            } else {
                sumMap[groupValue] = sumValue;
            }
        }
        
        // Criando um novo DataFrame para conter o resultado.
        vector<string> columnNames = {groupColumn, "Sum"};
        vector<string> columnTypes = {getTypes().at(groupColumn), "double"};
        DataFrame result(columnNames, columnTypes);
        
        // Preenche o novo DataFrame com os resultados do mapa sumMap.
        for (const auto& [key, value] : sumMap) {
            vector<RowVariant> rowData = {key, value};
            shared_ptr<Row> newRow = make_shared<Row>(columnNames, rowData);
            result.insertRow(newRow); 
        }

        return result;
    }
    
    /**
    * @brief Agrupa linhas por soma de valores em uma coluna especificada.
    *
    * Este método soma valores de uma coluna de 'sumColumn' para cada grupo definido por 'groupColumn'.
    * O resultado é um novo DataFrame com o valor do grupo e a soma dos valores.
    *
    * @param groupColumn Nome da coluna para agrupar os dados.
    * @param sumColumn Nome da coluna cujos valores serão somados.
    * @return DataFrame Resultante contendo duas colunas, uma para o valor do grupo e outra para a soma.
    *
    * @throws invalid_argument Se qualquer uma das colunas especificadas não existir no DataFrame.
    */
    DataFrame groupBySum(const string& groupColumn, const string& sumColumn) {
        if (!containsColumn(groupColumn) || !containsColumn(sumColumn)) {
            throw invalid_argument("Both columns must exist in the DataFrame.");
        }
        
        // Mapa para armazenar as somas, chaveado pelo valor da coluna de agrupamento.
        map<RowVariant, double> sumMap;

        // Itera sobre todas as linhas do DataFrame para acumular as somas dos valores especificados na coluna sumColumn.
        for (const auto& row : rows) {
            RowVariant groupValue = row->getCol_(groupColumn);
            double valueToAdd;
            if(types.at(sumColumn) == "int"){
                valueToAdd = get<int>(row->getCol_(sumColumn)); 
            }else{
                valueToAdd = get<double>(row->getCol_(sumColumn));
            }

            if (sumMap.find(groupValue) != sumMap.end()) {
                sumMap[groupValue] += valueToAdd;
            } else {
                sumMap[groupValue] = valueToAdd;
            }
        }

        // Criando um novo DataFrame para conter o resultado
        vector<string> columnNames = {groupColumn, "Sum"};
        vector<string> columnTypes = {getTypes().at(groupColumn), "double"};
        DataFrame result(columnNames, columnTypes);

        // Preencher o novo DataFrame com os resultados de sumMap
        for (const auto& [key, value] : sumMap) {
            vector<RowVariant> rowData = {key, value};
            shared_ptr<Row> newRow = make_shared<Row>(columnNames, rowData);
            result.insertRow(newRow);
        }

        return result;
    }

    /**
    * @brief Agrupa linhas por média de valores em uma coluna especificada.
    *
    * Este método calcula a média de valores em 'avgColumn' para cada grupo definido por 'groupColumn'.
    * O resultado é um novo DataFrame com o valor do grupo e a média dos valores.
    *
    * @param groupColumn Nome da coluna para agrupar os dados.
    * @param avgColumn Nome da coluna cujos valores serão usados para calcular a média.
    * @return DataFrame Resultante contendo duas colunas, uma para o valor do grupo e outra para a média.
    *
    * @throws invalid_argument Se qualquer uma das colunas especificadas não existir no DataFrame.
    */
    DataFrame groupByAvg(const string& groupColumn, const string& avgColumn) {
        if (!containsColumn(groupColumn) || !containsColumn(avgColumn)) {
            throw invalid_argument("Both columns must exist in the DataFrame.");
        }
        // Mapa para armazenar somas e contagens, com os valores da coluna de agrupamento como chaves.
        map<RowVariant, pair<double, double>> avgMap; // Primeiro elemento é soma, segundo é contagem.
        
        
        // Itera sobre todas as linhas do DataFrame para acumular somas e contagens dos valores especificados na coluna avgColumn.
        for (const auto& row : rows) {
            RowVariant groupValue = row->getCol_(groupColumn);
            double valueToAdd;
            if(types.at(avgColumn) == "int"){
                valueToAdd = get<int>(row->getCol_(avgColumn)); 
            }else{
                valueToAdd = get<double>(row->getCol_(avgColumn));
            }

            if (avgMap.find(groupValue) != avgMap.end()) {
                avgMap[groupValue].first += valueToAdd;
                avgMap[groupValue].second += 1;
            } else {
                avgMap[groupValue] = make_pair(valueToAdd, 1);
            }
        }

        // Criando um novo DataFrame para conter o resultado. Define os nomes e tipos das colunas do novo DataFrame.
        vector<string> columnNames = {groupColumn, "Average"};
        vector<string> columnTypes = {getTypes().at(groupColumn), "double"};
        DataFrame result(columnNames, columnTypes);

        // Preenche o novo DataFrame com os resultados do mapa avgMap.
        for (const auto& [key, value] : avgMap) {
            double average = static_cast<double>(value.first) / value.second; // Calcula a média dividindo a soma pela contagem.
            vector<RowVariant> rowData = {key, average};
            shared_ptr<Row> newRow = make_shared<Row>(columnNames, rowData);
            result.insertRow(newRow);
        }

        return result;
    }
};

#endif // DATAFRAME_H