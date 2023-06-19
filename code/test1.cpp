#include <iostream>

class Base
{
    public:
        virtual void func() = 0;
};

class Derv:public Base
{
    public:
        void func() override;
};

void Derv::func()
{
    std::cout << "This is Derv's func." << std::endl;
}

void func(Base &base)
{
    base.func();
}

int main(int argc, char const *argv[])
{   
    Derv derv;
    func(derv);
    return 0;
}
