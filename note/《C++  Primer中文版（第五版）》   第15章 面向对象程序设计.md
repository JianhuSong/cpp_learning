# 《C++  Primer中文版（第五版）》   第15章 面向对象程序设计

面向对象程序设计基于三个基本概念：数据抽象、继承和动态绑定。

继承和动态绑定对程序的编写有两个方面的影响：一是我们可以更容易定义与其他类相似但不完全相同的新类。二是在使用这些彼此相似的类编写程序时，我们可以在一定程度上忽略它们的区别。

## 15.1 OOP:概述

数据抽象：将类的接口与实现分离

继承：可以定义相似的类型并对其关系建模

动态绑定：可以在一定程度上忽略相似类型的区别，而一统一的方式使用它们的对象。

基类：

通过继承联系在一起的类构成一种层次关系。通常在层次关系的根部有一个基类。

派生类：

继承得到的类称为派生类。

虚函数：

对于某些函数，基类希望它的派生类各自定义适合自身的版本，此时基类就将这些函数声明成虚函数。

```cpp
class Quote
{
public:
	std::string isbn() const;
    virtual double net_price(std::size_t n) const;   //这里能够看出虚函数的声明方式了
};
```

如何定义一个派生类？

派生类必须通过类派生列表明确指出他是从哪个（哪些）基类继承而来。

```cpp
class Bulk_quote : public Quote //类派生表的形式：首先是一个冒号，后面紧跟以逗号分隔的基类列表（）
{
public:
    double net_price(std::size_t) const override;
}
```



## 15.2 定义基类和派生类

```cpp
/*
    在C++语言中，基类必须将它的两种成员函数区分开：
        1.基类希望其派生类进行覆盖的函数，这类函数通常将其定义为虚函数。（就是只在函数的内部声明前加上关键字vritual）
        2.基类希望派生类直接继承而不要改变的函数
    当我们使用指针或引用调用虚函数时，该调用将被动态绑定。根据引用或指针所绑定的对象类型不同，该调用可能执行基类的版本，也可能执行某个派生类的版本。
*/
class Quote
{
public:
    Quote() = default;
    Quote(const std::string &book, double sales_price) : bookNo(book), price(sales_price) {}
    std::string isbn() const { return bookNo; }                         // 这中就是希望派生类直接继承的
    virtual double net_price(std::size_t n) const { return n * price; } // 这种就是希望派生类进行覆盖的
    virtual ~Quote() = default;                                         // 基类通常都应该定义一个虚析构函数，对析构函数进行动态绑定
private:
    std::string bookNo;
protected:
    double price = 0.0;
};
```

 定义派生类：
    	派生类必须通过使用类派生列表（class derivation list）明确指出它是哪个（哪些）基类继承而来的。
    	形式：首先是一个冒号，后面紧跟以逗号分隔的基类列表，其中每个基类前面有访问说明符（public/protected/private）。
    	只继承自一个类的这种形式称为“单继承”，继承自多个类的形式称为“多继承”
    	访问说明符的作用是控制派生类从基类继承而来的成员是否对派生类的用户可见。

```cpp
class Bulk_quote : public Quote
{
public:
    Bulk_quote() = default;
    Bulk_quote(const std::string &book, double sales_price, std::size_t qty, double dis) : 
    			Quote(book, sales_price), min_qty(qty), discount(dis) {}
    
    //派生类必须将其继承来的成员函数中那些需要被覆盖的函数重新声明（试了下不定义该函数，编译也不会报错)
    double net_price(std::size_t) const override; 

private:
    std::size_t min_qty = 0;
    double discount = 0.0;
};
```

