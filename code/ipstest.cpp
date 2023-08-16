#include <iostream>
#include <string>

int main(int argc, char const *argv[])
{
    int32_t intip = 3233546242;
    for(int i = 0; i < 10; i++)
    {
        char ip[50];
        sprintf(ip,"%u.%u.%u.%u", intip >> 24 & 0xff,intip >> 16 & 0xff,intip >> 8 & 0xff,intip& 0xff);
        std::cout << "ip:" << ip << std::endl;
        intip++;
    }
    return 0;
}
