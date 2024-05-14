#ifndef ROW_HPP
#define ROW_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <any>
#include <stdexcept>
#include <typeinfo>
#include <numeric>
#include <functional>
#include <utility>
#include <type_traits>
#include <variant>
#include <typeindex>
#include <iomanip>
#include "dataframe/time.hpp"

using RowVariant = std::variant<int, double, std::string, std::vector<int>, Time>;

class Row {
private:
    std::unordered_map<std::string, RowVariant> row;

public:
    Row();
    Row(const Row&) = default;
    Row(Row&&) = default;
    Row(const std::vector<std::string>& keys, const std::vector<RowVariant>& values);
    
    void addColRow(const std::string& name, const RowVariant& value);
    size_t size();
    void removeCol(const std::string& name);
    RowVariant& getCol(const std::string& name);
    const std::unordered_map<std::string, RowVariant>& getData() const;
    void printTypes() const;
    void printRow() const;
    std::string getTypeCol(const std::string& name);
    bool containsColumn(const std::string& name) const;
    RowVariant getCol_(const std::string& name) const;
    bool mergeRows(const Row& otherRow, const std::string& mergeColumn);
};

#endif // ROW_HPP