派生类中的虚函数
    	派生类经常（但不总是）覆盖它继承的虚函数（书中曾提到“派生类必须将其继承来的成员函数中那些需要被覆盖的函数重新声明”）
        那些需要被覆盖的函数不是虚函数么？ 目前不知道，以后可能会知道,有没有可能是书上写错了
        如何显示地注明使用某个成员函数覆盖它继承的虚函数？
        		该函数必须与所继承的虚函数名称相同，参数列表相同，返回类型相同。
            	名字不同：marked 'override', but does not override
            	返回值不同：conflicting return type specified
            	形参列表不同：candidate expects 2 arguments, 1 provided
            	在该函数的声明的最后加上override。

派生类对象及派生类向基类的类型转换
    	因为在派生类对象中含有与其基类对应的组成部分，所以我们能把派生类对象当成基类对象来使用，而且我们也能将基类的指针或引用绑定到
        派生类对象中的基类部分上。

​        编译器会隐式的执行派生类到基类的类型转换，这就意味着我们可以在需要基类引用/指针的地方使用派生类的对象或引用/派生类对象指针。

```cpp
Qoute item;
Bulk_quote bulk;
Quote *p = &item;
p = &bulk;
Quote &r = bulk;
```

派生类构造函数

​		派生类对象中含有从基类继承而来的成员，但是派生类并不能直接初始化这些成员。

​		派生类也必须使用基类的构造函数来初始化它的基类部分（每个类控制他自己的成员初始化过程）。

​		派生类对象的基类部分与派生类对象自己的数据成员都是在构造函数的初始化阶段执行初始化操作的。

​		首先初始化基类的部分，然后按照声明的顺序依次初始化派生类的成员。

派生类使用基类的成员

​		派生类可以访问基类的公有成员和受保护的成员。

​		遵循基类的接口

​				每个类负责定义各自的接口。要想与类的对象教会必须使用该类的接口。即使这个对象是爬升类的基类部分也是如此。

继承与静态成员

​		如果基类定义了一个静态成员，则在整个继承系体中只存在该成员的唯一定义。

​		不论从基类派生出来多少个派生类，对于每个静态成员来说都只存在唯一的实例。

​		静态成员遵循通用的访问控制规则。

​		派生类的声明

​				派生类的声明与其他类差别不大。

​    			被用作基类的类

​						一个类是基类，同时它也可以是一个派生类。	

​						直接基类出现在派生列表中，而间接基类由派生类通过其直接基类继承而来。

```cpp
class Base {};
class D1:public Base {};
class D2:public D1 {};
/*
	在这样的继承关系中，Base 是D1的直接基类，同时是D2的间接基类。
*/
```

​    			防止继承的发生

​						在类名后面跟一个final可以阻止其它类继承它。（换句话说，它不能作为基类了）

```cpp
#include <iostream>
#include <string>

/*
    在C++语言中，基类必须将它的两种成员函数区分开：
        1.基类希望其派生类进行覆盖的函数，这类函数通常将其定义为虚函数。（就是只在函数声明前加上关键字vritual）
        2.基类希望派生类直接继承而不要改变的函数
*/

class Quote
{
public:
    Quote() = default;
    Quote(const std::string &book, double sales_price) : bookNo(book), price(sales_price) {}
    std::string isbn() const { return bookNo; }                         // 这中就是希望派生类直接继承的
    virtual double net_price(std::size_t n) const { return n * price; } // 这种就是希望派生类进行覆盖的
    virtual ~Quote() = default;                                         // 基类通常都应该定义一个虚析构函数
private:
    std::string bookNo;

protected:
    double price = 0.0;
};

class Bulk_quote : public Quote
{
public:
    Bulk_quote() = default;
    Bulk_quote(const std::string &book, double sales_price, std::size_t qty, double dis) : 
               Quote(book, sales_price), min_qty(qty), discount(dis) {}

    // 派生类必须将其继承来的成员函数中那些需要被覆盖的函数重新声明（试了下不定义该函数，编译不会报错）
    double net_price(std::size_t n) const override 
    {
        return n * price - discount * min_qty;
    }

private:
    std::size_t min_qty = 0;
    double discount = 0.0;
};

#if 0
class My_quote final: public Quote
{
};

//cannot derive from 'final' base 'My_quote' in derived type 'My_quote1'
class My_quote1:public My_quote
{};
#endif
int main(int argc, char const *argv[])
{
    Bulk_quote book1("Book no.1", 123.45, 12, 2.34);
    std::cout << book1.net_price(123) << std::endl;
    Quote item;
    Bulk_quote bulk;
    Quote *p = &item;
    p = &bulk;
    Quote &r = bulk;
    return 0;
}

```



