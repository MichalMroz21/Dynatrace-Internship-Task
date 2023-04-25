#include <chrono>
#include <fstream>
#include <iostream>
#include <string_view>
#include <algorithm>

#include <date/date.h>
#include <nlohmann/json.hpp>

#include "internship.h"

using json = nlohmann::json;
using namespace date;

namespace internship {

    bool validCalendarMonthAndDay(std::vector<unsigned long>& yearMonthDate){

        if(yearMonthDate.size() < 3) return false; //if something went wrong, in theory this should never trigger.

        unsigned long y{yearMonthDate[0]}, m{yearMonthDate[1]}, d{yearMonthDate[2]};

        if(m == 0 || d == 0 || y == 0){
            if(!(m == 0 && d == 0 && y == 0)) return false; //All of values 0 is considered as valid
        }
        
        if(m > 12 || d > 31) return false;

        if(m == 2){ //checking leap years for February
            if( (y % 4 == 0 && y % 100 != 0) || y % 400 == 0 ) return d <= 29;
            else return d <= 28;
        }

        if(m == 4 || m == 6 || m == 9 || m == 11){
            return d <= 30;
        }

        return true; 
    }

    bool checkIfULConvertable(const std::string& partOfDate){

        if( std::any_of(partOfDate.begin(), partOfDate.end(), [](char c){return !std::isdigit(c);}) || partOfDate.empty() ) return false; //if date is not numeric then date is invalid.
        return true;
    }

    void printResult(std::vector<std::tuple<int, std::string, std::string>>& maxSupportLengths){

        std::make_heap(maxSupportLengths.begin(), maxSupportLengths.end(), std::less<>{}); //turning min heap to max heap, so I can pop in ascending order.

        while(!maxSupportLengths.empty()){ 
            
            std::pop_heap(maxSupportLengths.begin(), maxSupportLengths.end(), std::less<>{});
            std::tuple<int, std::string, std::string> curr = maxSupportLengths.back();
            maxSupportLengths.pop_back(); 

            std::cout << std::get<2>(curr) << " " << std::get<1>(curr) << " " << std::get<0>(curr) << '\n';
        }
    }

    std::vector<long unsigned int> extractYearMonthDay(std::string& date){

        constexpr short amountToExtract{3};
        constexpr char SEPARATOR{'-'};

        std::vector<unsigned long> yearMonthDay(amountToExtract, 0); 
        //if no year, month, day specified then day month and year will be of value 0. (not considered as unvalid date, but date doesn't exist).

        date.push_back(SEPARATOR);

        std::size_t searchStartPos{}, curVectorPos{}, findResult{date.find(SEPARATOR, searchStartPos)};

        while(findResult != std::string::npos && curVectorPos < amountToExtract){

            std::string partOfDate{date.substr(searchStartPos, findResult - searchStartPos)};

            if(checkIfULConvertable(partOfDate) == false) return {}; //If EOL is empty - I assign currentDate or if releaseDate is empty - I skip, so
            //empty vector can be used as a flag.

            try{
                yearMonthDay[curVectorPos] = std::stoul(partOfDate); //to prevent ex. "eol": "2032232323223323223223-05-31", This for example would throw out-of-range exception
            }
            catch(std::exception& e){
                return {};
            }

            searchStartPos = findResult + 1;
            findResult = date.find(SEPARATOR, searchStartPos);
            curVectorPos++;
        }

        date.pop_back();

        if(validCalendarMonthAndDay(yearMonthDay) == false) return {}; //final check for correctness of date. If this isn't checked then a date like: 2022-32-40 will get through.
        
        return yearMonthDay;
    }


