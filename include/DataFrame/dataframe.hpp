#ifndef DATAFRAME_HPP
#define DATAFRAME_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <any>
#include <stdexcept>
#include <typeinfo>
#include <ctime>
#include <numeric>
#include <functional>
#include <utility>
#include <type_traits>
#include <typeinfo>
#include <memory>
#include <algorithm>
#include <map>

#include "dataframe/row.hpp"

class DataFrame {  
private:  
    std::vector<std::string> columns;
    std::unordered_map<std::string, std::string> types;
    std::vector<std::shared_ptr<Row>> rows;

public:  
    DataFrame();
    DataFrame(const std::vector<std::string>& colNames, const std::vector<std::string>& colTypes);

    const std::vector<std::string>& getColumns() const;
    const std::unordered_map<std::string, std::string>& getTypes() const;
    const std::vector<std::shared_ptr<Row>>& getRows() const;
    void setColumns(const std::vector<std::string>& newColumns);
    bool containsColumn(const std::string& name) const;
    void setTypes(const std::vector<std::string>& newTypes);
    void setRows(const std::vector<std::shared_ptr<Row>>& newRows);
    size_t getRowSize() const;
    size_t getColumnSize() const;
    bool validateRow(const Row& row) const;
    void insertRow(const std::shared_ptr<Row>& row);
    void removeRow(int index);
    const std::shared_ptr<Row>& getRow(int index) const;
    size_t getRowCount() const;
    size_t getColumnCount() const;
    void merge(const DataFrame& other);
    void head(int n) const;
    void tail(int n) const;
    void printTypes() const;
    void printDataFrame() const;
    void innerJoin(DataFrame& other, const std::string& joinColumn);
    DataFrame groupByFreq(const std::string& groupColumn);
    DataFrame groupBySum(const std::string& groupColumn, const std::string& sumColumn);
    DataFrame groupByAvg(const std::string& groupColumn, const std::string& avgColumn);
};

#endif // DATAFRAME_HPP