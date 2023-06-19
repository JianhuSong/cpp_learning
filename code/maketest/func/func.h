#ifndef _FUNC_H__
#define _FUNC_H__

class MyFuncClass
{
public:
    MyFuncClass() : a(0), b(0) {}
    MyFuncClass(int ina, int inb) : a(ina), b(inb) {}
    ~MyFuncClass() = default;

    int getAValue();
    int getBValue();
    void setValue(int ina, int inb);

private:
    int a = 0;
    int b = 0;
};

#endif // !_FUNC_H__