#include <iostream>
#include <string>
#include <vector>

class IPTool
{
public:
    static uint32_t maskLen2Mask(uint8_t masklen)
    {
        try
        {
            masklen = masklen > 24 ? 24 : masklen;
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

int main(int argc, char const *argv[])
{

    std::cout << (uint32_t)IPTool::mask2MaskLen(IPTool::stringIP2Uint(IPTool::uintIp2String(IPTool::maskLen2Mask(24)))) << std::endl;
    return 0;
}
