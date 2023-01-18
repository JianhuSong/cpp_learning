#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

class StrBlob
{
public:
    typedef std::vector<std::string>::size_type size_type;
    StrBlob() : data(std::make_shared<std::vector<std::string>>()) {}
    StrBlob(std::initializer_list<std::string> il) : data(std::make_shared<std::vector<std::string>>(il)) {}
    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    void push_back(const std::string &t) { data->push_back(t); }
    void pop_back()
    {
        check(0, "pop_back on empty StrBlob");
        data->pop_back();
    }
    std::string &front()
    {
        check(0, "front on empty StrBlob");
        return data->front();
    }
    std::string &back()
    {
        check(0, "back on empty StrBlob");
        return data->back();
    }

private:
    std::shared_ptr<std::vector<std::string>> data;
    void check(size_type i, const std::string msg) const
    {
        if (i >= data->size())
            throw std::out_of_range(msg);
    }
};

void test(StrBlob &b1)
{
    std::cout << "b1 size = " << b1.size() << std::endl;
    std::cout << "b1.front() = " << b1.front() << std::endl;
    std::cout << "b1.back() = " << b1.back() << std::endl;
    b1.empty() ? std::cout << " b1 is empty." << std::endl : std::cout << "b1.size() = " << b1.size() << std::endl;
}

void strblob_test()
{
    try
    {
        StrBlob b1;
        {
            StrBlob b2 = {"a", "an", "the"};
            b1 = b2;
            b2.push_back("about");
        } // b2被销毁了，但是b2中的元素不能销毁
        // b1指向最初由b2创建的元素

        test(b1);
        auto size = b1.size();
        for (int i = 0; i < size; ++i)
        {
            b1.pop_back();
        }

        test(b1);
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << '\n';
    }
}

void new_test()
{
    int *p1 = new int;     //值未定义，随机值
    int *p2 = new int();   //值初始化，0

    std::cout << "*p1 = " << *p1 << ", *p2 = " << *p2 << std::endl;
    delete p1;
    delete p2;
}

int main(int argc, char const *argv[])
{

    new_test();
    return 0;
}
