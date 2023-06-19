#include "common/common.h"
#include "func/func.h"
#include "func1/func1.h"

int main(int argc, const char **argv)
{
    MyVector myVec;
    MyFuncClass func;
    for (int i = 0; i < 100; i++)
    {
        myVec.addElement(i);
        func.setValue(i, i * i);
    }

    func.getAValue();
    func.getBValue();

    int value = 0;
    myVec.getValue(11, value);
    myVec.getValue(1000, value);

    myVec.size();
    uint64_t value1, value3;
    uint32_t value2 = (uint32_t)142061020;
    value1 = value2 * 100;
    value3 = value1;
    value3 = value3 * (uint64_t)100;
    std::cout << "value1 " << value1 << std::endl;
    std::cout << "vauel2 " << value2 << std::endl;
    std::cout << "value3 " << value3 << std::endl;
    std::string str("hello world;");

    std::vector<char> vchar(str.begin(), str.end());
    std::cout  << vchar.size() << std::endl;

    uint32_t AA = 0;
    AA |= (0xa1 << 8);
    std::cout  << AA << std::endl;

    return 0;
}