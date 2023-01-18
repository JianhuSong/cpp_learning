# 《C++  Primer中文版（第五版）》   第12章 动态内存

动态分配的对象的声明周期与它们在哪里创建是无关的，只有当显式地被释放时，这些对象才会销毁。

动态对象的正确释放被证明是编程中极其容易出错的地方。为了更安全地使用动态对象，标准库定义了两个智能指针类型来管理动态分配的对象。

当一个对象被释放时，指向它的之智能指针可以确保自动地释放它。

除了静态内存和栈内存，每个程序还拥有一个内存池。这部分内存被称为自由空间或者堆。程序用堆来存储动态分配的对象。

当动态对象不再使用时，我们的代码必须显式地销毁它们。

## 动态内存与智能指针

**标准库定义两种智能指针的目的？**

为了更容易（同时也更安全）地使用动态内存。

**智能指针和常规指针的不同之处？**

智能指针负责自动释放它所指向的对象。

**标准库定义了哪两种智能指针？**

unique_ptr和shared_ptr

**两种智能指针的区别？**

两种智能指针的区别在于管理底层指针的方式：shared_ptr允许多个指针指向同一个对象；unique_ptr则“独占”所指向的对象。

**还有别的么？**

有，标准库还定义了一个名为weak_ptr的伴随类，它是一种弱引用，指向shared_ptr所管理的对象。

**weak_ptr出现的目的是什么？**

weak_ptr用来解决shared_ptr引用成环的问题。

**它们在哪里？**

都定义在memory头文件中



### shared_ptr类

智能指针是模板，我们在创建一个智能指针时，必须提供额外的信息--指针可以指向的类型。

| shared_ptr和unique_ptr都支持的操作 |                                                              |
| ---------------------------------- | ------------------------------------------------------------ |
| shared_ptr<T> sp                   | 空智能指针，可以指向类型为T的对象                            |
| unique_ptr<T>up                    | 同上                                                         |
| p                                  | 将p作为一个条件判断，若p指向一个对象，则为true               |
| *p                                 | 解引用p,获得它指向的对象                                     |
| p->mem                             | 等价于(*p).mem                                               |
| p.get()                            | 返回p中保存的指针，要小心使用。若智能指针释放了对象，返回指针所指向的对象也就消失了。 |
| swap(p, q)                         | 交换p和q中的指针                                             |
| p.swap(q)                          | 同上                                                         |



| shared_ptr独有的操作 |                                                              |
| -------------------- | ------------------------------------------------------------ |
| make_shared<T>(args) | 返回一个shared_ptr，指向一个动态分配的类型为T的对象，使用args初始化此对象 |
| shared_ptr<T>p(q)    | p是shared_ptr q的拷贝；此操作会递增q中的计数器。q中的指针必须能转换为T* |
| p = q                | p和q都是shared_ptr，所保存的指针必须能够相互转换。此操作会递减p的引用计数，递增q的引用计数；若p的引用计数变为0，则将其管理的原内存释放。 |
| p.unique()           | 若p.use_count为1，返回true,否则返回false                     |
| p.use_count()        | 返回与p共享对象的智能指针的数量；可能很慢，主要用于调试      |

**make_shared函数**

最安全的分配和使用动态内存的方法时条用一个名为make_shared的标准库函数。此函数在动态内存中分配一个对象并初始化它，返回指向此对象的shared_ptr

```cpp
std::shared_ptr<int>p3 = std::make_shared<int>(42);
```

make_shared使用其参数来构造给定类型的对象。

**shared_ptr的拷贝和赋值**

**什么是引用计数器？**

我们可以认为每个shared_ptr都有一个关联的计数器。

**一旦一个shared_ptr的引用计数器变为0，它就会自动释放自己所管理的对象。**

**shared_ptr自动销毁所管理的对象**

shared_ptr的析构函数会递减它所指向的对象的引用计数器。如果引用计数器为0，shared_ptr的析构函数就会销毁对象，并释放它占用的内存。

