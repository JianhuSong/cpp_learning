#ifndef __QUOTE_H__
#define __QUOTE_H__

#include <string>
#include <ostream>

class Quote
{
public:
    Quote() = default;
    Quote(const std::string &book, double sales_price) : bookNo(book), price(sales_price) {}
    Quote(const Quote &) = default;
    Quote(Quote &&) = default;
    Quote &operator=(const Quote &) = default;
    Quote &operator=(Quote &&) = default;
    virtual Quote *clone() const & { return new Quote(*this); }
    virtual Quote *clone() && { return new Quote(std::move(*this)); }
    std::string isbn() const { return bookNo; }                         // 这中就是希望派生类直接继承的
    virtual double net_price(std::size_t n) const { return n * price; } // 这种就是希望派生类进行覆盖的
    virtual ~Quote() = default;                                         // 基类通常都应该定义一个虚析构函数
private:
    std::string bookNo;

protected:
    double price = 0.0;
};

double print_total(std::ostream &os, const Quote &item, std::size_t n)
{
    double ret = item.net_price(n);
    os << "ISBN: " << n << " total due: " << ret << std::endl;
    return ret;
}

#endif // !__QUOTE_H__