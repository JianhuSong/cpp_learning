#include <iostream>
#include "basket.h"
#include "bulk_quote.h"
int main(int argc, char const *argv[])
{
    Basket bsk;
    bsk.add_item(std::make_shared<Quote>("123", 45));
    bsk.add_item(std::make_shared<Bulk_quote>("345", 45, 3, .15));
    bsk.add_item(std::make_shared<Bulk_quote>("345", 45, 3, .15));
    bsk.add_item(std::make_shared<Bulk_quote>("345", 45, 3, .15));
    bsk.add_item(std::make_shared<Bulk_quote>("345", 45, 3, .15));
    bsk.add_item(Quote("234", 55));
    bsk.add_item(std::move(Quote("6666", 100.0)));
    double sum = bsk.total_receipt(std::cout);
    return 0;
}
