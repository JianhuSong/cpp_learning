# 第14章 重载运算与类型转换

C++语言定义了大量运算符以及内置类型的i自动转换规则。这些特性使得程序员能编写出形式丰富、含有多种混合类型的表达式。

当运算符被用于类类型的对象时，C++语言允许我们为其指定新的含义；同时，我们也能自定义类类型之间的转换规则。和内置类型一样，类类型转换隐式地将一种类型的对象转换成我们所需类型的对象。

## 14.1 基本概念

重载的运算符是具有特殊名字的函数：它们的名字由关键字operator和其后面要定义的运算符号共同组成。和其他函数一样，重载的运算符也包含返回类型，参数列表以及函数体。

如何决定重载运算符函数的参数个数？

​	重载运算符函数的参数数量应该与该运算符作用的运算对象数量一样多。

​	当一个重载的运算符是成员函数时，this绑定到左侧运算对象。成员运算符函数的参数数量比运算对象的数量少一个。

 参数与运算对象的对应关系？

​	对于二元运算符来说，左侧运算对象传递给第一个参数，而右侧运算对象传递给第二个参数。

重载运算符函数能够有默认参数么？

​	除了重载的函数运算符operator()外，其他重载运算符函数都不能有默认实参。

重载运算符对运算对象有什么要求？

​	当运算符作用于内置类型的对象时，我们无法改变该运算符的含义。

重载运算符对运算符有什么要求？

​	可以重载大部分的运算符。

​	只需记住不能重载的就行：（:: .* . ?:），括号中的是不能重载的

​	只能重载已经存在的运算符。（重载本身的含义就是重新定义，本来就没有，哪来的重新定义？//个人理解）

可不可以直接调用重载的运算符？

​	可以

什么情况下将操作定义为重载的运算符？

​	如果某些操作逻辑上与运算符有关，则它们适合于定义成重载的运算符

定义重载的运算符有哪些基本守则？

​	如果类执行IO操作，则定义位移运算符使其与内置类型的IO保持一致。

​	如果类的某个操作时检查相等性，则定义operator==;如果类有了operator==,意味着它应该有operator!=

​	如果类包含一个内在的单序比较操作，则定义operator<;如果类有了operator<,则它也应该包含有其他关系操作。

​	重载运算符的返回类型通常情况下应该与其内置版本的返回类型兼容；逻辑运算符和关系运算符应该返回bool,算术运算符应该返回该类类型的值，赋值运算符和复合赋值运算符应该返回左侧运算对象的一个引用。

如何判断是将运算符定义为成员函数还是普通的非成员函数？

​	赋值（=）、下标（[]）、调用（( )）和成员访问箭头（->）运算符必须是成员函数。

​	复合赋值运算符一般来说应该是成员，但非必须。

​	改变对象状态的运算符或者与给定类型密切相关的运算符，如递增，递减和解引用运算符，通常应该是成员。

​	具有对称性的运算符可能转换任意一端的运算对象，例如算术、相等性、关系和位运算符等，因此它们通常应该是普通的非成员函数。

  把运算符定义成类成员函数时对参与运算的对象有什么要求？

​	左侧运算对象必须时运算符所属类的一个对象。

简单写一段：

