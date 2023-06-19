#include <iostream>
#include <string>

/*
    在C++语言中，基类必须将它的两种成员函数区分开：
        1.基类希望其派生类进行覆盖的函数，这类函数通常将其定义为虚函数。（就是只在函数声明前加上关键字vritual）
        2.基类希望派生类直接继承而不要改变的函数
*/

class Quote
{
public:
    Quote() = default;
    Quote(const std::string &book, double sales_price) : bookNo(book), price(sales_price) {}
    std::string isbn() const { return bookNo; }                         // 这中就是希望派生类直接继承的
    virtual double net_price(std::size_t n) const { return n * price; } // 这种就是希望派生类进行覆盖的
    virtual ~Quote() = default;                                         // 基类通常都应该定义一个虚析构函数
private:
    std::string bookNo;

protected:
    double price = 0.0;
};

class Bulk_quote : public Quote
{
public:
    Bulk_quote() = default;
    Bulk_quote(const std::string &book, double sales_price, std::size_t qty, double dis) : Quote(book, sales_price), min_qty(qty), discount(dis) {}
    double net_price(std::size_t n) const override // 派生类必须将其继承来的成员函数中那些需要被覆盖的函数重新声明（试了下不定义该函数，编译不会报错）
    {
        return n * price - discount * min_qty;
    }

private:
    std::size_t min_qty = 0;
    double discount = 0.0;
};

class Base
{
public:
    virtual int base_return(int i, int j = 11)
    {
        std::cout << "Using base::base_return!!" << std::endl;
        return i * j;
    }

    virtual ~Base() = default;
};

class D : public Base
{
public:
    int base_return(int j, int g = 200)
    {
        std::cout << "Using D::base_return!!" << std::endl;
        return j + g;
    }
};

void test(Base &base)
{
    std::cout << base.base_return(100) << std::endl;
    std::cout << base.Base::base_return(100) << std::endl;
}


int main(int argc, char const *argv[])
{
    D d;
    test(d);
    return 0;
}

// 输出结果
// Using base::base_return!!
// 1100
// Using base::base_return!!
// 1100
// Using D::base_return!!
// 111
// Using base::base_return!!
// 1100
