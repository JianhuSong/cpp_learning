# 《C++  Primer中文版（第五版）》   第13章 拷贝控制

## 拷贝、赋值与销毁

### 拷贝构造函数

如果一个构造函数的**第一个参数时自身类类型的引用**，且**任何额外参数都有默认值**，则此构造函数**是拷贝构造函数**。

拷贝构造函数的第一个参数必须是一个引用

拷贝构造函数通常应该是explicit的

**合成拷贝构造函数**

如果我们没有为一个类定义拷贝构造函数，编译器会为我们定义一个。

**合成拷贝构造函数的作用？**

​	对于某些类来说，合成拷贝构造函数用来阻止哦我们拷贝该类类型的对象

​	一般情况，合成拷贝构造函数将其参数成员逐个拷贝到正在创建的对象中。

每个成员的类型决定了其如何被拷贝：

​	对于类类型的成员，会使用其拷贝构造函数来拷贝

​	内置类型的成员则直接拷贝

​	对于数组合成拷贝构造函数会之元素拷贝一个数组类型的成员。

**拷贝初始化**

**什么时候会发生拷贝初始化？**

​	使用=定义变量时

​	将一个对象作为实参传递给一个非引用类型的形参

​	从一个返回类型为非引用类型的函数返回一个对象

​    用花括号列表初始化一个数组中的元素或一个聚合类中的成员

​	某些类类型还会对它所分配的对象使用拷贝初始化。（初始化标准库容器或调用其insert或push成员）

**参数和返回值**

在函数调用过程中，具有非引用类型的参数要进行拷贝初始化

但一个函数具有非引用的返回类型时，返回值会被用来初始化调用方的结果。

**拷贝初始化的限制**

对于那些构造函数时explicit的类而言，不能使用拷贝初始化

**编译器可以绕过拷贝构造函数**

在拷贝初始化过程中，编译器可以（但不是必须）跳过拷贝/移动构造函数，直接创建对象。

### 拷贝赋值运算符

与拷贝构造函数一样，如果类未定义自己的拷贝赋值运算符，编译器就会为它合成一个。

**重载赋值运算符**

重载运算符本质上是函数

名字由operator关键字后接表示要定义的运算符的符号组成

赋值运算符通常返回一个指向其左侧运算符对象的引用。

注意事项：

​	标准库通常要求保存在容器中的类型要具有赋值运算符，且其返回时左侧运算符对象的引用。

合成拷贝赋值运算符的作用？

​	对于某些类，合成拷贝赋值运算符用来禁止该类型对象的赋值

​	如果不是出于上述目的，它会将右侧运算对象的每个非static成员赋予左侧运算对象的对应成员，这一操作通过成员类型的拷贝赋值运算符来完成。对于数组类型的成员，逐个赋值数组元素。

### 析构函数

析构函数会释放对象使用的资源，并销毁对象的非static数据成员。

析构函数特点：

​	是类的一个成员函数

​	由波浪号接类名构成

​	没有返回值

​	不接受参数

析构函数完成什么工作？

​	首先执行函数体，然后销毁成员。成员按初始化顺序逆序销毁。

成员销毁时发生什么完全依赖于成员的类型

关键点：

​	隐式销毁一个内置指针类型的成员不会delete它所指向的对象。

什么时候会调用析构函数？

​	无论何时一个对象被销毁，就自动调用其析构函数

合成析构函数

当一个类未定义自己的析构函数时，编译器会为它定义一个合成析构函数。

对于某些类，合成析构函数被用来阻止该类型的对象被销毁，如果不是这种情况，合成析构函数的函数体就为空

析构函数体本身并不直接销毁成员。成员是在析构函数体之后隐含的析构阶段中被销毁的。

### 三/五法则

三个基本操作控制类的拷贝操作：

​	拷贝构造函数

​	拷贝赋值运算符

​	析构函数

新标准下还可以定义一个移动构造函数和一个移动赋值运算符

C++语言并不要求定义所有这些。

**需要析构函数的类也需要拷贝和赋值操作**

如果一个类需要一个析构函数，几乎可以肯定它也需要一个拷贝构造函数和一个拷贝赋值运算符

如果一个类需要自定义析构函数，几乎可以肯定它也需要自定义拷贝赋值运算符和拷贝构造函数。

**需要拷贝操作的类也需要赋值操作，反之亦然。**