```cpp
#include <iostream>

class myOperator
{
public:
    myOperator() : a(0), b(0), objname("") { std::cout << "called myOperator()" << std::endl; }
    myOperator(std::string name) : a(0), b(0), objname(name) { std::cout << "called myOperator(std::string name)" << std::endl; }
    myOperator(int a, int b, std::string name = "") : a(a), b(b), objname(name)
    {
        std::cout << "called myOperator(int a, int b, std::string name = \"\")" << std::endl;
    }

    ~myOperator() = default;

    myOperator operator+(myOperator b1)
    {
        std::cout << "operator+ called." << std::endl;
        myOperator tmp("tmp");
        tmp.a = a + b1.a;
        tmp.b = b + b1.b;
        tmp.myOperatorPrint();
        return tmp;
    }

    myOperator operator-(myOperator b1)
    {
        std::cout << "operator+-called." << std::endl;
        myOperator tmp;
        tmp.a = a - b1.a;
        tmp.b = b - b1.b;
        tmp.myOperatorPrint();
        return tmp;
    }

    void operator=(myOperator b1)
    {
        std::cout << "operator= called." << std::endl;

        a = b1.a;
        b = b1.b;
        myOperatorPrint();
    }

    void myOperatorPrint()
    {
        std::string prefix = objname.empty() ? std::string("") : objname + std::string(".");
        std::cout << prefix << "a = " << a << " " << prefix << "b = " << b << std::endl;
    }

private:
    int a = 0;
    int b = 0;
    std::string objname = "";
};

myOperator operator+(myOperator a, int i)
{
    myOperator tmp1, tmp(i, 0);
    tmp1 = a + tmp;
    return tmp1;
}

int main(int argc, char const *argv[])
{
    myOperator a("a"), b(1, 23, "b"), c(22, 0, "c");
    a = b + c;
    std::cout << "after operator+" << std::endl;
    a.myOperatorPrint();
    b.myOperatorPrint();
    c.myOperatorPrint();
    a = b - c;
    std::cout << "after operator-" << std::endl;
    a.myOperatorPrint();
    b.myOperatorPrint();
    c.myOperatorPrint();
    return 0;
}

```



## 14.2 输入和输出运算符

重载输出运算符（<<）有哪些注意事项？

通常情况下，输出运算符的第一个形参时要给非常量ostream对象的引用。

第二个形参一般来说是一个常量的引用，该常量是我们想要打印的类类型。

打印对象不会改变对象的内容。

为了与其他输出运算符保持一致，operator<<一般要返回它的ostream形参。

输出运算符尽量减少格式化操作

输入输出运算符必须是非成员函数。（原因是：若输入输出运算符是某个类的成员，则它们也必须是istream/ostream的成员。我们无法给标准库中的类添加成员）

按照上述要求写一个：

```cpp
std::ostream& operator<<(std::ostream &os, const myOperator &print)
{
    os << print.a << " " << print.b;  //输出运算符尽量减少格式化输出，所以这里就不写std::endl了。 
    return os;
}

```

重载输入运算符(>>)有哪些注意事项？

通常情况下，输入运算符的第一个形参是运算符将要读取的流的引用，第二个形参是将要读入到（非常量）对象的引用。

该运算符通常会返回某个给定流的引用。

输入运算符必须处理输入可能失败的情况

输入运算符应该设置流的条件状态以标示出失败信息。

按照上述要求写一个：

```cpp
std::istream & operator>>(std::istream &is, myOperator &in)
{
    myOperator tmp;
    is >> tmp.a >> tmp.b;  //为了能够直接访问类的数据成员，将该函数定义为myOperator的友元函数。
    
    //读取过程没有出错的化就为in赋新值。
    if(is)
    {
        in = tmp;
    }

    return is;
}
```



## 14.3 算术与关系运算符

定义算术和关系运算符时有哪些注意事项？

通常情况下我们把算术和关系运算符定义成非成员函数。

算术运算符通常会计算它的两个运算对象并得到一个新值，这个值有区别与任意一个运算对象，常常位于一个局部变量之内，操作完成后返回该局部变量的副本作为其结果。

如果定义了算术运算符一般也会定义一个对应的复合赋值运算符。（干脆直接定义一个赋复合赋值运算符）

如果类定义了算术运算符和相关的复合赋值运算符，则通常情况下应该使用复合赋值运算符实现算术运算符。

定义相等（==）运算符有哪些设计准则？

如果一个类含有判断两个对象是否相等的操作，则它显然应该把函数定义为operator==而非一个普通的命名函数

