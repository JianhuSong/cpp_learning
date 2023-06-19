#include "func.h"
#include "../common/common.h"

int MyFuncClass::getAValue()
{
    std::cout << std::string("Get the var a's value :") << a << std::endl;
    return a;
}

int MyFuncClass::getBValue()
{
    std::cout << std::string("Get the var b's value :") << b << std::endl;
    return b;
}

void MyFuncClass::setValue(int ina, int inb)
{
    std::cout << "Before value, a = " << a << ", b = " << b << std::endl;
    a = ina;
    b = inb;
}