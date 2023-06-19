#ifndef __BASKET_H__
#define __BASKET_H__
#include <set>
#include <memory>
#include "quote.h"

class Basket
{
public:

    void add_item(const Quote & sale)
    {
        items.insert(std::shared_ptr<Quote>(sale.clone()));
    }

    void add_item(Quote && sale)
    {
        items.insert(std::shared_ptr<Quote>(std::move(sale).clone()));
    }

    void add_item(const std::shared_ptr<Quote> &sale)
    {
        items.insert(sale);
    }

    double total_receipt(std::ostream &os) const
    {
        double sum = 0.0;
        for (auto iter = items.cbegin(); iter != items.cend(); iter = items.upper_bound(*iter))
        {
            sum += print_total(os, **iter, items.count(*iter));
        }

        os << "Total sales: " << sum << std::endl;
        return sum;
    }

private:
    static bool compare(const std::shared_ptr<Quote> &lhs,
                        const std::shared_ptr<Quote> &rhs)
    {
        return lhs->isbn() < rhs->isbn();
    }
    std::multiset<std::shared_ptr<Quote>, decltype(compare) *> items{compare};
};
#endif // !__BASKET_H__