    void calculateMaxSupports(const std::string& jsonFileName, int elementsCount){

        std::ifstream f(jsonFileName);
        json productData = json::parse(f);

        constexpr std::string_view NAME_OF_RELEASE_DATE{"releaseDate"}, SECONDARY_NAME_OF_RELEASE_DATE{"releasedDate"}, 
        OS_NAME{"os"}, VERSIONS_NAME{"versions"}, EOL_NAME{"eol"}, CYCLE_NAME{"cycle"}, NAME{"name"}; //std::string_view better than std::string here. RAM instead of Heap.

        const auto CURRENT_DATE{ year_month_day{floor<days>( std::chrono::system_clock::now() )} };
        
        std::vector<std::tuple<int, std::string, std::string>> maxSupportLengths{}; 

        /* Data Structure and Algorithm consideration
        I considered traditional array with sorting, std::multiset, std::priority_queue and priority double ended queue.
        Heap data structure will be the best here. First of all heap sort is a O(nlogn) time complexity sorting algorithm, so that's good.
        But now a memory optimization can be made using heap. We don't have to store all of the supportLengths inside of some data structure like normal array. 
        We can just store elementsCount amount of supportLengths at one point of time, by removing the smallest supportLength everytime after pushing, 
        which makes memory usage more efficient.
        Now I didn't use std::priority_queue, because It doesn't support a make_heap (heapify all non-leaf nodes) operation in-place.
        If I used a std::priority_queue, then instead of having std::make_heap in printResult I would have to create additional memory - ex. stack to reverse the heap, and with that the whole
        point of memory optimization with heap would lose it's meaning. Fortunately std::priority_queue is just a wrapper around std::heap, so I can use std::vector with heap algorithms
        to achieve memory optimization with O(nlogn) sorting.
        */

        for(const auto& [id, product] : productData.items()) {

            if(product[NAME] == nullptr || product[OS_NAME] == nullptr || product[OS_NAME] == false) continue; //nullptr checks if there is no row named NAME, OS_NAME... in JSON file at all.

            int maxReleaseDate{}, selectedSupportLength{};
            std::string cycleOfMaxReleaseVersion{}, productName = product[NAME];

            bool atLeastOneVersionValid{false};

            for(const auto& [id, versionInfo] : product[VERSIONS_NAME].items()){

                bool isEOLFalse{false};

                auto endOfLive = versionInfo[EOL_NAME]; 
                auto cycle = versionInfo[CYCLE_NAME];
                auto releaseDate = versionInfo[NAME_OF_RELEASE_DATE];

                if(endOfLive == true) continue; //I'm assuming true is just invalid, so version with that is skipped. 
                if(endOfLive == nullptr || endOfLive == false || endOfLive == "") isEOLFalse = true; //currentDate will be taken instead, it just has to be compared with "" for some reason, .empty() wont work.

                if(releaseDate == nullptr || releaseDate == "") releaseDate = versionInfo[SECONDARY_NAME_OF_RELEASE_DATE]; //data1.json case, releasedDate instead of releaseDate.
                if(releaseDate == nullptr || releaseDate == "") continue; //version was never released, releaseDate = "", so continue skips it;

                std::string releaseDateStr{releaseDate};      
                std::vector<unsigned long> releaseDateYMDConverted{extractYearMonthDay(releaseDateStr)};

                if(releaseDateYMDConverted.empty()) continue; //empty vector flag - if something is wrong with the date.

                int yearRD = releaseDateYMDConverted[0];
                unsigned int monthRD = releaseDateYMDConverted[1], dayRD = releaseDateYMDConverted[2];

                date::year_month_day releaseDateYMD{
                    year_month_day(year{yearRD}/month{monthRD}/day{dayRD}) //This function takes int in year, and then two unsigned ints in month and day...
                }, eolDateYMD{};

                if(isEOLFalse == false){
                    std::string eolDateStr{endOfLive};
                    std::vector<unsigned long> eolDateYMDConverted{extractYearMonthDay(eolDateStr)};

                    if(eolDateYMDConverted.empty()) continue;

                    int yearEOL = eolDateYMDConverted[0];
                    unsigned int monthEOL = eolDateYMDConverted[1], dayEOL = eolDateYMDConverted[2];

                    if(yearEOL == 0 && monthEOL == 0 && dayEOL == 0){
                        eolDateYMD = CURRENT_DATE; //I cant simply check if endOfLive == "0", because what if it is for example "000000"
                    }
                    else eolDateYMD = year_month_day(year{yearEOL}/month{monthEOL}/day{dayEOL});
                }

                else eolDateYMD = CURRENT_DATE; //current system date taken if EOL doesn't exist - version is still supported to current day.
                
                const int supportLength{ (local_days(eolDateYMD) - local_days(releaseDateYMD)).count() + 1}; //substracting 2 ymds as days and converting to int with .count
                int releaseDateDays = (local_days(releaseDateYMD) - local_days(year_month_day(year{0}/month{0}/day{0}))).count(); //it won't allow count on just local_days(releaseDateYMD)..

                if(releaseDateDays == 0 || supportLength < 0) continue; //no releaseDate, handling releaseDate = "000000", negative supportLength - invalid

                int prevMaxReleaseDate{maxReleaseDate};

                maxReleaseDate = std::max(maxReleaseDate, releaseDateDays); 
                atLeastOneVersionValid = true; 

                if(prevMaxReleaseDate != maxReleaseDate){ //if new max is found with std::max then update corresponding information for this version.
                    cycleOfMaxReleaseVersion = cycle;
                    selectedSupportLength = supportLength;
                }
               
            }

            if(atLeastOneVersionValid){ //if every version is invalid is some way (continue triggered), then don't add length = 0 to results.
                maxSupportLengths.push_back(make_tuple(selectedSupportLength, cycleOfMaxReleaseVersion, productName));
                std::push_heap(maxSupportLengths.begin(), maxSupportLengths.end(), std::greater<>{}); 

                if(maxSupportLengths.size() > elementsCount){
                    std::pop_heap(maxSupportLengths.begin(), maxSupportLengths.end(), std::greater<>{}); //memory complexity optimization - only elementsCount amount of supportLengths need to be stored at one time.
                    maxSupportLengths.pop_back();
                }
            } 
                     
        }

        printResult(maxSupportLengths);
    }

    void solution(const std::string& jsonFileName, int elementsCount){
      calculateMaxSupports(jsonFileName, elementsCount); 
    }
}