**shared_ptr还会自动释放相关联的内存**

**使用了动态生存周期的资源的类**

程序使用动态内存出于以下三种原因之一：

1.程序不知道自己需要使用多少对象

2.程序不知道所需对象的准确类型

3.程序需要在多个对象间共享数据。

一般而言，如果两个对象共享底层数据，当某个对象被销毁时，我们不能单方面地销毁底层数据。

```cpp
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

int main(int argc, char const *argv[])
{
    try
    {
        StrBlob b1;
        {
            StrBlob b2 = {"a", "an", "the"};
            b1 = b2;
            b2.push_back("about");
        } //b2被销毁了，但是b2中的元素不能销毁
        //b1指向最初由b2创建的元素

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

    return 0;
}
```



### 直接管理内存

C++定义了两个运算符来分配和释放动态内存。

​	new:分配内存

​	delete:释放new分配的内存

**使用new动态分批和初始化对象**

在自由空间分配的内存是无名的，因此new无法为其分配一个对象命名，而是返回一个指向该对象的指针。

默认情况下，动态分配的对象是默认初始化的，这意味着内置类型和组合类型的对象的值将是未定义的，而类类型对象将用默认构造函数进行初始化。

我们可以使用直接初始化方式来初始化一个动态分配的对象。还可以是用传统的构造方式，在新标准下还以使用列表初始化。

值初始化的内置类型对象有着良好定义的值，而默认初始化的对象的值则是未定义的。

```cpp
    int *p1 = new int;     //值未定义，随机值
    int *p2 = new int();   //值初始化，0

    std::cout << "*p1 = " << *p1 << ", *p2 = " << *p2 << std::endl;
    delete p1;
    delete p2;
```

**动态分配的const对象**

用new分配const对象是合法的

一个动态分配的const对象必须进行初始化。对于一个定了默认构造函数的类类型，其const动态对象可以隐式初始化，而其他类型的对象就必须显式初始化。

由于分配的对象是const的，new返回的指针是一个指向const的指针。

**内存耗尽**

一旦一个程序耗光了它所有可用的内存，new表达式就会失败。抛出一个bad_alloc的异常。但是可以阻止其抛出异常。

```cpp
   int *p1 = new int;             //如果分配失败，new抛出std::bad_alloc
   int *p2 = new (nothrow) int;   //如果分配失败，new返回一个空指针。这种形式的new为定位new.定位new允许我们向new传递额外的参数。
```

**释放动态内存**

为了防止内存耗尽，在动态内存使用完毕后，必须将其归还给系统。

​	delete表达式接受一个指针，指向我们想要释放的对象。

**指针值和delete**

传递给delete的指针必须指向动态分配的内存，或者是一个空指针。

释放一块非new分配的内存，或者将相同的指针值释放多次，其行为是未定义的。

**动态内存的生存周期知道被释放为止**

**小心：动态内存的管理非常容易出错**

**使用new和delete管理动态内存存在三个问题：**

1.忘记delete内存

2.使用已经释放的对象。

3.同一块内存释放两次

**delete之后重置指针值**

shared_ptr和new配合使用

接受参数的智能指针构造函数是explicit的，必须使用直接初始化形式

```cpp
std::shared_ptr<int>p2(new int(1024));
```

不能进行内置指针到智能指针间的隐式转换

| 定义和改变shared_ptr的其他方法 |                                                              |
| ------------------------------ | ------------------------------------------------------------ |
| shared_ptr<T>p(q)              | p管理内存指针q所指向的对象；q必须指向new分配的内存，且能狗转换成T* |
| shared_ptr<T>p(u)              | p从unique_ptr u那里接管了对象的所有权；将u置空               |
| shared_ptr<T>p(q,d)            | p接管了内置指针q所指向的对象的所有全。q必须能够转换为T*类型。p将使用可调用对象d来代器delete |
| shared_ptr<T>p(p2,d)           | p是shared_ptr p2的拷贝，唯一区别是p将用可调用对象d来代替delete |
| p.reset()                      | 若p是唯一指向其对象的shared_ptr，reset会释放此对象           |
| p.reset(q)                     | 让p指向q                                                     |
| p.reset(q,d)                   | p指向q,p将使用可调用对象d来代替delete                        |

