#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MyTest
#include <boost/test/included/unit_test.hpp>
#include <iostream>

#include <date/date.h>
#include <nlohmann/json.hpp>

#include "internship.h"

#include <chrono>

using namespace date;

//GITHUB BASE CASE WITH INVALID PRODUCTS CHECKING (EDGE CASES)
//some invalid versions skipping checked with data1.json "releasedDate" switch
//also testing if a product version with not defined EOL works.
BOOST_AUTO_TEST_CASE(BASE_TEST)
{
    constexpr int elementsCount{4};
    const auto CURRENT_DATE{ year_month_day{floor<days>( std::chrono::system_clock::now() )} };

    auto test_name = std::string(boost::unit_test::framework::current_test_case().p_name);

    boost::test_tools::output_test_stream output;

    std::cout << '\n' << "Running test: " << test_name << '\n' << '\n';
    std::cout << "[" + test_name + "] Actual output: \n"; 

    internship::calculateMaxSupports("tests/test1.json", elementsCount, output);

    std::array<int, elementsCount> expectedDays {
        (local_days(CURRENT_DATE) - local_days(year_month_day(year{2000}/month{10}/day{2}))).count() + 1,
        (local_days(year_month_day(year{2024}/month{7}/day{1})) - local_days(year_month_day(year{2021}/month{8}/day{14}))).count() + 1,
        (local_days(year_month_day(year{2023}/month{12}/day{1})) - local_days(year_month_day(year{2022}/month{6}/day{9}))).count() + 1,
        (local_days(year_month_day(year{2023}/month{7}/day{20})) - local_days(year_month_day(year{2022}/month{10}/day{20}))).count() + 1
    };

    std::string expectedOutput = "NoEOLDefined 22.04 " + std::to_string(expectedDays[0]) + '\n' 
    + "debian 11 " + std::to_string(expectedDays[1]) + '\n' 
    + "opensuse 15.4 " + std::to_string(expectedDays[2]) + '\n' 
    + "ubuntu 22.10 " + std::to_string(expectedDays[3]) + '\n';
    
    BOOST_CHECK(output.is_equal(expectedOutput));
    std::cout << "\n[" + test_name + "]" + " Expected output: \n" << expectedOutput;
}

//checking if no EOL definition is detected and currentGlobalDate is taken instead of it as EOL.
BOOST_AUTO_TEST_CASE(VARIATIONS_OF_EOL_NOT_DEFINED)
{
    constexpr int elementsCount{6};
    const auto CURRENT_DATE{ year_month_day{floor<days>( std::chrono::system_clock::now() )} };

    auto test_name = std::string(boost::unit_test::framework::current_test_case().p_name);

    boost::test_tools::output_test_stream output;

    std::cout << '\n' << "Running test: " << test_name << '\n' << '\n';
    std::cout << "[" + test_name + "] Actual output: \n"; 

    internship::calculateMaxSupports("tests/test2.json", elementsCount, output);

    std::array<int, elementsCount> expectedDays {
        (local_days(CURRENT_DATE) - local_days(year_month_day(year{2000}/month{10}/day{2}))).count() + 1,
        (local_days(CURRENT_DATE) - local_days(year_month_day(year{1999}/month{10}/day{2}))).count() + 1,
        (local_days(CURRENT_DATE) - local_days(year_month_day(year{1998}/month{10}/day{2}))).count() + 1,
        (local_days(CURRENT_DATE) - local_days(year_month_day(year{1997}/month{10}/day{2}))).count() + 1,
        (local_days(CURRENT_DATE) - local_days(year_month_day(year{1996}/month{10}/day{2}))).count() + 1,
        (local_days(CURRENT_DATE) - local_days(year_month_day(year{1995}/month{10}/day{2}))).count() + 1
    };

    std::string expectedOutput = "NoEOLDefined6 22.04 " + std::to_string(expectedDays[5]) + '\n' 
    + "NoEOLDefined5 22.04 " + std::to_string(expectedDays[4]) + '\n' 
    + "NoEOLDefined4 22.04 " + std::to_string(expectedDays[3]) + '\n' 
    + "NoEOLDefined3 22.04 " + std::to_string(expectedDays[2]) + '\n' 
    + "NoEOLDefined2 22.04 " + std::to_string(expectedDays[1]) + '\n' 
    + "NoEOLDefined1 22.04 " + std::to_string(expectedDays[0]) + '\n';
    
    BOOST_CHECK(output.is_equal(expectedOutput));
    std::cout << "\n[" + test_name + "]" + " Expected output: \n" << expectedOutput;
}

