#pragma once
#include <string>

#include <vector>
#include <tuple>
#include <functional>

namespace internship {
    // do not remove this function
    void solution(const std::string& jsonFileName, int elementsCount);

    bool checkIfULConvertable(const std::string& partOfDate);
    bool validCalendarMonthAndDay(std::vector<unsigned long>& yearMonthDate);

    void calculateMaxSupports(const std::string& jsonFileName, int elementsCount);
    void printResult(std::vector<std::tuple<int, std::string, std::string>>& maxSupportLengths);

    std::vector<unsigned long> extractYearMonthDay(std::string& date);
}