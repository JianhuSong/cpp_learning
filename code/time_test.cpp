#include <iostream>
#include <ctime>
#include <cmath>
#include <map>
#include <utility>
int main(int argc, char const *argv[])
{
    // time_t now;
    // struct tm newyear;
    // double seconds;
    // time(&now);
    // newyear =*localtime(&now);
    // newyear.tm_sec =0;

    // seconds = std::abs(difftime(now, mktime(&newyear)));
    // std::cout << seconds << std::endl;
    // seconds = std::abs(difftime(mktime(&newyear), now));
    // std::cout << seconds << std::endl;
    std::map<std::string, uint32_t> myMap;
    myMap["1"] = 1;
    myMap["2"] = 2;
    myMap["3"] = 3;
    myMap["4"] = 4;
    myMap["5"] = 5;

    for (auto &iter : myMap)
    {
        iter.second = 3333;
    }

    for (auto iter : myMap)
    {
        std::cout << iter.first <<" : "<< iter.second << std::endl;
    }

    return 0;
}