### 使用=default

可以通过使用=default来显式地要求编译器生成合成的版本。

类内使用

​	合成的函数将隐式的声明为内联的

类外使用

​	只对成员的类外定义使用，合成的成员就不是内联的

注意事项：

​	我们只能对具有合成版本的成员函数使用=default(即，默认构造函数或拷贝控制成员)

### 阻止拷贝

**定义删除的函数**

​	在新标准下，我们可以通过将拷贝构造函数和拷贝赋值运算符定义为删除函数来阻止拷贝。

删除函数是这样一种函数，我们虽然定义了它，但是不能以任何方式使用它们。在函数的参数列表后面加上=delete来指出我们希望将它定义为删除的。

=delete必须出现在函数第一次声明的时候。

我们可以对任何函数指定=delete

**析构函数不能是删除的成员**

对于析构函数已删除的类型，不能定义该类型的变量或释放指向该类型动态分配的指针。

**合成的拷贝控制成员可能是删除的**

**类的合成析构函数什么时候被定义为删除的？**

​	类的某个成员的析构函数是删除的或不可访问的

**类的合成拷贝构造函数什么时候被定义为删除的？**

​	类的某个成员的拷贝构造函数是删除的或不可访问的

​	类的某个成员的析构函数是删除的或不可访问的

**类的合成拷贝赋值运算符什么时候被定义为删除的？**

​	类的某个成员的拷贝赋值运算符是删除的或不可访问的

​	类有一个const的成员

​	类有一个引用成员

**类的默认构造函数什么时候定义为删除的？**

​	类的某个成员的析构函数是删除的或不可访问的

​	类有一个引用成员，它没有内类初始化器

​	类有一个const成员，它没有内类初始化器且其类型未显式定义默认构造函数

注意：

​	本质上，当不可能拷贝、赋值或销毁类的成员时，类的合成拷贝控制成员就被定义未删除的

**private拷贝控制**

新标准前，类是通过将其拷贝构造函数和拷贝赋值运算符声明为private来阻止拷贝

声明但不定义一个成员函数是合法的，对此只有一个例外。

注意：

​	希望阻止拷贝的类应该用=delete来定义它们自己的拷贝构造函数和拷贝赋值运算符，而不应该将它们声明为private的。

## 拷贝控制和资源管理

类的行为像一个值，意味着它应该有自己的状态。当我们拷贝一个像值的对象时，副本和原身对象是完全独立的。

行为像指针的类则共享状态，当我们拷贝一个这种类的对象时，副本和原身使用相同的底层数据。改变副本也会改变原身，反之亦然。

### 行为像值的类

为了提供类值的行为，对于类管理的资源，每个对象都应该拥有一份自己的拷贝。

**类值拷贝赋值运算符**

赋值运算符通常组合了析构函数和构造函数的操作。类似析构函数，赋值操作会销毁左侧运算对象的资源。类似拷贝构造函数，赋值操作会从右侧运算对象拷贝数据。

**编写赋值运算符需要注意的两点：**

​	1.如果将一个对象赋予自身，赋值运算符必须能正常工作。

​	2.大多数赋值运算符组合了析构函数和拷贝构造函数的工作。

### 定义行为像指针的类

对于行为类似指针的类，我们需要为其定义拷贝构造函数和拷贝赋值运算符，来拷贝指针成员本身而不是它指向的string。

令一个类展现处类似指针的行为的最好方法是使用shared_ptr来管理类中的资源。

当我们希望直接管理资源的情况下，使用引用计数就很有用了。

**引用计数**

引用计数的工作方式：

​	除了初始化对象外，每个构造函数（拷贝构造函数除外）还要创建一个引用计数，用来记录有多少对象与正在创建的对象共享状态。当	我们创建要给对象时，只有一个对象共享状态，因此将引用计数初始化为1.

​	拷贝构造函数不分配新的计数器，而是拷贝给对象的数据成员，包括计数器。拷贝构造函数递增共享的计数器，指出给定对象的状态又	被一个新用户共享。

​	析构函数递减计数器，指出共享状态的用户少了一个。如果计数器变为0，则析构函数释放状态。

​	拷贝赋值运算符递增右侧运算对象的计数器，递减左侧运算对象的计数器。如果左侧运算对象的计数器变为0，意味着它的共享状态灭	有用户了，拷贝赋值运算符就必须销毁状态。

