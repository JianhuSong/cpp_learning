# 《C++  Primer中文版（第五版）》   第七章 类

**什么是类？**

类是实现**信息封装**的基础。类是一种**用户自定义**的**数据类型**。每个类包含数据说明和一组操作数据或传递消息的函数。

**对象是什么？**

类的实例称为对象。

**类的基本思想？**

类的基本思想是数据抽象和封装。类要想实现数据抽象和封装，需要首先定义一个抽象数据类型。

**什么是数据抽象？**

数据抽象是一种依赖接口和实现分离的编程技术。

数据抽象是一种把项目中展示数据的部分与项目中操控数据分隔开来的强大的设计方法。也就是不关心如何运作。

## 7.1定义抽象数据类型

**什么是抽象数据类型？**

简单理解：无法直接访问类的数据成员，那么这个类就是一个抽象数据类型。

note:

C++程序员们无须可以区分应用程序的用户和类的用户

在一些简单的应用程序中，类的用户和类的设计者常常是同一人。尽管如此还是最好把角色区分开来。当我们设计类的接口时，应该考虑如何才能使类易于使用；而当我们使用类时，不应该顾及类的实现机理。

一个设计良好的类，既要有直观且易于使用的接口，也必须具备高效的实现过程。

**成员函数的实现的位置有什么要求？**

没有要求，成员函数可以实现在类内，也可以实现在类外。

类内定义的成员函数，默认的都是inline,类外实现的如果需要inline修饰，需要手动加上。

**如何定义常量成员函数？**

C++语言允许把const放在成员函数的参数列表之后。

返回值类型 类名::函数名(参数列表) const;

## 7.2访问控制与封装

**为什么要使用访问说明符来控制访问权限？**

目的是为了加强类的封装性。

访问说明符的种类和作用

| 访问说明符名称 | 作用                                                   |
| :------------: | ------------------------------------------------------ |
|     public     | 整个程序内可被访问                                     |
|    private     | 可以被类的成员函数访问，但是不能被使用该类的代码访问。 |

**同样的都是用户自定义的数据类型struct和class有什么区别？**//这也是我面试的时候，现在的主管问的

使用class和struct定义的类唯一的区别就是默认的访问权。

补充说明：C++中的 struct 和 class 基本是通用的，唯有几个细节不同：

1. 使用 class 时，类中的成员默认都是 private 属性的；而使用 struct 时，结构体中的成员默认都是 public 属性的。
2. class 继承默认是 private 继承，而 struct 继承默认是 public 继承。
3. class 可以使用模板，而 struct 不能。

**什么是友元？**

​	友元**是一种定义在类外部的普通函数或类**，但它需要在类体内进行说明，为了与该类的成员函数加以区别，在说明时前面加以关键字friend。 友元不是成员函数，但是它可以访问类中的私有成员。 友元的作用在于提高程序的运行效率，但是，它破坏了类的封装性和隐藏性，使得非成员函数可以访问类的私有成员。

注意事项：

友元的声明仅仅指定了访问的权限，而非一个通常意义上的声明。如果是友元函数，我们**必须**在友元声明之外再专门声明一次该函数。

## 7.3类的其他特性

直接上码

