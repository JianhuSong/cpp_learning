#include <iostream>
#include <list>
#include <string>
#include <iterator>

void init_list(std::list<int> &lst, int init, int length)
{
    for (int i = 0; i < length; ++i)
    {
        lst.push_back(init + i);
    }
}

void list_printf(const std::list<int> &lst, std::string name = "")
{
    if (!name.empty())
    {
        std::cout << name << " : ";
    }

    for (auto _lst : lst)
    {
        std::cout << _lst << " ";
    }

    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    std::list<int> lst, lst2;
    init_list(lst, 5, 3);
    init_list(lst2, 0, 5);
    list_printf(lst, "lst");
    list_printf(lst2, "lst2");
    // lst.merge(lst2);
    lst.merge(lst2, [](int x, int y)
              { return x < y; });
    list_printf(lst, "lst"); // 输出0 1 2 3 4 5 6 7

    lst.remove(2);
    list_printf(lst, "lst"); // 输出0 1 3 4 5 6 7

    lst.remove_if([](int x)
                  { return x > 4; });
    list_printf(lst, "lst"); // 输出0 1 3 4

    lst.reverse();
    list_printf(lst, "lst"); // 4 3 1 0

    lst.push_back(3);
    lst.push_back(4);
    lst.push_back(3);
    list_printf(lst, "lst"); // 4 3 1 0 3 4 3

    lst.sort();
    list_printf(lst, "lst"); // 0 1 3 3 3 4 4

    lst.sort([](int x, int y)
             { return x > y; }); // 值大的元素排前面
    list_printf(lst, "lst");     // 4 4 3 3 3 1 0

    lst.unique();
    list_printf(lst, "lst"); // 4 3 1 0
    init_list(lst, 0, 10);
    lst.sort();
    list_printf(lst, "lst"); // 0 0 1 1 2 3 3 4 4 5 6 8 9

    lst.unique([](int x, int y)
               { return y == x + 1; }); // 后一个元素比前一个元素大1的话就删除
    list_printf(lst, "lst");            // 0 0 2 4 4 6 8
    lst.unique();

    init_list(lst2, 10, 2);

    auto it = lst.begin();
    // auto it = lst.begin() + 1; //没有定义+
    it++;
    lst.splice(it, lst2);

    list_printf(lst, "lst"); // 0 10 11 2 4 6 8

    init_list(lst2, 10, 2);
    lst.splice(it, lst2, lst2.begin());
    list_printf(lst, "lst"); // 0 10 11 10 2 4 6 8
    lst2.push_back(14);

    lst.splice(++it, lst2, lst2.begin(), lst2.end());
    list_printf(lst, "lst"); // 0 10 11 2 11 14 4 6 8

    return 0;
}
