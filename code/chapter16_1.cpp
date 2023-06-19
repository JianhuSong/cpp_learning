#include <iostream>
#include <memory>
#include <vector>
#include <string>
template <class T>
int compare(T a, T b)
{
    if (a > b)
        return 1;
    else if (a == b)
        return 0;
    else
        return -1;
}

template <typename T>
class test
{
public:
    test() {}
    ~test() {}
    T func(T t)
    {
        return t;
    }
    T func1(T, T);
};

// 可以这样理解：
// 模板类外定义类成员函数，必须以template开头
template <typename T>
T test<T>::func1(T t1, T t2)
{
    return t1 + t2;
}

template <typename T>
class BlobPtr;
template <typename T>
class Blob;
template <typename T>
bool operator==(const Blob<T> &, const Blob<T> &);

template <typename T>
class Blob
{
public:
    friend class BlobPtr<T>;
    //friend bool operator==(const Blob<T> &, const Blob<T> &);
    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;
    Blob() {}
    Blob(std::initializer_list<T> il) {}
    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    void push_back(const T &t) { data->push_back(t); }
    void push_back(const T &&t) { data->push_back(std::move(t)); }
    void pop_back()
    {
        check(0, "pop back on empty Blob");
        data->pop_back();
    }

    T &back();
    T &operator[](size_type i);

private:
    std::shared_ptr<std::vector<T>> data;
    void check(size_type i, const std::string &msg) const;
};

template<typename T> using Ublob = Blob<T>;

template <typename T>
void Blob<T>::check(size_type i, const std::string &msg) const
{
    if (i >= data->size())
        throw std::out_of_range(msg);
}
template <typename T>
T &Blob<T>::back()
{
    check(0, "back on empty Blob");
    return data->back();
}

template <typename T>
T &Blob<T>::operator[](size_type i)
{
    check(i, "subscript out of range");
    return (*data)[i];
}

template <typename T>
class BolbPtr
{
public:
    BolbPtr() : curr(0) {}
    BolbPtr(Blob<T> &a, std::size_t sz = 0) : wptr(), curr(sz) {}
    T &operator*() const
    {
        auto p = check(curr, "derefence past end");
        return (*p)[curr];
    }
    // 前置版本
    BolbPtr *operator++()
    {
        ++this;
        return this;
    }
    BolbPtr *operator--() // 处于类模板的作用域中时，可以直接使用模板名而不提供实参
    {
        --this;
        return this;
    }

    // 后置版本，多了个无用的int
    BolbPtr operator++(int);

private:
    std::shared_ptr<std::vector<T>> check(std::size_t, const std::string &) const {}
    std::weak_ptr<std::vector<T>> wptr;
    std::size_t curr;
};

template <typename T>
BolbPtr<T> BolbPtr<T>::operator++(int)
{
    BolbPtr ret = *this;
    ++*this;
    return ret;
}

template <typename T>
class Foo
{
public:
    static std::size_t count() { return ctr; }

private:
    static std::size_t ctr;
};

// 将模板类的static数据成员定义成模板
template <typename T>
std::size_t Foo<T>::ctr = 0;

// 如何在类外定义类模板的

template<typename T>
typename T::value_type top(const T& c)
{
    return typename T::value_type();
}

template <class T, class F = std::less<T>>
int compare1(const T &v1, const T &v2, F f = F())
{
    if (f(v1, v2))
        return -1;
    if (f(v2, v1))
        return 1;
    return 0;
}

template <class T = int>
class Numbers
{
public:
    Numbers(T v = 0) : val(v) {}

private:
    T val;
};

Numbers<double> aa;
Numbers<> a;

// 普通类使用成员模板
class test1
{
public:
    test1(){};
    ~test1() = default;
    template <class T>
    T func(const T &t1, const T &t2)
    {
        return t1 + t2;
    }
};

template <typename T>
class test2
{
public:
    test2(){};
    ~test2() = default;

    template <class T1>
    T1 func(const T1 &t1, const T1 &t2)
    {
        std::cout << "func " <<t1 + t2 << std::endl;
        return t1 + t2;
    }

    template <class u>
    u func2(const u &t1, const u &t2);
};

//类外定义
template <class T>
template <class u>
u test2<T>::func2(const u &t1, const u &t2)
{
    std::cout << "func2 " <<t1 + t2 << std::endl;
        return t1 + t2;
}


int main(int argc, char const *argv[])
{
    //std::cout << compare(1, 2) << std::endl;
    // Ublob<int> aa;

    // //要想使用模板类的静态成员，必须将实例化模板类
    // auto ct = Foo<int>::count();
    // std::cout << top(std::string("11")) << std::endl;

    std::cout << test1().func(23, 234) << std::endl; //普通类的成员模板调用方式
    test2<std::string> test3;
    test3.func(123,123);
    test3.func2(123,123);
    test2<int>().func(1234,1234);
    test2<int>().func2(1234,1234);
    return 0;
}