**不要混用普通指针和智能指针**

使用一个内置指针来访问一个智能指针所负责的对象是很危险的，因为我们无法知道对象何时会被释放

**不要使用get初始化另一个智能指针或为之智能指针赋值**

智能指针类型定义了一个名为get的函数，它返回一个内置指针。

get用来将指针的访问权限传递给代码，只有在确定代码不会delete指针的情况下才能使用get.

### 智能指针与异常

异常处理的程序能在异常发生后令程序流程继续

**智能指针和哑类**

包括所有标准库类在内的很多c++类都定义了析构函数，负责清理对象使用的资源。但是不是所有类都是这样良好定义的。

**使用我们自己的释放操作**

可以定义一个函数来代替delete

正确使用智能指针的一些基本规范：

1.不使用相同的内置指针初始化（或reset）多个智能指针。

2.不delete get()返回的指针

3.不使用get()初始化或reset另一个智能指针

4.如果使用get()返回的指针，记住当最后一个对应的智能指针销毁后，你的指针就变无效了。

5.如果使用智能指针管理的资源不是new分配的内存，记住传递给它一个删除器。

### unique_ptr

一个unique_ptr“拥有”它所指向的对象。

当我们定义一个unique_ptr时，需要将其绑定到一个new返回的指针上。初始化unique_ptr必须采用直接初始化形式。

unique_ptr不支持普通的拷贝或赋值操作。

| unique_ptr操作                        |                                                              |
| ------------------------------------- | ------------------------------------------------------------ |
| unique_ptr<T> u1;unique_ptr<T， D> u2 | 空unique_ptr，可以指向类型为T的对象。u1会使用delete来释放它的指针；u2会使用一个类型为D的可调用对象类释放它的指针 |
| unique_ptr<T, D> u(d)                 | 空unique_ptr，指向类型为T的对象，用类型为D的可调用对象d来代替delete |
| u=nullptr                             | 释放u指向的对象，将u置为空                                   |
| u.release()                           | u放弃对指针的控制权，返回指针，将u置空                       |
| u.reset()                             | 释放u指向的对象                                              |
| u.reset(q)；u.reset(nullptr)          | 如果提供了内置指针q,令u指向这个对象；否则将u置为空           |

传递unique_ptr参数和返回unique_ptr

我们可以拷贝和赋值一个将要被销毁的unique_ptr.最常见的就是函数返回一个unique_ptr

还可以返回一个局部对象的拷贝。

**向unique_ptr传递删除器**

我们可以重载一个unique_ptr中默认的删除器，我们必须在unique_ptr指向的类型之后提供删除器类型

### weak_ptr

waek_ptr是一种不控制所指向对象生存期的智能指针，它指向一个由shared_ptr管理的对象。

| weak_ptr         |                                                              |
| ---------------- | ------------------------------------------------------------ |
| weak_ptr<T>w     | 空weak_ptr指向类型为T的对象                                  |
| weak_ptr<T>w(sp) | 与shared_ptr sp指向相同对象的weak_ptr.T必须能够转换成sp指向的类型 |
| w = p            | p可以是一个shared_ptr或一个weak_ptr。赋值后w于p共享对象      |
| w.reset()        | 将w置空                                                      |
| w.use_count()    | 于w共享对象的shared_ptr的数量                                |
| w.expired()      | 若w.use_count()为0，返回true,否则返回false                   |
| w.lock()         | 如果expired为true,返回一个shared_ptr;否则返回一个指向w对象的shared_ptr |

当我们创建一个weak_ptr时，要用一个shered_ptr来初始化它

不能使用weak_ptr直接访问对象，而必须调用lock



## 动态数组

**如何一次为很多对象分配内存？**