在哪里存放引用计数器？

​	一种方法是将计数器保存在动态内存中。	

定义一个使用引用计数的类

```cpp
#include <iostream>
#include <string>

class HasPtr
{
public:
    // 构造函数分配新的string和新的计数器，将计数器置为1
    HasPtr(const std::string &s = std::string()) : ps(new std::string(s)), i(0), use(new std::size_t(1))
    {
        std::cout << "HasPtr(string/null) >>>>*ps = " << *ps << ", i = " << i << ", *use = " << *use << std::endl;
    }

    // 拷贝构造函数拷贝所有三个数据成员，并递增计数器
    HasPtr(const HasPtr &p) : ps(p.ps), i(p.i), use(p.use)
    {
        std::cout << "use +1" << std::endl;
        ++*use;
        std::cout << "HasPtr(const HasPtr &p)>>>>*ps = " << *ps << ", i = " << i << ", *use = " << *use << std::endl;
    }

    HasPtr &operator=(const HasPtr &rhs)
    {
        ++*rhs.use;                               //递增右侧运算对象的引用计数
        std::cout << "other use +1" << std::endl;
        if (--*use == 0)                          //然后递减本对象的引用计数      
        {
            std::cout << "in op= release." << std::endl;
            delete ps;                            //如果没有其他用户
            delete use;                           //释放本对象分配的成员
        }

        ps = rhs.ps;                              //将数据从rhs拷贝到本对象
        i = rhs.i;
        use = rhs.use;
        std::cout << "operator=>>>*ps = " << *ps << ", i = " << i << ", *use = " << *use << std::endl;
        return *this;                             //返回本对象
    }

    ~HasPtr()
    {
        if (--*use == 0) // 如果引用计数器为0
        {
            std::cout << "No user to use, free up resources." <<std::endl;
            delete ps;  // 释放string内存
            delete use; // 释放引用计数器
        }
    }

private:
    std::string *ps;
    int i;
    std::size_t *use;
};

int main(int argc, char const *argv[])
{
    HasPtr p1("P1"), p2("P1"), p3,p4(p1);
    p1 = p2;
    p3 = p1;
    return 0;
}

```



## 交换操作

除了定义拷贝控制成员，管理资源的类通常还定义了一个名为swap的函数。对于那些与重排算法一起使用的类，定义swap是非常重要的。这类算法在需要交换两个元素时会调用swap.

如果一个类定义了自己的swap,那么算法将使用类自定义版本，否则，算法将使用标准库的swap。

swap函数因该调用swap，而不是std::swap

在赋值运算符中使用swap

定义swap的类通常使用swap来定义它们的赋值运算符。使用拷贝交换技术

这个版本中，参数不是一个引用，将右侧运算对象以传值方式传递。

**拷贝控制示例**

```cpp
class Message
{
    friend class Folder;
    friend void swap(Message &lhs, Message &rhs);

public:
    explicit Message(const std::string &str = "") : contents(str) {}
    Message(const Message &m) : contents(m.contents), folders(m.folders)
    {
        add_to_floders(m);
    }

    std::string getContents()
    {
        return this->contents;
    }

    Message &operator=(const Message &rhs)
    {
        std::cout << "message operator=" << std::endl;
        remove_from_floders();
        contents = rhs.contents;
        folders = rhs.folders;
        add_to_floders(rhs);
        return *this;
    }

    bool operator<(const Message &rhs)
    {
        return this->contents < rhs.contents ? true : false;
    }

    ~Message()
    {
        remove_from_floders();
    }

    void save(Folder &f)
    {
        folders.insert(&f);
        f.addMsg(this);
    }

    void remove(Folder &f)
    {
        folders.erase(&f);
        f.remMsg(this);
    }

private:
    std::string contents;
    std::set<Folder *> folders;

    void add_to_floders(const Message &m)
    {
        for (auto f : m.folders)
        {
            f->addMsg(this);
        }
    }

    void remove_from_floders()
    {
        for (auto f : folders)
        {
            f->remMsg(this);
        }
    }
};
```



## 动态内存管理类

某些类需要在运行时分配可变大小的内存空间。这种类通常可以使用标准库容器来保存它们的数据。

某些类需要自己进行内存分配，这些类一般来说必须定义自己的拷贝控制成员来管理所分配的内存。

