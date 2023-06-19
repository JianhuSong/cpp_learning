#include "func1.h"
int MyVector::size()
{
    std::cout << "get the vector size. size = " << _myVec.size() << std::endl;
    return _myVec.size();
}

void MyVector::addElement(int ina)
{
    std::cout << "Add " << ina << " to the vector." << std::endl;
    _myVec.push_back(ina);
}

int MyVector::getValue(int index, int &value)
{
    if (index < 0 || index >= size())
    {
        std::cout << "The index[" << index << "] is out of range. cur vec size = " << _myVec.size() << std::endl;
        return -1;
    }

    value = _myVec.at(index);
    std::cout << "Get the value of index[" << index << "]. value = " << value << std::endl;
    return 0;
}