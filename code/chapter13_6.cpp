#include <iostream>
#include <utility>

int main(int argc, char const *argv[])
{
    int i = 42, &rr1 = i;
    int &&rr3 = std::move(rr1);
    std::cout << rr3<< std::endl;
    rr3 = i * 42;
    std::cout << rr3<< std::endl;
    
    return 0;
}
