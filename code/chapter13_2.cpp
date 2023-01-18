#include <iostream>
#include <string>
#include <utility>

class HasPtr
{

    friend void swap(HasPtr &, HasPtr &);

public:
    // 构造函数分配新的string和新的计数器，将计数器置为1
    HasPtr(const std::string &s = std::string()) : ps(new std::string(s)), i(0), use(new std::size_t(1))
    {
        std::cout << "HasPtr(string/null) >>>>*ps = " << *ps << ", i = " << i << ", *use = " << *use << std::endl;
    }

    // 拷贝构造函数拷贝所有三个数据成员，并递增计数器
    HasPtr(const HasPtr &p) : ps(p.ps), i(p.i), use(p.use)
    {
        std::cout << "use +1" << std::endl;
        ++*use;
        std::cout << "HasPtr(const HasPtr &p)>>>>*ps = " << *ps << ", i = " << i << ", *use = " << *use << std::endl;
    }

    // HasPtr &operator=(const HasPtr &rhs)
    // {
    //     ++*rhs.use;                               //递增右侧运算对象的引用计数
    //     std::cout << "other use +1" << std::endl;
    //     if (--*use == 0)                          //然后递减本对象的引用计数
    //     {
    //         std::cout << "in op= release." << std::endl;
    //         delete ps;                            //如果没有其他用户
    //         delete use;                           //释放本对象分配的成员
    //     }

    //     ps = rhs.ps;                              //将数据从rhs拷贝到本对象
    //     i = rhs.i;
    //     use = rhs.use;
    //     std::cout << "operator=>>>*ps = " << *ps << ", i = " << i << ", *use = " << *use << std::endl;
    //     return *this;                             //返回本对象
    // }

     HasPtr &operator=( HasPtr rhs)
     {
        std::cout << "HasPtr &operator=( HasPtr rhs) swap version." << std::endl;
        swap(*this, rhs);
        return *this;
     }

    ~HasPtr()
    {
        if (--*use == 0) // 如果引用计数器为0
        {
            std::cout << "No user to use, free up resources." << std::endl;
            delete ps;  // 释放string内存
            delete use; // 释放引用计数器
        }
    }

private:
    std::string *ps;
    int i;
    std::size_t *use;
};

inline void swap(HasPtr &lhs, HasPtr &rhs)
{
    std::cout << "Hasptr swap." << std::endl;
    std::swap(lhs.ps, rhs.ps);
    std::swap(lhs.i, rhs.i);
}

int main(int argc, char const *argv[])
{
    HasPtr p1("P1"), p2("P1"), p3, p4(p1);
    p1 = p2;
    p3 = p1;
    return 0;
}