```C++
#include <iostream>
#include <vector>

class Screen
{
public:
    typedef std::string::size_type pos;
    // using pos = std::string::size_type; //这是另外一种方式
    Screen() = default;
    Screen(pos ht, pos wd, char c = ' ') : height(ht), width(wd), contents(ht * wd, c) { print(); }
    char get() const { return contents[cursor]; } //隐式内联

    /*
        成员函数也可以被重载，只要函数之间的参数数量或者类型上有所区别就行。
    */
    inline char get(pos ht, pos wd) const; //显式内联
    Screen &move(pos r, pos c);            //能在之后被设为内联
    /*
        虽然我们无须在声明和定义的地方同时说明inline,但是这么做是合法的。
        不过最好只在类外部定义的地方说明inline,这样可以使类更容易理解。
        inline成员函数应该与相应的类定义在一个头文件中。
    */

    void some_number() const
    {
        std::cout << "mutable data member value[before]:" << access_ctr << std::endl;
        ++access_ctr;
        std::cout << "mutable data member value[after]:" << access_ctr << std::endl;
    }

    Screen &set(char c)
    {
        contents[cursor] = c;
        print();
        return *this;
    }

    Screen &set(pos h, pos w, char c)
    {
        contents[h * width + w] = c;
        print();
        return *this;
    }

    void print() const
    {
        std::cout << "\nheight :" << height << "\nwidth : " << width
                  << "\ncoursor : " << cursor
                  << "\ncontents : " << contents << std::endl;
    }

    Screen &display(std::ostream &os)
    {
        std::cout << "this is the normal version." << std::endl;
        do_display(os);
        return *this;
    }

    const Screen &display(std::ostream &os) const
    {
        std::cout << "this is the const version." << std::endl;
        do_display(os);
        return *this;
    }

private:
    void do_display(std::ostream &os) const { os << contents; }
    pos cursor = 0;
    pos height = 0, width = 0;
    std::string contents;

    /*
        一个可变数据成员永远不会是const,即使它是const对象成员。
        因此一个const成员函数可以改变一个可变成员的值。
    */
    mutable size_t access_ctr;
};

inline Screen &Screen::move(pos r, pos c)
{
    pos row = r * width;
    cursor = row + c;
    return *this;
}

char Screen::get(pos r, pos c) const
{
    pos row = r * width;
    return contents[row + c];
}

class Window_mgr
{
private:
    std::vector<Screen> screens{Screen(24, 80, 'X')};
};

int main(int argc, char const *argv[])
{
    Screen myscreen, myscreen1(5, 3);
    char ch = myscreen.get();
    ch = myscreen.get(0, 0);
    myscreen.move(4, 0).set('#');
    myscreen.some_number();
    Window_mgr mywindow;

    myscreen1.set('A').move(2, 1).set('B').display(std::cout);
    myscreen1.set('#').display(std::cout).set('X');

    const Screen blank(5, 3);
    blank.display(std::cout);
    
    /*以下语句会报错： passing 'const Screen' as 'this' argument discards qualifiers */
    //blank.display(std::cout).set('x'); 
    /*
        display 的const版本返回的是一个常量引用。
    */
}
```



## 7.4类的作用域

**类的作用域范围**

一个类就是一个作用域。普通的数据成员和函数成员只能由对象/引用/指针通过使用成员访问运算符（./->)来访问。

为什么定义在前面的成员函数内可以使用定义在后面的数据成员/成员函数？

编译器处理完类中的**全部声明**后才会处理成员函数的定义。



**名字查找的过程**

首先，在名字所在的块中查找其声明语句，只考虑在名字的使用之前的出现的声明。

如果没有找到，继续查找外层作用域。

如果最终没有找到匹配的声明，则程序报错。



**类型名要特殊处理**

一般来说，内层作用域中可以重新定义外层作用域中的名字，即使改名字在内层作用域中已经被使用。

然而在类中，如果成员使用了外层作用域中的名字，而该名字代表一种类型，则类不能在之后重新定义该名字；



**成员定义中的普通块作用域的名字查找**

首先，在成员函数中查找该名字的声明，和前面一样，只有在函数使用前的声明才被考虑。

如果在成员函数中没有找到，则在类内继续查找，这是类的所有成员都可以被考虑。

如果类内也没有，在成员函数定义前的作用域中继续查找。

note:一般不建议使用其他成员的名字作为某个成员函数的参数。



**类作用域之后，在外围作用域中查找**

如果编译器在函数和类的作用域中都没有找到名字，它将在外围的作用域中查找。

**在文件中查找名字的出现处对其进行解析**

当成员定义在类的外部时，名字查找的第三步不仅要考虑类定义之前的全局作用域中的声明，还要考虑在成员函数定义之前的全局作用域中的声明。

## 7.5构造函数再探

**为什么建议使用构造函数初始值？**