//Checking correctness with a lot of different data.
BOOST_AUTO_TEST_CASE(DATA_SET_NORMAL_CHECK)
{
    constexpr int elementsCount{10};
    const auto CURRENT_DATE{ year_month_day{floor<days>( std::chrono::system_clock::now() )} };

    auto test_name = std::string(boost::unit_test::framework::current_test_case().p_name);

    boost::test_tools::output_test_stream output;

    std::cout << '\n' << "Running test: " << test_name << '\n' << '\n';
    std::cout << "[" + test_name + "] Actual output: \n"; 

    internship::calculateMaxSupports("tests/test3.json", elementsCount, output);

    std::array<int, elementsCount> expectedDays {
        (local_days(year_month_day(year{2032}/month{5}/day{31})) - local_days(year_month_day(year{2022}/month{5}/day{26}))).count() + 1,
        (local_days(year_month_day(year{2029}/month{3}/day{1})) - local_days(year_month_day(year{2021}/month{3}/day{30}))).count() + 1,
        (local_days(year_month_day(year{2026}/month{4}/day{1})) - local_days(year_month_day(year{2018}/month{10}/day{23}))).count() + 1,
        (local_days(year_month_day(year{2028}/month{3}/day{15})) - local_days(year_month_day(year{2023}/month{3}/day{1}))).count() + 1,
        (local_days(year_month_day(year{2027}/month{11}/day{1})) - local_days(year_month_day(year{2023}/month{2}/day{27}))).count() + 1,
        (local_days(year_month_day(year{2026}/month{6}/day{30})) - local_days(year_month_day(year{2021}/month{10}/day{31}))).count() + 1,
        (local_days(year_month_day(year{2025}/month{5}/day{1})) - local_days(year_month_day(year{2022}/month{12}/day{13}))).count() + 1,
        (local_days(year_month_day(year{2024}/month{5}/day{23})) - local_days(year_month_day(year{2022}/month{5}/day{23}))).count() + 1,
        (local_days(year_month_day(year{2024}/month{5}/day{31})) - local_days(year_month_day(year{2022}/month{11}/day{7}))).count() + 1,
        (local_days(year_month_day(year{2024}/month{5}/day{18})) - local_days(year_month_day(year{2022}/month{11}/day{16}))).count() + 1,
    };

    std::string expectedOutput = "almalinux2 9 " + std::to_string(expectedDays[0]) + '\n' 
    + "almalinux 8 " + std::to_string(expectedDays[1]) + '\n' 
    + "amazon-rds-mysql 8.0 " + std::to_string(expectedDays[2]) + '\n' 
    + "amazon-linux 2023 " + std::to_string(expectedDays[3]) + '\n' 
    + "amazon-rds-postgresql 15 " + std::to_string(expectedDays[4]) + '\n' 
    + "antix 21 " + std::to_string(expectedDays[5]) + '\n'
    + "apache-cassandra 4.1 " + std::to_string(expectedDays[6]) + '\n'
    + "alpine 3.16 " + std::to_string(expectedDays[7]) + '\n'
    + "ansible-core 2.14 " + std::to_string(expectedDays[8]) + '\n'
    + "angular 15 " + std::to_string(expectedDays[9]) + '\n';
    
    BOOST_CHECK(output.is_equal(expectedOutput));
    std::cout << "\n[" + test_name + "]" + " Expected output: \n" << expectedOutput;
}