类型转换与继承

​	可以将基类的指针或引用绑定到派生类对象上

​	当使用基类的引用（或指针）时，实际上我们并不清楚该引用（或指针）所绑定对象的真实类型。该对象可能是基类的对象，也可能是派生类的对象。

​    静态类型与动态类型

​		一个变量或其他表达式在编译和运行时分为两种类型：

​		静态类型

​			表达式的静态类型是在编译时总是已知的。

​		动态类型

​			动态类型则是变量或表达式在内存中的对象的类型。

​		如果表达时既不是引用也不是指针，则它的动态类型与静态类型一致。

​		基类的指针或引用的静态类型可能与其动态类型不一致。

​	不存在从基类向派生类的隐式类型转换

​	派生类向基类的自动类型转换只对指针或引用类型有效。		

## 15.3 虚函数

我们必须为每一个虚函数都提供定义，而不管它是否被用到了，这是因为连编译器也无法确定到底会用哪个虚函数。

对虚函数的调用可能在运行时才被解析

被调用的函数始于绑定到指针或引用上的对象的动态类型相匹配的那一个。

动态绑定只有当我们通过指针或引用调用虚函数时才会发生。

派生类中的虚函数

​	某个函数被声明成虚函数，则在所有派生类中它都是虚函数。

​	一个派生类函数如果覆盖了某个继承而来的虚函数，则它的形参类型必须与被它覆盖的基类函数一致（这个是需要严格匹配的）。

​	派生类中虚函数的返回类型也必须与基类函数匹配。（当类的虚函数返回类型时类本身的指针或引用时，返回类型可以不一样） 

```cpp
class Base
{
    public:
    virtual Base& base_return(int i) {return *this;}
    virtual Base* base_return1(int j) {return this;}
    virtual ~Base() = default;
};

class D : public Base
{
    public:
    D* base_return1(int i) {return this;}
    D& base_return(int j) {return *this;}
};
//如果基类中的虚函数返回的是基类的指针/引用，那么派生类中同名同参数的应该返回派生类的指针/引用
//如果返回值类型不匹配error: conflicting return type specified
```

final和override说明符

​	派生类中如果定义了一个函数与基类中虚函数的名字相同但是形参列表不同，这是合法的行为，但是我们实际想要的是覆盖基类中的虚函数。使用override可以	让编译器检出这种错误

​	使用final可以将函数标记为不可覆盖的

​	final和override说明符出现在形参列表（包括任何const或引用修饰符）以及尾置返回类型之后。

虚函数与默认实参

​	虚函数可以有默认实参

​	如果我们通过基类的引用或指针调用函数，则使用基类定义中的默认实参，即使实际运行的是派生类中的函数版本也是如此。

​	如果虚函数使用默认实参，则基类和派生类中的定义默认实参最好一致。

```cpp
class Base
{
public:
    virtual int base_return(int i, int j = 11)
    {
        std::cout << "Using base::base_return!!" << std::endl;
        return i * j;
    }

    virtual ~Base() = default;
};

class D : public Base
{
public:
    int base_return(int j, int g = 200)
    {
        std::cout << "Using D::base_return!!" << std::endl;
        return j + g;
    }
};

void test(Base &base)
{
    std::cout << base.base_return(100) << std::endl;
    std::cout << base.Base::base_return(100) << std::endl;
}

int main(int argc, char const *argv[])
{
    D d;
    test(d);
    return 0;
}

// 输出结果可以知道，默认实参一直都是用的基类虚函数的。
//Using D::base_return!!
//111
//Using base::base_return!!
//1100
```