在很多类中，初始化和赋值的区别事关底层效率问题：前者直接初始化数据成员，后者则先初始化再赋值。

出了效率问题更重要的是，一些数据成员必须被初始化。建议养成使用构造函数初始值的习惯，这样能避免一些意想不到的编译错误，

特别是遇到需要构造函数初始值的成员时。

**构造函数初始值的顺序与成员声明的顺序必须一致？**

没有要求，这个看编译器。但是最好令构造函数初始值的顺序与成员声明的顺序一致。而且如果可能的话，尽量避免使用某些成员初始化其他成员。

**委托构造函数**

【C++11】一个委托构造函数使用它所属类的其他构造函数执行他自己的初始化过程，或者说它把它自己的一些（或者全部）职责委托给了其他构造函数。

**什么是默认构造函数？**

书上的不够，引用一篇博文

https://blog.csdn.net/hankai1024/article/details/7947989?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-1-7947989-blog-72798301.pc_relevant_vip_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-1-7947989-blog-72798301.pc_relevant_vip_default&utm_relevant_index=2

**隐式的类类型转换**

如果构造函数只接受一个实参，则它实际上定义了转换为此类类型的隐式转换机制，有时把这种构造函数称为转换构造函数。

**只允许一步类类型转换**

类类型能定义由编译器自动执行的转换，不过编译器每次只能执行一种类类型的转换。如果同时提出多个转换请求，这些请求将被拒绝。

```C++
class strinit
{
public:
    strinit(std::string str) : _str(str) { std::cout << "strinit init." << std::endl; }
    strinit &XXX(strinit init)
    {
        std::cout << "origin _str:" << _str << std::endl;
        std::cout << "param _str:" << init._str <<  std::endl;
        return *this;
    }

private:
    std::string _str;
};

int main(int argc, char const *argv[])
{
    std::string str("998"); //char->str
    strinit init1(str);
    init1.XXX("999"); //char->str->strinit, 两次转换都想让编译器干，是不行的。
    //init1.XXX(std::string("999")); 
    //init1.XXX(strinit("999"));
    //init1.XXX(str); //str->strinit
    return 0;
}

//报错
// .\test7.cpp:132:20: error: no matching function for call to 'strinit::XXX(const char [4])' 
//      init1.XXX("999"); //?1;31mm^
// .\test7.cpp:117:14: note: candidate: 'strinit& strinit::XXX(strinit)'
//      strinit &XXX(strinit init)
//               ^~~
// .\test7.cpp:117:14: note:   no known conversion for argument 1 from 'const char [4]' to 'strinit'

```

**抑制构造函数定义的隐式转换**

通过将构造函数声明为explicit加以阻止

**explicit使用注意事项**

explicit只对一个实参的构造函数有效

只能再类内声明构造函数时使用，类外定义的时候不应重复。

explicit构造函数只能用于直接初始化。

**什么是聚合类？**
聚合类使得用于可以直接访问其成员，并且具有特殊的初始化语法形式。

**聚合类的条件**

1.所有成员都是public

2.没有定义任何的构造函数

3.没有类内初始值

4.没有基类，也没有virtual函数。

**什么是字面值常量类？**

两种定义：

​	a.数据成员都是字面值类型的聚合类是字面值常量类。

​	b.或者满足以下定义

​		1.数据成员都必须是字面值类型。

​		2.类必须至少含有一个constexpr构造函数。

​		3.如果一个数据成员含有类内初始值，则内置类型的初始值必须是一条常量表达式。或者如果

​			成员属性某种类类型，则初始值必须使用成员自己的constexpr构造函数。

​		4.类必须使用析构函数的默认定义，该成员负责销毁类的对象。

**如何定义一个constexpr构造函数？**

constexpr构造函数可以声明成=defaut的形式或者删除函数的形式。不是前面说的形式，那么就**必须初始化所有数据成员**

constexpr构造函数体**一般来**说**应该**是空的。(试了以下函数体不为空，编译通过)

constexpr构造函数初始化数据成员的方式:**初始值或者使用constexpr构造函数，或一条常量表达式**

