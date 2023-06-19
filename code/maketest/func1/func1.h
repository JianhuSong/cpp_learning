#ifndef _FUNC1_H__
#define _FUNC1_H__
#include "../common/common.h"

class MyVector
{
private:
    std::vector<int> _myVec;
    /* data */
public:
    MyVector() {}
    ~MyVector() = default;
    int size();
    void addElement(int ina);
    int getValue(int index, int &value);
};

#endif // !_FUNC1_H__