回避虚函数的机制

​	使用作用域运算符（::）可以强制执行虚函数的某个特定版本。

​	通常情况下，只有成员函数（或友元）中的代码才需要使用作用域运算符来回避虚函数的机制。

​    什么时候我们需要回避虚函数的默认机制呢?

​			通常是一个派生类的虚函数调用覆盖的基类的虚函数版本时，在此情况下，基类版本通常完成继承层次中所有类型都要做的共同任务，而派生类定义的版本		需要执行一些与派生类本身密切相关的工作。

## 15.4 抽象基类

将所有子类中重复最多的数据和接口抽象出来组成一个基类，被称为抽象基类。//这个概念太笼统

根据可不可以把类实例化分为：

​	不能实例化的抽象类

​	能实例化的具体类/实现类

抽象类的唯一目的就是让其派生类继承并实现它的接口方法发，因此也通常称为抽象基类。

纯虚函数

​	在虚函数的声明的分号前加一个“=0”，=0只能出现在类内部的虚函数声明处。

​	一个纯虚函数无须定义。

含有纯虚函数的类是抽象基类

​	抽象基类负责定义接口，而后续的类来实现接口。

​	不能（直接）创建一个抽象基类的对象。

​	抽象基类的派生类必须给出自己的相关接口的定义，否则其自身仍是抽象基类。

派生类构造函数只初始化它的直接基类

## 15.5 访问控制与继承

访问控制与继承

​	每个类分别控制这其成员对于派生类来说是否可访问	

​	受保护的成员

​		使用protected关键字来声明那些它希望与派生类分享但是不想被其他公共访问使用的成员。

​		受保护的成员对于类的用于来说是不可访问的

​		受保护的成员对于派生类的成员和友元来说是可访问的

​		派生类的成员或友元只能通过派生类对象来访问基类受保护成员。派生类对于一个基类对象中的受保护成员没有任何访问权限。

​	公有、私有和受保护继承

​		某个类对其继承而来的成员的访问权限受到两个因素的影响：

​			1.基类中该成员的访问说明符

​			 2.派生类的派生列表中的访问说明符

​		对基类成员的访问权限只与基类中的访问说明符有关。

​		派生类访问说明符的目的是控制派生类用户（包括派生类的派生类在内）对于基类成员的访问权限。

```cpp
class Base
{
public:
    void pub_men();

protected:
    int prot_mem;

private:
    char priv_mem;
};

struct Pub_Derv : public Base
{
    int f() { return prot_mem; }

    // 错误:private 成员对于派生类来说是不可访问的。
    // 编译也会报错：'char Base::priv_mem' is private within this context
    char g() { return priv_mem; }
};

struct Priv_Derv : private Base
{
    // private 继承不会影响派生类的访问权限
    int f1() { return prot_mem; }
};
```

​	派生类向基类转换的可访问性

​		由使用该转换的代码决定，同时派生生的派生访问说明符也会有印象。假定D继承自B

​			只有当D公有继承B时，用户代码才能使用派生类向基类的转换；

​			不论D以什么方式继承B,D的成员函数和友元都能使用派生类向基类的转换；

​			如果D继承B的方式是公有的或者受保护的，则D的派生类的成员和友元可以使用D向B的转换。