使用另一种new表达式语法

使用标准库中的allocator类

使用标准库容器

### new和数组

为了让new分配一个对象数组，我们要在类型名之后跟一对方括号，在其中指明要分配的对象的数目。分配成功后返回指向第一个对象的指针。

方括号中的大小必须时整型，但不必是常量。

T*pia = new T[size];

也可以用一个表述数组类型的类型别名来分配要给数组

typedef T arr[size];

T *p = new arr;

**分配一个数组会得到一个元素类型的指针**

当用new分配一个数组是，并未得到一个数组类型的对象，而是得到一个数组元素类型的指针。

动态数组并不是数组类型

初始化动态分配的数组

默认情况下new分配的对象，不管是单个分配还是数组中的，都是默认初始化的。可以对数组中的元素进行值初始化，方法是在大小之后跟一对空括号。

还可以低通一个元素初始化器的花括号列表

初始化器会用来初始化动态数组中的开始部分的元素，如果初始化器数目小于元素数目，剩余元素将进行值初始化。如果初始化器数目大于元素数目，则new表达式失败，抛出一个名为bad_array_new_length的异常

**动态分配一个空数组是合法的**

当我们用new分配一个大小为0的数组时，new返回要给合法的非空指针。此指针保证与new返回的其他任何指针不相同。

**释放动态数组**

为了释放动态数组，我们使用一种特殊形式的delete，在指针前方加上空的方括号。

数组中的元素按逆序销毁

智能指针和动态数组

标准库提供了一个可以管理new分配的数组的unique_ptr版本。为了用一个unique_ptr管理动态数组，我们必须在对象类型后面跟一对空方括号。

**指向数组的unique_ptr**

| 指向数组的unique_ptr不支持成员访问运算符。其他unique_ptr操作不变 |                                                           |
| ------------------------------------------------------------ | --------------------------------------------------------- |
| unique_ptr<T[]>u                                             | u可以指向一个动态分配的数组，数组元素的类型为T            |
| unique_ptr<T[]>u(p)                                          | u指向内置指针p所指向的动态分配的数组，p必须能转换成类型T* |
| up[i]                                                        | 返回拥有的数组中位置I处的对象，u必须指向一个数组          |

如果使用shared_ptr管理一个动态数组，必须提供自己定义的删除器。

如果使用shared_ptr管理一个动态数组，必须用get获取一个内置指针，然后用它来访问数组元素。

那些没有默认构造函数的类就不能动态分配数组

### allocator类

标准库的allocator类帮助我们将内存分配和对象构造分离开来。

**标准库allocator类及其算法**

| allocator<T>a        | 定义了一个名为a的allocator对象，它可以为类型为T的对象分配内存 |
| -------------------- | ------------------------------------------------------------ |
| a.allocate(n)        | 分配一段原始的，未构造的内存，保存n个类型为T的对象。         |
| a.deallocate(p, n)   | 释放T*指针p地址开始的内存，这块内存保存了n个类型为T的对象，p必须是一个先前由allocatef返回的指针，且n必须是p创建时所要求的大小。在调用deallocate之前，用户必须对每个在这块内存中创建的对象调用destory |
| a.construct(p, args) | p必须是一个类型为T*的指针，指向一块原始内存，arg被传递给类型为T的构造函数，用来在p指向的内存中构造一个对象 |
| a.destroy(p)         | p为T*类型的指针，此算法对p指向的对象执行析构函数。           |

在还未构造对象情况下使用原始内存是错误的。

当我们用完对象后，必须对每个构造的元素调用destroy来销毁它们。

我们只能对真正构造了的元素进行destroy操作。

**拷贝和填充未初始化内存的算法**

**allocator算法**

