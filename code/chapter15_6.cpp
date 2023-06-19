#include <iostream>
#if 0
class Base
{
private:
    /* data */
public:
    Base(/* args */) = default;
    virtual ~Base() = default;
    virtual int fcn() { return -1; }
};

class D1 : public Base
{
public:
    // 隐藏基类的fcn,形参列表不同
    // D1继承了Base::fcn的定义。
    int fcn(int param) { return param; }  
    virtual void f2(){};
};

class D2 : public D1
{
public:
    int fcn(int param) { return param + 1; } //是一个非虚函数，隐藏了D1::fcn(int)
    int fcn() { return 2; }                  //覆盖了Base的虚函数fcn
    void f2() {}                             //覆盖了D1的虚函数f2
};

#endif

class Base
{
public:
    Base() = default;
    virtual ~Base() = default;

    //不管是不是虚函数都可以重载
    virtual int func(int a) { return a++; }
    virtual int func(int a, int b) { return a + b; }
    virtual int func(int a, int b, int c) { return a + b + c; }
};

class Derv : public Base
{
public:
    Derv() = default;
    ~Derv() = default;
    // using Base::func;
    // int func(int a) { return a + a + 1; } // 基类中的同名函数被隐藏，基类中的同名函数对Derv来说就是不可见的
    int test()
    {
        // return func(2,3); //如果不使用using, no matching function for call to 'Derv::func(int, int)'
        // return Base::func(2,3); //2可以使用作用域运算符让基类的func(int,int)可见
        return func(2, 3); // 不定义任何与基类成员函数同名的函数，那么基类中的所有func对Derv来说就是可见的
    }
};

int main(int argc, char const *argv[])
{
#if 0
    D1 d1obj;
    D2 d2obj;
    Base bobj, *bp1 = &bobj, *bp2 = &d1obj, *bp3 = &d2obj;
    bp1->fcn(); // 虚调用，将在运行时调用Base::fcn
    bp2->fcn(); // 需调用，将在运行时调用Base::fcn
    bp3->fcn(); // 虚调用，将在运行时调用D1::fcn

    D1 *d1p = &d1obj;
    D2 *d2p = &d2obj;
    // bp2->f2();     // 'class Base' has no member named 'f2'
    d1p->f2(); // 虚调用，将在运行时调用D1::f2()
    d2p->f2(); // 需调用，将在运行时调用D2::f2()
#endif

    return 0;
}
