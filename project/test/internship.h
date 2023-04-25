#pragma once
#include <string>

#include <vector>
#include <tuple>
#include <functional>

#include <boost/test/tools/output_test_stream.hpp>

namespace internship {
    // do not remove this function
    void solution(const std::string& jsonFileName, int elementsCount);

    bool checkIfULConvertable(const std::string& partOfDate);
    bool validCalendarMonthAndDay(std::vector<long unsigned int>& yearMonthDate);

    void calculateMaxSupports(const std::string& jsonFileName, int elementsCount, boost::test_tools::output_test_stream& output);
    void printResult(std::vector<std::tuple<int, std::string, std::string>>& maxSupportLengths, boost::test_tools::output_test_stream& output);

    std::vector<unsigned long> extractYearMonthDay(std::string& date);
}