| 这些函数在给定目的位置创建元素，而不是系统分配内存给它们 |                                                              |
| -------------------------------------------------------- | ------------------------------------------------------------ |
| uninitialized_copy(b, e,b2)                              | 从迭代器b和e的范围中拷贝元素到迭代器b2指定的原始未构造内存中，b2指向的内存必须足够大，能够容纳输入序列中的元素的拷贝 |
| uninitialized_copy_n(b,n,b2)                             | 从迭代器b指向的元素开始，拷贝n个元素到b2开始的内存中         |
| uninitialized_fill(b,e,t)                                | 在迭代器b和e指定的原始内存范围中创建对象，对象的值均为t的拷贝 |
| uninitialized_fill_n(b,n,t)                              | 从迭代器b指向的内存地址开始创建n个对象。b必须指向足够大的未构造的原始内存，能够容纳给定数量的对象 |



## 使用标准库：文本查询程序

开始一个程序设计的一个好方法是列出程序的操作。方便分析处需要什么样的数据结构。

```cpp
#include <iostream>
#include <memory>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using line_no = std::vector<std::string>::size_type;   //在两个例子中都有使用，干脆直接在外面写好了。

class QueryResult
{
    friend std::ostream &print(std::ostream &, const QueryResult &);

public:
    QueryResult(std::string s,
                std::shared_ptr<std::set<line_no>> p, 
                std::shared_ptr<std::vector<std::string>> f) : sought(s), lines(p), file(f) {}

private:
    std::string sought;                             //查询单词
    std::shared_ptr<std::set<line_no>> lines;       //出现的行号，原例子中，这里这个line_no 并未定义
    std::shared_ptr<std::vector<std::string>> file; //输入文件
};

class QueryResult;

class TextQuery
{
public:
    TextQuery(std::ifstream &is) : file(new std::vector<std::string>)
    {
        std::string text;
        while (std::getline(is, text))   
        {
            file->push_back(text);
            std::cout <<text<<std::endl;
            int n = file->size() - 1;
            std::istringstream line(text);
            std::string word;
            while (line >> word)
            {
                auto &lines = wm[word];
                if (!lines)
                {
                    lines.reset(new std::set<line_no>);
                }

                lines->insert(n);
            }
        }
    }

    QueryResult query(const std::string &sought) const
    {
        static std::shared_ptr<std::set<line_no>> nodata(new std::set<line_no>);
        auto loc = wm.find(sought);
        if (loc == wm.end())
            return QueryResult(sought, nodata, file);
        else
            return QueryResult(sought, loc->second, file);
    }

private:
    std::shared_ptr<std::vector<std::string>> file;
    std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;
};

//该函数为6.3.2-值是如何返回的 书中例子（没记住，百度查的）
//如果ctr大于1,返回word的复数形式
std::string make_plural(std::size_t ctr, const std::string &word, const std::string &ending)
{
    return (ctr > 1) ? word + ending : word;
}

std::ostream &print(std::ostream &os, const QueryResult &qr)
{
    os << qr.sought << " occurs " << qr.lines->size() << " "
       << make_plural(qr.lines->size(), "time", "s") << std::endl;

    for (auto num : *qr.lines)
    {
        os << "\t(line " << num + 1 << ")" << *(qr.file->begin() + num) << std::endl;
    }
    
    return os;
}

// 与用户交互,打印给定单词的查询结果
void runQueries(std::ifstream &infile)
{
    // infile 是一个ifstream指向我们要处理的文件
    TextQuery tq(infile);     //保存文件并建立查询map
    while (true)
    {
        std::cout << "enter word to look for, or q to quit: ";
        std::string s;

        //若遇到文件尾或用户输入了‘q’时循环退出
        if (!(std::cin >> s) ||s == "q")
            break;

        //指向查询并打印结果
        print(std::cout, tq.query(s)) << std::endl;
    }
}

int main(int argc, const char **argv)
{
    //argc == 1说明，没有给参数。./a.exe 这样argc = 1
    if(argc == 1)
    {
        //argc ==
        std::cout <<"The number of parameters is incorrect.  eg: ./a.exe filename"<<std::endl;
        return -1;
    }

    std::ifstream infile(argv[1]);
    runQueries(infile);

    return 0;
}
```

