#include <iostream>

class myOperator
{
public:
    myOperator() : a(0), b(0), objname("") { std::cout << "called myOperator()" << std::endl; }
    myOperator(std::string name) : a(0), b(0), objname(name) { std::cout << "called myOperator(std::string name)" << std::endl; }
    myOperator(int a, int b, std::string name = "") : a(a), b(b), objname(name)
    {
        std::cout << "called myOperator(int a, int b, std::string name = \"\")" << std::endl;
    }

    ~myOperator() = default;
    friend std::ostream &operator<<(std::ostream &os, const myOperator &print);
    friend std::istream &operator>>(std::istream &is, myOperator &in);
    myOperator operator+(myOperator b1)
    {
        std::cout << "operator+ called." << std::endl;
        myOperator tmp("tmp");
        tmp.a = a + b1.a;
        tmp.b = b + b1.b;
        tmp.myOperatorPrint();
        return tmp;
    }

    myOperator operator-(myOperator b1)
    {
        std::cout << "operator+-called." << std::endl;
        myOperator tmp;
        tmp.a = a - b1.a;
        tmp.b = b - b1.b;
        tmp.myOperatorPrint();
        return tmp;
    }

    myOperator& operator=(myOperator b1)
    {
        std::cout << "operator= called." << std::endl;
        a = b1.a;
        b = b1.b;
        myOperatorPrint();
        return *this;
    }

    myOperator& operator+=(const myOperator &right)
    {   
        *this = *this + right;
        return *this;
    }

    void myOperatorPrint()
    {
        std::string prefix = objname.empty() ? std::string("") : objname + std::string(".");
        std::cout << prefix << "a = " << a << " " << prefix << "b = " << b << std::endl;
    }

    bool operator==(const myOperator &right)
    {
        return a == right.a && b == right.b;
    }

    bool operator!=(const myOperator &right)
    {
        return !(*this == right);
    }

    bool operator<(const myOperator &right)
    {
        return (a < right.a) && (b < right.b);
    }

    bool operator<=(const myOperator &right)
    {
        return *this < right || *this == right;
    }

    bool operator>(const myOperator &right)
    {
        return !(*this <= right);
    }

    bool operator>=(const myOperator &right)
    {
        return *this > right || *this == right;
    }

    //不带参数的是前置版本，前置版本返回递增后的对象的引用
    myOperator& operator++()
    {
        a++;
        b++;
        return *this;
    }

    //后置版本带一个不使用的形参，返回自增前的值；
    myOperator operator++(int)
    {
        myOperator ret = *this;   //记录计算前的值
        ++*this;                  //调用前置版本，实现自增功能。
        return ret;               //返回计算前的值。
    }

    myOperator& operator--()
    {
        a--; 
        b--;
        return *this;
    }

    myOperator operator--(int)
    {
        myOperator ret = *this;
        --*this;
        return ret;
    }


private:
    int a = 0;
    int b = 0;
    std::string objname = "";
};

std::ostream &operator<<(std::ostream &os, const myOperator &out)
{
    os << out.a << " " << out.b; // 输出运算符尽量减少格式化输出，所以这里就不写std::endl了。
    return os;
}

/*
class StrBlobPtr
{
    public:
    std::string & operator(std::size_t n)
    {
        return elements[n];
    }
    
    const std::string& operator(std::size_t n) const
    {
        return elements[n];
    }

    std::string& operator*() const
    {   
        std::vector<std::string> strvec(elements)
        return []
    }

    private:
    int a[100];
    std::size_t curr;
}
std::istream &operator>>(std::istream &is, myOperator &in)
{
    myOperator tmp;
    is >> tmp.a >> tmp.b; // 为了能够直接访问类的数据成员，将该函数定义为myOperator的友元函数。

    // 读取过程没有出错的化就为in赋新值。
    if (is)
    {
        in = tmp;
    }

    return is;
}
myOperator operator+(myOperator a, int i)
{
    myOperator tmp1, tmp(i, 0);
    tmp1 = a + tmp;
    return tmp1;
}
*/
class FuncOperator
{
    public:
        int operator()()
        {
            return 22;
        }

        int operator()(int a)
        {
            return a + 22;
        }
};
int main(int argc, char const *argv[])
{
    myOperator a("a"), b(1, 23, "b"), c(22, 0, "c");
    a = b + c;
    std::cout << "after operator+" << std::endl;
    a.myOperatorPrint();
    b.myOperatorPrint();
    c.myOperatorPrint();
    a = b - c;
    std::cout << "after operator-" << std::endl;
    a.myOperatorPrint();
    b.myOperatorPrint();
    c.myOperatorPrint();
    // operator>>(std::cin, a);
    // operator<<(std::cout, a) << std::endl;

    if (a == b)
    {
        std::cout << "a == b" << std::endl;
    }
    else
    {
        std::cout << "a != b" << std::endl;
    }

    FuncOperator f1,f2;
    std::cout << f1() << f2(122) << std::endl;
    return 0;
}