如果类定义了operator==,则该运算符应该能够判断一组给定的对象中是否含有重复数据。

通常情况下相等运算符应该具有传递性。（若a==b且b==c成立，那么a==c成立）

如果类定义了operator==,则这个类应该定义operator!=，反之亦然。

相等运算符和不等运算符应该只有一个负责实际的对象比较工作。另外一个调用那个工作的实现其功能。

如果一个类在逻辑上有相等性的含义，则该类应该定义operator==

定义关系运算符有哪些准则？

定义顺序关系，令其与关联容器中对关键字的要求一致。

如果类同时也含有==运算符的话，则定义一种关系令其与==保持一致。

如果存在唯一一种逻辑可靠的<定义，则应该考虑为这个类定义<运算符。

```cpp
    bool operator==(const myOperator &right)
    {
        return a == right.a && b == right.b;
    }

    bool operator!=(const myOperator &right)
    {
        return !(*this == right);
    }

    bool operator<(const myOperator &right)
    {
        return (a < right.a) && (b < right.b);
    }

    bool operator<=(const myOperator &right)
    {
        return *this < right || *this == right;
    }

    bool operator>(const myOperator &right)
    {
        return !(*this <= right);
    }

    bool operator>=(const myOperator &right)
    {
        return *this > right || *this == right;
    }
```



## 14.4 赋值运算符

赋值运算符的要求？

我们可以重载赋值运算符，不论形参的类型是什么，赋值运算符都必须定义为成员函数。

重载的赋值运算符必须先释放当前内存空间。

复合赋值运算符的要求？

为了与内置类型的复合赋值保持一致，类中的复合赋值运算符也要返回其左侧运算对象的引用。

## 14.5 下标运算符

定义下标运算符时有哪些注意事项？

下标运算符必须是成员函数

下标运算通常以所访问的元素的引用作为返回值

最好同时定义下标运算的常量版本(常量成员并返回常量引用)和非常量版本。

## 14.6 递增和递减运算符

定义递增或递减运算符时有哪些注意事项？

建议将其设定为类的成员函数。

定义时应该同时定义前置版本和后置版本。

前置版本应该返回递增或递减后对象的引用。

后置版本应该返回原值(是一个值而非引用)

如何在定义上区分前置版本和后置版本？

后置版本比前置版本多一个额外的（不被使用）int类型的形参。

```cpp
 //不带参数的是前置版本，前置版本返回递增后的对象的引用
    myOperator& operator++()
    {
        a++;
        b++;
        return *this;
    }

    //后置版本带一个不使用的形参，返回自增前的值；
    myOperator operator++(int)
    {
        myOperator ret = *this;   //记录计算前的值
        ++*this;                  //调用前置版本，实现自增功能。
        return ret;               //返回计算前的值。
    }

    myOperator& operator--()
    {
        a--; 
        b--;
        return *this;
    }

    myOperator operator--(int)
    {
        myOperator ret = *this;
        --*this;
        return ret;
    }
```



## 14.7 成员访问运算符

定义成员访问运算符时有哪些注意事项？

箭头运算符必须是成员函数。

解引用运算符通常也是类的成员。

箭头运算符永远不能丢掉成员访问这个最基本的含义。

重载的箭头运算符必须返回类的指针或者自定义了箭头运算符的某一个类的对象。

## 14.8 函数调用运算符

定义函数调用运算符时有哪些注意事项？

函数调用运算符必须是成员函数。

一个类可以定义多个不同版本的调用运算符，相互之间应该在参数数量或类型上有所区别。

```cpp
class FuncOperator
{
    public:
        int operator()()
        {
            return 22;
        }

        int operator()(int a)
        {
            return a + 22;
        }
};
```

lambda表达式是函数对象。

lambda表达式产生的类不含有默认构造函数、赋值运算符以及默认析构函数；它是否含有默认的拷贝/移动构造函数则通常要视捕获的数据成员类型而定。



