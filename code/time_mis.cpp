#include <iostream>
#include <ctime>
#include <chrono>

int main(int argc, const char** argv) 
{
    auto timestart = std::chrono::system_clock::now().time_since_epoch();
    uint64_t timecount = timestart.count();
    std::cout << timestart.count() << std::endl;
    for(int i = 0;i< 100; i++)
    {
        if(i%2)
        {
            std::cout << "hello world." << std::endl;
        }
        
    }

    auto timeend = std::chrono::system_clock::now().time_since_epoch();
    uint64_t endtimecount = timeend.count();
    std::cout << timeend.count() << std::endl;

    auto timespend = timeend - timestart;
    auto microscond = std::chrono::duration_cast<std::chrono::microseconds>(timespend);
    std::cout << microscond.count()  <<std::endl;

    std::cout << endtimecount - timecount << std::endl;

    return 0;
}