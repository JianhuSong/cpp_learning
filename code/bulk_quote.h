#ifndef __BULK_QUOTE_H__
#define __BULK_QUOTE_H__
#include "disc_quote.h"
class Bulk_quote : public Disc_quote
{
public:
    using Disc_quote::Disc_quote;
    Bulk_quote() = default;
    Bulk_quote* clone() const & {return new Bulk_quote(*this);}
    Bulk_quote* clone() && {return new Bulk_quote(std::move(*this));}
    Bulk_quote(const std::string &book, double sales_price, std::size_t qty, double dis) : Disc_quote(book, sales_price, qty, dis) {}
    double net_price(std::size_t cnt) const override
    {
        if (cnt >= min_qty)
            return cnt * (1 - discount) * price;
        else
            return cnt * price;
    }

private:
    std::size_t min_qty = 0;
    double discount = 0.0;
};
#endif //!__BULK_QUOTE_H__