```cpp
class Base
{
private:
    int priv = 1;

protected:
    int port = 0;

public:
    Base(/* args */) = default;
    ~Base() = default;
    int return_priv() { return priv + port; }
};

class Derv1 : public Base
{
private:
    /* data */
public:
    Derv1(/* args */) = default;
    virtual ~Derv1() = default;
    friend void d1_friend();
    void pub_d_b()
    {
        Derv1 d1;
        Base &b1 = d1;
    }
};

class Derv2 : private Base
{
private:
    /* data */
public:
    Derv2(/* args */) = default;
    ~Derv2() = default;
    friend void d2_friend();
    void priv_d_b()
    {
        Derv2 d2;
        Base &b2 = d2;
    }
};

class Derv3 : protected Base
{
private:
    /* data */
public:
    Derv3(/* args */) = default;
    ~Derv3() = default;
    friend void d3_friend();
    void prot_d_b()
    {
        Derv3 d3;
        Base &b3 = d3;
    }
};

void d1_frined()
{
    Derv1 d1;
    Base &b1 = d1;
}
void d2_friend()
{
    Derv2 d2;
    Base &b2 = d2;
}
void d3_friend()
{
    Derv3 d3;
    Base &b3 = d3;
}

class D1_1_pub: public Derv1
{
private:
    /* data */
public:
    D1_1_pub(/* args */) = default;
    ~D1_1_pub() = default;
    void d_b()
    {
        Derv1 d1;
        Base &b1 = d1; //无报错
    }
};

class D1_1_priv: private Derv1
{
private:
    /* data */
public:
    D1_1_priv(/* args */) = default;
    ~D1_1_priv() = default;
     void d_b()
    {
        Derv1 d1;
        Base &b1 = d1; //无报错
    }
};

class D1_1_prot: protected Derv1
{
private:
    /* data */
public:
    D1_1_prot(/* args */) = default;
    ~D1_1_prot() = default;
    void d_b()
    {
        Derv1 d1;
        Base &b1 = d1;    //无报错
    }
};

class D2_2_pub: public Derv2
{
private:
    /* data */
public:
    D2_2_pub(/* args */) = default;
    ~D2_2_pub() = default;
    void d_b()
    {
        Derv2 d2;
        Base &b2 = d2; //class Base Base::Base' is inaccessible within this context
    }
};

int main(int argc, char const *argv[])
{
    Derv1 D1;
    Derv2 D2;
    Derv3 D3;
    Base &b1 = D1;
    Base &b2 = D2;  //直接报错：Base' is an inaccessible base of 'Derv2'
    Base &b3 = D3;  //'Base' is an inaccessible base of 'Derv3'

    return 0;
}
```

友元与继承

​	友元关系不能传递，不能继承。

​	当一个类将另外一个类声明成友元时，这种友元关系只对做出声明的类有效。对于原来的那个类来说，其友元的基类或者派生类不具有特殊的访问能力。

改变个别成员的可访问性

​	通过在类的内部使用using声明语句，我们可以将该类的直接或间接基类中的任何可访问成员（）标记处理啊。using声明语句中的名字的访问权限由该using声	明语句之前的访问说明符来决定。

​	派生类只能为那些它可以访问的名字提供using声明。

默认的继承保护机制

​	默认情况下，使用class关键字定义的派生类是私有继承的；而使用struct关键字定义的派生类是公有继承的。

## 15.6 继承中的作用域

每个类定义自己的作用域，在这个作用域内我们定义类的成员。当存在继承关系时，派生类的作用域嵌套在其基类的作用域内。

在编译时进行名字查找

​	一个对象、引用或指针的静态类型决定了该对象的那些成员是可见的。

名字冲突与继承

​	派生类的成员将隐藏同名的基类成员

通过作用域运算符来使用隐藏的成员

​	我们可以通过作用域运算符来使用一个被隐藏的基类成员

除了覆盖继承而来的虚函数之外，派生类最好不要重用其他定义在基类中的名字。

一如往常，名字查找先于类型检查

​	声明在内层作用域的函数并不会重载声明在外层作用域的函数。

虚函数与作用域

​	基类和派生类的虚函数接受的实参不同，无法通过基类的引用或指针调用派生类的虚函数。

通过基类调用隐藏的虚函数