标准库定义的函数对象

标准库定义了一组表示算术运算符、关系运算符和逻辑运算符的类，每个类分别定义了一个执行命名操作的调用运算符。

plus类定义了一个函数调用运算符用于对一对运算对象执行+操作。

modulus类定义了一个调用运算符执行二元的%操作

equal_to类执行==

这些类都被定义成模板的形式，我么可以为其指定具体的应用类型，这里的类型即调用运算符的参数类型。

在算法中使用标准库函数对象

表示运算符的函数对象类常用来替换算法中的默认运算符。

```cpp
std::vector<std::string> svec;
sort(svec.begin(),  svec.end(), greater<std::string>());
```

标准库规定其函数对象对于指针同样适用。

```cpp
std::vector<std::string *> nameTable;
sort(nametable.begin(), nameTable.end(), less<std::string*>());
```

C++中有几种可调用的对象？

函数、函数指针、lambda表达式、bind创建的对象以及重载了函数调用运算符的类。

不同类型可能具有相同的调用形式

标准库function类型

function是一个模板，当创建一个具体的function类型时我们必须提供额外的信息。

我们不能(直接)将重载函数的名字存入functio类型的对象中，一种方法是将函数指针存入，第二种是使用lambda限定参数。

## 14.9 重载、类型转换与运算符

类型转换运算符

类型转换运算符是一种特殊成员函数，它负责将一个类类型的值转换成其他类型。

一般形式：operator type() const;

类型转换符可以面向任意类型（除了void）进行定义。

不允许转换成数组或者函数类型，但允许转换成指针（包括数组指针及函数指针）或者引用类型

类型转换既没有显式的返回类型（不能声明返回类型）也没有形参，而且必须定义成类的成员函数。

类型转换运算符通常不应该改变待转换对象的内容。

类型转换运算符可能产生意外的结果。

在实践中，类很少提供类型转换运算符。在大多数情况下，如果类型转换自动发生，用户可能会感觉比较意外，而不是感觉受到了帮助。

显式的类型转换运算符

explicit operator type() const;

当表达式出现在下列位置时，显式的类型转化将被隐式地执行：

if,while以do语句的条件部分。

for语句头的条件表达式

逻辑非运算符（!），逻辑或运算符(||)，逻辑与运算符(&&)的运算对象

条件运算符（？：）的条件表达式。

转换为bool

向bool的类型转化通常用在条件部分，因此operator bool 一般定义成explicit的。

避免有二义性的类型转换

通常情况下，不要为类定义相同的类型转换，也不要在类中定义两个及两个以上的转换源或转换目标的算术类型的转换。

类型转换与运算符

不要令两个类执行相同的类型转换

避免转换目标时内置算术类型的类型转换。特别是当你已经定义了一个转换成算术类型的类型转换时，接下来

------不要在定义接受算术类型的重载运算符。

-------不要定义转换到多种算术类型的类型转换。

除了显式地向bool类型的转换之外，我们应该尽量避免定义类型转换函数并尽可能地限制哪些“显然正确”的非显式构造函数。

重载函数与转换构造函数

​	如果调用重载函数时我们需要使用构造函数或者强制类型转换来改变实参的类型，则这通常意味着程序的设计存在不足。

重载函数与用户定义的类型转换

​	在调用重载函数时，如果需要额外的标准类型转换，则该转换的级别只有当多有可行函数都请求同一个用户定义的类型转换时才用，如果需要用户定义的类型转换不止一个，则该调用具有二义性。

函数匹配与重载运算符

​	当运算符出现在表达式中时，候选函数集的规模要比我们使用调用运算符调用函数时更大。

​	表达式中的运算符的候选函数集应该包括成员函数，也应该包括非成员函数。

​    如果我们对同一个类既提供了转换目标是算术类型的类型转换，也提供了重载的运算符，则将会遇到重载运算符与内置运算符的二义性问题。



​    