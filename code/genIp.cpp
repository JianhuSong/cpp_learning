#include <iostream>
#include <string>
#include <vector>
#include <fstream>
class IPTool
{
public:
    static uint32_t maskLen2Mask(uint8_t masklen)
    {
        try
        {
            masklen = masklen > 32 ? 32 : masklen;
            uint32_t mask = 0;
            for (int i = 0; i < masklen; ++i)
            {
                mask |= (0x01 << i);
            }

            std::cout << "Convert masklen(" << uint32_t(masklen) << ") to mask(0x" << std::hex << mask << ")." << std::endl;
            std::cout.unsetf(std::ios::hex);
            return mask;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        return 0;
    }

    static uint8_t mask2MaskLen(uint32_t mask)
    {
        try
        {
            uint8_t masklen = 0;
            for (int i = 0; i < 32; i++)
            {
                masklen = (mask & (0x01 << i)) ? masklen + 1 : masklen;
            }

            std::cout << "Convert uint mask(" << mask << ") to masklen(" << uint32_t(masklen) << ")." << std::endl;

            return masklen;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        return 0;
    }

    static uint32_t stringIP2Uint(std::string ip)
    {
        try
        {
            std::string substr;
            uint8_t dip[4]{0};
            std::string tmp = ip;
            for (int i = 0; i < 4; ++i)
            {
                int p2 = 0;
                p2 = tmp.find('.');
                if (p2 == std::string::npos)
                {
                    dip[i] = std::atoi(tmp.c_str());
                    break;
                }
                std::string tmp1 = tmp.substr(0, p2);
                dip[i] = std::atoi(tmp1.c_str());
                tmp = tmp.substr(p2 + 1);
            }

            std::cout << "convert string ip(" << ip << ") to uint ip(" << *(uint32_t *)dip << ")." << std::endl;
            return *(uint32_t *)dip;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }

        return 0u;
    }

    static std::string uintIp2String(uint32_t ip)
    {
        try
        {
            char ipstr[50];
            uint8_t *chip = (uint8_t *)(&ip);
            sprintf(ipstr, "%u.%u.%u.%u", *chip, *(chip + 1), *(chip + 2), *(chip + 3));
            std::cout << "Convert uint ip(" << ip << ") to string ip(" << ipstr << ")." << std::endl;
            return std::string(ipstr);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }

        return std::string("");
    }

    static uint32_t getNetAddr(uint32_t ip, uint8_t masklen)
    {
        return ip & maskLen2Mask(masklen);
    }
};


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

std::vector<std::string> genIPStr(std::string ip_start, uint8_t mask_len, uint32_t ip_cnt)
{
    std::vector<std::string> ret;
    try
    {
        //masklen的最大值为32,处理以下传进来的masklen
        mask_len = mask_len > 32 ? 32:mask_len;

        //将IP的数量限制以下，最多可用IP有(2 << (32 - mask_len)), 主机ID有三个通常是保留的0.1.255
        uint32_t useful_ip = (1 << (32 - mask_len)) - 3;
        if(useful_ip > 0)
        {
            ip_cnt = useful_ip > ip_cnt?ip_cnt:useful_ip;
        }
        else
        {
            throw std::string("没有可用IP");
        }
        
        uint32_t mask = 0;
        for(int i = 0; i < (32 - mask_len); i++)
        {
            mask |= (0x01 << i);
        }
        std::cout <<"mask1 = " << mask << std::endl;
        mask = ~mask;
        std::cout <<"mask2 = " << mask << std::endl;

        
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
        
    }
    
    return ret;
    

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