```cpp
#include <iostream>

class Base
{
private:
    /* data */
public:
    Base(/* args */) = default;
    virtual ~Base() = default;
    virtual int fcn() { return -1; }
};

class D1 : public Base
{
public:
    // 隐藏基类的fcn,形参列表不同
    // D1继承了Base::fcn的定义。
    int fcn(int param) { return param; }  
    virtual void f2(){};
};

class D2 : public D1
{
public:
    int fcn(int param) { return param + 1; } //是一个非虚函数，隐藏了D1::fcn(int)
    int fcn() { return 2; }                  //覆盖了Base的虚函数fcn
    void f2() {}                             //覆盖了D1的虚函数f2
};

int main(int argc, char const *argv[])
{
    D1 d1obj;
    D2 d2obj;
    Base bobj, *bp1 = &bobj, *bp2 = &d1obj, *bp3 = &d2obj;
    bp1->fcn();     //虚调用，将在运行时调用Base::fcn
    bp2->fcn();     //需调用，将在运行时调用Base::fcn
    bp3->fcn();     //虚调用，将在运行时调用D1::fcn

    D1 *d1p = &d1obj;
    D2 *d2p = &d2obj;
    //bp2->f2();     // 'class Base' has no member named 'f2'
    d1p->f2();       //虚调用，将在运行时调用D1::f2()
    d2p->f2();       //需调用，将在运行时调用D2::f2()
    return 0;
}

```

覆盖重载的函数

​	成员函数无论是否是虚函数都可以被重载。

​	如果派生类希望所有的重载版本对他来说都是可见的，那么它就需要覆盖所有的版本，或者一个也不覆盖。

​	一条基类成员函数的using声明语句就可以把该函数的所有重载实例添加到派生类作用域中。（本身就可以通过作用域运算符来调用基类的同名成员函数，使用using的目的是为了不使用作用域运算符么？）

```cpp
class Base
{
public:
    Base() = default;
    virtual ~Base() = default;

    //不管是不是虚函数都可以重载
    virtual int func(int a) { return a++; }
    virtual int func(int a, int b) { return a + b; }
    virtual int func(int a, int b, int c) { return a + b + c; }
};

class Derv : public Base
{
public:
    Derv() = default;
    ~Derv() = default;
    // using Base::func;
    // int func(int a) { return a + a + 1; } // 基类中的同名函数被隐藏，基类中的同名函数对Derv来说就是不可见的
    int test()
    {
        // return func(2,3);       //如果不使用using, no matching function for call to 'Derv::func(int, int)'
        // return Base::func(2,3); //可以使用作用域运算符让基类的func(int,int)可见
        return func(2, 3);         // 不定义任何与基类成员函数同名的函数，那么基类中的所有func对Derv来说就是可见的
    }
};
```

## 15.7 构造函数域拷贝控制

如果一个类（基类或派生类）没有定义拷贝控制操作，则编译器将为它合成一个版本。

虚析构函数

​	基类通常应该定义一个虚析构函数

通过在基类中将析构函数定义成虚析构函数以确保执行正确的析构函数版本。

如果一个类需要析构函数，那么它也同样需要拷贝和赋值操作这条规则不适用于虚析构函数。

虚析构函数将阻止合成移动操作

​	如果一个类定义了析构函数，编译器也不会为其合成移动操作。

合成拷贝控制与继承

​	合成的成员还负责使用基类中对应的操作对要给对象的直接基类部分进行初始化。

​	无论是基类成员是合成的版本还是自定义的版本，都要求相应的成员可以访问并且不是一个被删除的 函数。

派生类中删除的拷贝控制与基类的关系

​	定义基类的方式可能导致有的派生类成员成为被删除的函数

​		如果基类中的默认构造函数、拷贝赋值运算符或析构函数是被删除的函数或是不可访问的，则派生类中对应的成员将是被删除的。

​		如果基类中有一个不可访问或删除掉的析构函数，则派生类中合成的默认和拷贝构造函数将是被删除的。

​		编译器不会合成一个删除掉的移动操作。当我们使用=default请求一个移动操作时，如果基类中对应操作是删除的或是不可访问的，那么派生

