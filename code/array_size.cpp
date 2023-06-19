#include <iostream>
#include <memory>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    char *cp[100];
    for(int i = 0; i < 100; i++)
    {
        cp[i] = (char *)malloc(10);
    }
    std::cout << "sizeof(cp) = " << sizeof(cp)  << std::endl;
    std::cout << "sizeof(int) = " << sizeof(int) << std::endl;
    std::cout << "sizeof(char *cp) = " << sizeof(char *) << std::endl;
    std::cout << "sizeof(cp)/sizeof(int) = " << sizeof(cp) / sizeof(int) << std::endl;
    std::cout << "sizeof(cp)/sizeof(char*) = " << sizeof(cp) / sizeof(char *) << std::endl;
    return 0;
}
