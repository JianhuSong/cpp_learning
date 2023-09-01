# 条款41： 了解隐式接口和编译期多态

隐式接口：

​	仅仅由一组有效表达式构成，表达式自身可能看起来很复杂，但它们要求的约束条件一般而言相当直接而明确。

显式接口：

​	通常由函数的签名式（也就是函数名称、参数类型、返回类型）构成

​	在源码中明确可见。

```cpp
#include <iostream>

// 个人感觉隐式和显式接口只是站在了不同的角度看一个类的接口。
// 站在类的角度:
// 类中定义的接口是显示的

// 站在template 参数的这个角色的角度而言：接口是隐式的，就是个表达式。
// （换个说法：这个隐式接口其实就是隐藏条件的说法，如果某个类想要作为template的参数，它必须有满足template 表达式要求的接口）
class Base
{
    public:
        ~Base() = default;
        virtual void myPrint() = 0;
        int size() {return 111;}
};

class Derived: public Base
{
    public:
    Derived() {}
    ~Derived() = default;
    virtual void myPrint()
    {
        std::cout << "Derived cout!!!" <<std::endl;
    }
};

void myPrint1(Derived &dd)
{
    dd.myPrint();
}

template<typename T>
void myPrint2(T t)
{
    t.myPrint();
    t.size();
}

int main(int argc, char const *argv[])
{
    Derived dd;
    myPrint1(dd);
    myPrint2(dd);
    return 0;
}
```

编译期多态：

在编译时才能确定具体调用哪个函数

```cpp
#include <iostream>
#include <string>

// 编译期多态，个人理解就是：在编译时存在多个选择，根据参数类型的不同调用不同的函数。
// 对于重载: 在编译时根据参数的不同，选择不同的函数。（这些个函数已经存在，选一个心仪的）
// 对于function templates: 在编译期，根据不同的template参数具现出不同的函数（函数还没有，根据参数不同，现生成一个心仪的）
template <typename T>
T max1(const T &a, const T &b)
{
    return a > b ? a : b;
}

int min1(const int &a, const int &b)
{
    std::cout << "int min" << std::endl;
    return a < b ? a : b;
}

std::string min1(const std::string &a, const std::string &b)
{
    std::cout << "string min" << std::endl;
    return a < b ? a : b;
}

int main(int argc, char const *argv[])
{
    int a = 11, b = 12;
    std::string a1("hello world"), b1("hello");

    std::cout << max1(a, b) << std::endl;
    std::cout << max1(a1, b1) << std::endl;
    std::cout << min1(a, b) << std::endl;
    std::cout << min1(a1, b1) << std::endl;
    return 0;
}

```

运行期多态：

在运行时才知道待用哪个函数。

```cpp
#include <iostream>

// 运行时多态演示代码
class Base
{
    public:
        ~Base() = default;
        virtual void myPrint() = 0;
};

class Derived: public Base
{
    public:
    Derived() {}
    ~Derived() = default;
    virtual void myPrint()
    {
        std::cout << "Derived cout!!!" <<std::endl;
    }
};


int main(int argc, char const *argv[])
{
    Base *pb = new Derived;

    // 程序运行的时候，才能明确调用的时Derived的myPrint()
    pb->myPrint();
    delete pb;
    return 0;
}
```

请记住：

classes 和template都支持接口和多态

对于classes而言接口是显式的，以函数签名为中心。多态则是通过virtual函数发生于运行期

对于template参数而言，接口是隐式的，基于有效表达式。多态则是通过template具现化和函数重载解析发生于编译器

# 条款42：了解typename的双重意义

请记住：

声明template参数时，前缀关键字class和typename可互换

请使用关键字表示嵌套从属类型名称；但不得在base class lists(基类列)或member initialization list(成员初值列)内以它作为base class修饰符。

# 条款43： 学习处理模板化基类内的名称

# 条款44： 将与参数无关的代码抽离templates

# 条款45：运用成员模板函数接受所有兼容类型

# 条款46：需要类型转换时请为模板定义非成员函数

# 条款47：请使用traits class 表现类型信息

# 条款48：认识template元编程