## 7.6类的静态成员

直接写一段代码体会一下

```c++
class StaticTest
{
public:
    static int st;
    //static int funcIn() const  //静态成员函数不能声明成const
    static int funcIn()
    {
        st = 234;
        return 111;
    }
    static int funcOut();
    int func11() { return st + funcIn(); } //非静态成员函数可以访问静态成员

private:
    int nor = 33333;
    static int st1;
    static int func1()
    {
        return 222;
    }
    int func12() { return st1 + func1(); }
};

// static int StaticTest::funcOut()  //类外定义类的静态成员函数的时候不能加上static关键字
int StaticTest::funcOut()
{
    // int a = nor;   //静态成员函数不能访问类的非静态成员。
    return st + st1 + func1() + funcIn();
}

//不管是public还是private的静态数据成员，在类内都只有声明。
//static int StaticTest::st = 234;   //类外定义类的静态数据成员时不能加static
int StaticTest::st = 234;
int StaticTest::st1 = 1234;

int main(int argc, char const *argv[])
{
    //int a = StaticTest::st1; //private的静态成员无法通过作用域运算符访问
    //int a = StaticTest::func1();

    int a1 = StaticTest::st;
    int a2 = StaticTest::funcOut();
    StaticTest mystatic;
    StaticTest &mystatic1 = mystatic;
    StaticTest *mystaticp = &mystatic;

    int b= mystatic.funcOut();
    int b1 = mystatic1.funcOut();
    int b2 = mystaticp->funcOut();

    return 0;
}
```

## 小结

类是C++中最基本的特性。

类有两项基本能力：一是数据抽象，二是封装

类可以定义一种特殊的成员函数，构造函数，其作用是控制初始化对象的方式。

类还可以定义可变或者静态成员。

# 附录：

最后附上 Sales_item.h    //网上找的，不一定对，可能有参考价值

```c++
#ifndef SALESITEM_H
#define SALESITEM_H
#include <iostream>
#include <string>
 
class Sales_item
{
public:
    Sales_item(const std::string &book):isbn(book),units_sold(0),revenue(0.0){}
    Sales_item(std::istream &is){ is >> *this;}
    friend std::istream& operator>>(std::istream &,Sales_item &);
    friend std::ostream& operator<<(std::ostream &,const Sales_item &);
public:
    Sales_item & operator+=(const Sales_item&);
public:
    double avg_price() const;
    bool same_isbn(const Sales_item &rhs)const
    {
        return isbn == rhs.isbn;
    }
    Sales_item():units_sold(0),revenue(0.0){}
public:
    std::string isbn;
    unsigned units_sold;
    double revenue;
};
 
using std::istream;
using std::ostream;
Sales_item operator+(const Sales_item &,const Sales_item &);

inline bool operator==(const Sales_item &lhs,const Sales_item &rhs)
{
    return lhs.units_sold == rhs.units_sold && lhs.revenue == rhs.revenue && lhs.same_isbn(rhs);
}

inline bool operator!=(const Sales_item &lhs,const Sales_item &rhs)
{
    return !(lhs == rhs);
}
 
inline Sales_item & Sales_item::operator +=(const Sales_item &rhs)
{
    units_sold += rhs.units_sold;
    revenue += rhs.revenue;
    return *this;
}

inline Sales_item operator+(const Sales_item &lhs,const Sales_item &rhs)
{
    Sales_item ret(lhs);
    ret += rhs;
    return ret;
}

inline istream& operator>>(istream &in,Sales_item &s)
{
    double price;
    in >> s.isbn >> s.units_sold >> price;
    if(in)
        s.revenue = s.units_sold * price;
    else
        s = Sales_item();
    return in;
}

inline ostream& operator<<(ostream &out,const Sales_item &s)
{
    out << s.isbn << "\t" <<s.units_sold << "\t" << s.revenue << "\t" << s.avg_price();
    return out;
}

inline double Sales_item::avg_price() const
{
    if(units_sold)
        return revenue/units_sold;
    else
        return 0;
}
#endif
```