​		类中该函数将是被删除的。如果基类的析构函数是删除的或是不可访问的，则派生类中的移动构造函数也将是被删除的。

移动操作与继承

​	定义了一个虚析构函数，基类通常不含有合成的移动操作，并且它的派生类中也没有合成的移动操作。如果需要那么首先在基类中定义，若派生类中没有排斥移动的成员，那么它将自动获得合成的移动操作。

​	派生类的拷贝控制成员

​		当派生类定义了拷贝或移动操作时，该操作负责拷贝或移动包括基类部分成员在内的整个对象。

定义派生类的拷贝或移动构造函数

​	为派生类定义拷贝或移动构造函数时，通常使用对应的基类构造函数初始化对象的基类部分。

在默认情况下，基类默认构造函数初始化派生类对象的基类部分。如果我们向拷贝（移动）基类部分，则必须在派生类的构造函数初始值列表中显式的使用基类的拷贝（移动）构造函数。

派生类赋值运算符

​	派生类的赋值运算符必须显式地为其基类部分赋值。

派生类析构函数

​	派生类析构函数只负责销毁由派生类自己分配的资源。

​	对象的销毁顺序与其创建的顺序相反。

在构造函数和析构函数中调用虚函数

​	如果构造函数或析构函数调用了某个虚函数和，则我们应该执行与构造函数或析构函数所属类型相对应的虚函数版本。

继承的构造函数

​	派生类能够重用其直接基类定义的构造函数。

​	类不能继承默认、拷贝和移动构造函数。

​	当using声明语句作用于构造函数时，using声明语句将令编译器产生代码。对于基类的每一个构造函数，编译器都生成一个与之对应的派生类构造函数。

继承的构造函数的特点

​	一个构造函数的using声明不会改变该构造函数的访问几倍。

​	一个using声明不能指定explicit或constexpr,如果基类的构造函数是explicit/constexpr，则继承的构造函数也有相同的属性。

​	当一个基类构造函数含有默认实参时，这些参数并不会被继承。相反派生类将获得多个继承的构造函数，其中每个构造函数分配省略掉一个含有默认实参的形参。

​	如果基类含有几个构造函数，则除了两个例外，大多数时候派生类会继承所有这些构造函数。

​		第一个例外：派生类继承一部分，而另外一部分定义自己的版本

​		第二个例外：默认、拷贝和移动构造函数不会被继承。

## 15.8 容器与继承

当我们使用容器存放继承体系中的对象时，通常必须采取间接存储的方式。

当派生类对象被赋值给基类对象时，其中的派生类部分将被“切掉”，因此容器和存在继承关系的类型无法兼容。

在容器中放置指针而非对象

​	当我们希望在容器中存放具有继承关系的对象时，我们时间上存放的通常是基类的指针（更好的选择是智能指针）。

```cpp
//quote.h
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
```

```cpp
//disc_quote.h
#ifndef __DISC_QUOTE_H__
#define __DISC_QUOTE_H__
#include "quote.h"

class Disc_quote : public Quote
{
public:
    Disc_quote() = default;
    Disc_quote(const std::string &book, double price,
               std::size_t qty, double disc) : Quote(book, price), quantity(qty), discount(disc) {}
    double net_price(std::size_t) const = 0;
    std::pair<std::size_t, double> discount_policy() const
    {
        return {quantity, discount};
    }

protected:
    std::size_t quantity = 0;
    double discount = 0.0;
};

#endif // !__DISC_QUOTE_H__
```



```cpp
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

```

```cpp
//chapter15_8.cpp
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
    double sum = bsk.total_receipt(std::cout);    return 0;
}
```

## 15.9 文本查询程序再探

继承与组合

​	当我们令一个类公有地继承另一个类时，派生类应当反映与基类的“是一种（Is A）”关系。在设计良好的类体系中，公有派生类的对象应该可以用在任何需要基类对象的地方。

​	类型之间的另一种常见的关系是“有一个（Has A）”关系，具有这种关系的类暗含成员的意思。