```cpp
#include <iostream>
#include <string>
#include <memory>

class StrVec
{
public:
    StrVec() : elements(nullptr), first_free(nullptr), cap(nullptr) {}
    StrVec(const StrVec &);
    StrVec &operator=(const StrVec &);
    ~StrVec() { free(); }
    void push_back(const std::string &s);
    size_t size() const { return first_free - elements; }
    size_t capacity() const { return cap - elements; }
    std::string *begin() const { return elements; }
    std::string *end() const { return first_free; };

private:
    static std::allocator<std::string> alloc; // 静态成员，这是只是声明
    void chk_n_alloc()
    {
        if (size() == capacity())
            reallocate();
    }

    std::pair<std::string *, std::string *> alloc_n_copy(const std::string *, const std::string *);
    void free();
    void reallocate();
    std::string *elements;
    std::string *first_free;
    std::string *cap;
};

void StrVec::push_back(const std::string &s)
{
    chk_n_alloc();
    alloc.construct(first_free++, s);
}

std::pair<std::string *, std::string *> StrVec::alloc_n_copy(const std::string *b, const std::string *e)
{
    auto data = alloc.allocate(e - b);
    return {data, std::uninitialized_copy(b, e, data)};
}

void StrVec::free()
{
    if (elements)
    {
        for (auto p = first_free; p != elements;)
            alloc.destroy(--p);

        alloc.deallocate(elements, cap - elements);
    }
}

StrVec::StrVec(const StrVec &s)
{
    auto newdata = alloc_n_copy(s.begin(), s.end());
    elements = newdata.first;
    first_free = cap = newdata.second;
}

StrVec &StrVec::operator=(const StrVec &rhs)
{
    auto data = alloc_n_copy(rhs.begin(), rhs.end());
    free();
    elements = data.first;
    first_free = cap = data.second;
    return *this;
}

void StrVec::reallocate()
{
    auto newcapacity = size() ? 2 * size() : 1;
    auto newdata = alloc.allocate(newcapacity);
    auto dest = newdata;
    auto elem = elements;
    for (size_t i = 0; i != size(); ++i)
    {
        alloc.construct(dest++, std::move(*elem++));
    }
    free();
    elements = newdata;
    first_free = dest;
    cap = elements + newcapacity;
}

std::allocator<std::string> StrVec::alloc;

void strVecInit(StrVec &sv)
{
    for (int i = 0; i < 10; i++)
    {
        sv.push_back(std::string("helll world.  ") + std::to_string(i));
    }
}

void strVecPrint(StrVec &sv, std::string msg = "")
{
    std::cout << msg << std::endl;

    std::cout << "size = " << sv.size() << std::endl;
    std::cout << "capacity = " << sv.capacity() << std::endl;
    for (auto i = sv.begin(); i != sv.end(); ++i)
    {
        std::cout << *i << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    StrVec sv, sv2;
    strVecInit(sv);
    strVecPrint(sv, "Sv:");
    sv2 = sv;
    strVecPrint(sv2, "SV2:");
    return 0;
}
```



## 对象移动

在某些情况下，对象拷贝后立即就被销毁了，在这种情况下，移动而非拷贝对象会大幅度提升性能。

有些时候从旧的内存将元素拷贝到新的内存是不必要的。

标准库容器、string和shared_ptr类及支持移动也支持拷贝。IO类和unique_ptr类可以移动但不能拷贝。

### 右值引用

所谓右值引用就是必须绑定到右值的引用。可以通过&&来获取右值引用。

右值引用有一个重要的性质------只能绑定到一个将要销毁的对象。

一般而言一个左值表达式表示一个对象的身份，而一个右值表达式表示一个对象的值。

一个右值引用不过是某个对象的另一个名字而已。

返回左值引用的函数，连同赋值、下标、解引用和前置递增/递减运算符，都是返回左值的表达式的例子

返回非引用类型的函数，连同算术、关系、位以及后置递增/递减运算符，都生成右值。

左值持久：右值短暂

右值引用只能绑定到临时对象

​	所引用的对象将要销毁

​	该对象没有其他用户

变量是左值

​	变量可以看作只有一个运算对象而没有运算符的表达式。

标准库move函数

​	可以通过名为move的标准库函数来获得绑定到左值上的右值引用。该函数在头文件utility中。

```cpp
	int &&rr3 = std::move(rr1);
```

