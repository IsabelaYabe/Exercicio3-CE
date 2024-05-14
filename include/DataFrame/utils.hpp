// data_handling.hpp
#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <map>
#include <regex>
#include <memory>
#include <optional>
#include "dataframe/row.hpp"
#include "dataframe/dataframe.hpp"
#include "dataframe/time.hpp"

bool isNumber(const std::string& str);
bool isTimeFormat(const std::string& str);
std::vector<std::vector<std::string>> splitLogEntries(const std::string &log);
std::vector<DataFrame> processLogs(const std::string& logData);
std::vector<std::string> splitCsvLine(const std::string& line);
std::vector<std::vector<std::string>> createValidHeaders();
bool isHeader(const std::vector<std::string>& line, const std::vector<std::vector<std::string>>& validHeaders);
std::vector<std::vector<std::string>> splitCsvEntries(const std::string& csvData);
DataFrame processCsvData(const std::string& csvData);
bool isConcatenatedDateTime(const std::string& input);
void fixDateTimeFormat(std::string& dateTime);
std::vector<std::map<std::string, std::string>> splitJsonEntries(const std::string &jsonString);
DataFrame processJson(const std::string& jsonData);

#endif // UTILS_HPP