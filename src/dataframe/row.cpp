#include "dataframe/row.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <type_traits>
#include <typeinfo>

Row::Row() {}

Row::Row(const std::vector<std::string>& keys, const std::vector<RowVariant>& values) {
    if (keys.size() != values.size()) {
        throw std::invalid_argument("Number of keys must match number of values.");
    }
    for (size_t i = 0; i < keys.size(); ++i) {
        row[keys[i]] = values[i];
    }
}

void Row::addColRow(const std::string& name, const RowVariant& value) {
    row[name] = value;
}

size_t Row::size() {
    return row.size();
}

void Row::removeCol(const std::string& name) {
    auto it = row.find(name);
    if (it == row.end()) {
        throw std::invalid_argument("Column not found");
    }
    row.erase(it);
}

RowVariant& Row::getCol(const std::string& name) {
    auto it = row.find(name);
    if (it == row.end()) {
        throw std::invalid_argument("Column not found");
    }
    return it->second;
}

const std::unordered_map<std::string, RowVariant>& Row::getData() const {
    return row;
}

void Row::printTypes() const {
    for (const auto& col : row) {
        std::cout << col.first << " type: " << std::visit([](auto&& arg) -> std::string {
            return typeid(decltype(arg)).name();
        }, col.second) << std::endl;
    }
}

void Row::printRow() const {
    for (const auto& [key, value] : row) {
        std::cout << key << ": ";
        std::visit([](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double> || std::is_same_v<T, std::vector<int>>) {
                if constexpr (std::is_same_v<T, std::vector<int>>) {
                    for (int v : arg) std::cout << v << " ";
                } else {
                    std::cout << arg;
                }
            } else if constexpr (std::is_same_v<T, std::string>) {
                std::cout << '"' << arg << '"';
            } else if constexpr (std::is_same_v<T, Time>) {
                arg.print();
            }
            std::cout << " ";
        }, value);
        std::cout << std::endl;
    }
}

std::string Row::getTypeCol(const std::string& name) {
    auto it = row.find(name);
    if (it == row.end()) {
        throw std::invalid_argument("Column not found");
    }
    return std::visit([](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        return typeid(T).name();
    }, it->second);
}

bool Row::containsColumn(const std::string& name) const {
    return row.find(name) != row.end();
}

RowVariant Row::getCol_(const std::string& name) const {
    auto it = row.find(name);
    if (it == row.end()) {
        throw std::invalid_argument("Column not found");
    }
    return it->second;
}

bool Row::mergeRows(const Row& otherRow, const std::string& mergeColumn) {
    if (!containsColumn(mergeColumn) || !otherRow.containsColumn(mergeColumn)) {
        throw std::invalid_argument("Merge column not found in one or both rows");
    }
    if (getCol_(mergeColumn) != otherRow.getCol_(mergeColumn)) {
        throw std::invalid_argument("Merge column values do not match");
    }
    for (const auto& [key, value] : otherRow.getData()) {
        if (!containsColumn(key)) {
            addColRow(key, value);
        }
    }
    return true;
}