我们可以销毁一个移后源对象，也可以赋予它新值，但不能使用一个移后源对象。

对move不提供using声明，直接用std::move而不是move

### 移动构造函数和移动赋值运算符

移动构造函数的第一个参数是右值引用，任何额外的参数都必须要有默认参数

一旦资源完成移动，源对象必须不许再指向被移动的资源-----这些资源的所有全已经归属新创建的对象。

移动操作不因抛出任何异常

移动构造函数不分配任何新内存：它直接接管源对象的内存

```cpp
StrVec::StrVec(StrVec &&s) noexcept
    :elements(s.element), first_free(s.first_free), cap(s.cap)
{
	s.elements = s.first_free = s.cap = nullptr;        
}
```

移动操作、标准库容器和异常

移动操作通常不会抛出任何异常，当编写一个不会抛异常的移动操作时，应该通知标准库。

可以通过再构造函数中指明noexcept.(在一个函数的参数列表后指定noexcept)

不抛出异常的移动构造函数和移动赋值运算符必须标记位noexcept

移动赋值运算符

​	移动赋值运算符必须正确处理自赋值

```cpp
StrVec &StrVec::operator=(StrVec &&rhs) noexcept
{
	if(this != &rhs)
    {
        free();
        elements = rhs.elements;
        first_free = rhs.first_free;
        cap = rhs.cap;
        rhs.elements = rhs.first_free = rhs.cap = nullptr;
    }
    
    return *this;
}
```

移后源对象必须可析构

从一个对象移动数据并不会销毁此对象，但有时再移动操作完成后，源对象会被小伙。

我们编写一个移动操作时，必须确保移后源对象进入一个可析构的状态。

移动操作还必须保证对象仍然时有效的。

合成的移动操作

​	只有当一个类没有定义任何自己版本的拷贝控制成员，且类的每个非 static数据成员都可以移动时，编译器才会为它合成移动构造函数或移动赋值运算符。编译器能够移动内置类型的成员，如果一个成员时类类型，且该类右对应的移动操作，编译器也能移动这个成员。

移动操作永远不会隐式定义位删除的函数

将合成移动操作定义为删除的函数的条件：

​	有类成员定义了自己的拷贝构造函数且未定义移动构造函数，或者时类成员未定义自己的拷贝构造函数且编译器不能为其合成移动构造函数

​	如果有类成员的移动构造函数或移动赋值运算符被定义为删除的或是不可访问的，则类的移动构造函数或移动赋值运算符被定义为删除的。

如果类的析构函数被定义为删除的或不可访问的，则类的移动构造函数被定义为删除的。

如果有类成员是const的或是引用，则类的移动赋值运算符被定为删除的。

定义了一个移动构造函数或移动赋值运算符的类必须定义自己的拷贝擦欧总。否则，这些成员默认地被定义为删除的

移动右值，拷贝左值

但如果没有移动构造函数，右值也被拷贝

​	一般情况下，拷贝构造函数满足对应的移动构造函数的要求：它会拷贝给定对象，并将原来的对象置为有效转台。实际上，拷贝构造函数甚至都不会改变原对象的值。

更新三/五法则：

​	所有五个拷贝控制成员应该看作一个整体：一般来说，如果一个类定义了任何一个拷贝操作，它就应该定义所有五个操作。

建议：不要随意使用移动操作

​	由于一个移后源对象具有不确定的状态，对其调用std::move是危险的。当我们调用move时，必须绝对确认移后源对象没有其他用户。

### 右值引用和成员函数

除了构造函数和赋值运算符之外，如果成员函数同时提供拷贝函数和移动版本，它能从中受益。

一个版本接受要给指向const的左值引用，第二个版本几首一个指向非 const的右值引用。

**右值和左值引用成员函数**

通常，我们在一个对象上调用成员函数，而不管该对象是一个左值还是一个右值。

我们指出this的左值/右值属性的方式： 在参数列表后放置一个引用限定符

引用限定符只能用于（非static）成员函数，且必须同时出现在函数的声明和定义中

一个函数可以同时用const和引用限定。在此情况下，引用限定必须跟在 const限定符之后

```cpp
class Foo
{
public:
	Foo someMem() const &;    
}
```

重载和引用函数

如果我们定义两个或两个以上的具有相同名字和相同参数列表的成员函数，就必须对所有函数加上引用限定，或者都不加