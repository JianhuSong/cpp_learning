#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::vector<uint16_t> genIP(uint16_t ipStart, uint16_t nsize)
{
    std::vector<uint16_t> ips;
    for(uint16_t i = 0;; i++)
    {

        uint16_t tmp = ipStart + i;
        int tmp1 = int(*((uint8_t*)(&tmp)));
        if(tmp1 !=0 && tmp1 != 255 && tmp1 != 1)
        {
            ips.push_back(tmp);
        }

        if(ips.size() >= nsize)
        {
            break;
        }
    }

    return ips;
}

int main(int argc, char const *argv[])
{
    std::vector<uint16_t> ipMasks;

    std::ofstream out("ipMask.txt");

    if (!out)
    {
        std::cout << "can open the file." << std::endl;
    }

    ipMasks = genIP(2,2400);
    for (auto &ipmask : ipMasks)
    {
        
        int(*((uint8_t*)(&ipmask)));
        out << std::string("tdma:188.188.") << int(*((uint8_t*)(&ipmask) + 1)) << std::string(".") << int(*((uint8_t*)(&ipmask)))
            << std::string("+255.255.0.0;") << std::endl;
    }
    return 0;
}
