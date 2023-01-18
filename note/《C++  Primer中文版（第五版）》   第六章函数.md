---
typora-copy-images-to: ./
---

# 《C++  Primer中文版（第五版）》   第六章函数  

## 函数基础

### 1.什么是函数？

函数是一个命名了的代码块。

函数可以有0个或者多个参数。

### 2.函数的定义格式？

```C++
函数返回值类型 函数名字(形参列表)
{
    函数体
}
```

### 3.函数内定义的变量的作用域？

从定义的位置开始，直到函数结束。

函数外无法访问函数内定义的变量。

### 4.什么是分离式编译？

将函数的声明放在一个文件中，而其定义放在另外一个文件中。

## 参数传递

### 什么是传值参数？

当初始化一个非引用类型的的变量时，初始值被拷贝给变量。

对改变量的改动不会影响初始值。

```c++
int func(int a)
{
    a++;
    return a;
}

int main(int argc, char **argv)
{
    int a = 1, b = 0;
    std::cout << "before b = func(a), a = " << a << ",b = " << b << std::endl;
    b = func(a);
    std::cout << "after b = func(a), a = " << a << ",b = " << b << std::endl;
    return 0;
}

//执行结果
before b = func(a), a = 1,b = 0
after b = func(a), a = 1,b = 2
```



### 指针形参的特点有哪些？

当执行指针拷贝时，拷贝的是指针的值。

```c++
int func(int *a)
{
    std::cout << "address = " <<  a << std::endl;
    *a = 3000;
    return 0;
}

int main(int argc, char **argv)
{
    int a = 1, *b = &a;
    std::cout << "before func(b), a = "<< a << ",b = " << b << std::endl;
    func(b);
    std::cout << "after func(b), a = "<< a << ",b = " << b << std::endl;
    return 0;
}

//执行结果
before func(b), a = 1,b = 0x61fe14  
address = 0x61fe14
after func(b), a = 3000,b = 0x61fe14
```

### 如何传引用参数？

```C++
void func(int &a)
{
    a = 3000;
}

int main(int argc, char **argv)
{
    int a = 1;
    std::cout << "before func(a), a = " << a << std::endl;
    func(a);
    std::cout << "after func(a), a = " << a << std::endl;
    return 0;
}

//执行结果
before func(a), a = 1
after func(a), a = 3000
```

### 如何将在函数内的改变体现到函数外？

传引用，传地址和使用返回值

## 返回类型和return语句

return 语句的作用

终止当前正在执行的函数并将控制权返回到调用该函数的地方。

格式：

​	return;

​	return expression;

无返回值的return只能用于void类型的函数中。但是在void类型的函数中，return可以不写。

注意事项：

​	不要返回局部对象的引用或指针。

​    在含有return语句的循环后面也应该有一条return语句，如果没有该程序就是错误的。很多编译器无法发现此类错误。

```c++
#include <iostream>

int func(int i)
{
    for (; i > 10; i--)
    {
        if (i == 20)
        {
            std::cout << "----------return----------" << std::endl;
            return i;
        }
    }
}

int main(int argc, char const *argv[])
{
    /* code */
    std::cout << "Value = " << func(19) << std::endl;
    return 0;
}
```

编译器输出：

chapter_6_3.cpp: In function 'int func(int)':
chapter_6_3.cpp:13:1: warning: control reaches end of non-void function [-Wreturn-type]
 }

编译器版本：

g++ --version
g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

## 函数重载

定义：如果同一个作用域内的几个函数名字相同但是参数列表不同，我们称之为重载函数。

### 判断两个形参的类型是否相异

函数重载定义中的"参数列表不同"指的是参数的类型，不是名字。

形参的名字仅仅起到方便记忆的作用，有没有它并不影响参数列表的内容。

### 重载和const形参

顶层const不影响传入函数的对象。一个拥有顶层const的形参无法和另外一个没有顶层const的形参区分开来。

```c++
int func(int num);
int func(const int num);

//编译器报错：
//chapter_6_4.cpp:17:5: error: redefinition of 'int func(int)'
//int func(const int num)
//    ^~~~
//chapter_6_4.cpp:12:5: note: 'int func(int)' previously defined here
//int func(int num)
    
```

如果形参是某种类型的指针或引用，则通过区分其指向的是常量对象还是非常量对象可以实现函数重载，此时的const是底层的。

对于接受引用或者指针的函数来说，对象是常量还是非常量对应的形参不同。

## 特殊用途语言特性

默认实参，内联函数和constexpr函数

#### 默认是实参

定义：

在函数声明中添加

在给定的作用域中一个形参只能被赋予一次默认实参。

一旦某一个形参被赋予了默认值，那么其后的所有形参都必须有默认值。

调用：

函数调用时实参按其位置解析，默认实参负责填补函数调用缺少的尾部实参（靠右位置）

```c++
int func(int a, int b = 10, int c = 20);
func(11);
func(11,12);
func(11,12,13)
```

默认实参的初始值

局部变量不能作为默认实参。

只要是表达式的类型能够转换成形参所需要的类型，该表达式救恩那个作为默认实参

#### 内联函数

什么时候将函数定义成内联函数？

规模较小，流程直接，频繁调用

怎样定义一个内联函数？

加了关键字：inline

inline  int func();

类里面定义的函数都是内联。

加了inline的函数一定会在调用处展开？

不一定，inline相当于告诉编译器这个需要在调用处展开，是否展开是看编译器。

constexpr函数

什么是constexpr函数？

能用于常量表达式的函数

如何定义constexpr函数？

约定：函数的返回类型及所有形参的类型都得是字面值类型，并且函数体中必须有且只有一条return语句。



## 函数匹配

函数匹配的第一步就是选定本次调用对应的重载函数集，集合中的函数称为候选函数

​	特征1：与被调用的函数同名

​	特征2：其声明在调用点可见

第二步考察本次调用提供的实参，然后从候选函数中选出能被这组实参调用的函数，这些新选出的函数称为可行函数。

​	特征1：形参数量与本次调用提供的 实参数量相等。

​    特征2：每个实参的类型与对应的形参类型相同（或者可以转换成形参的类型）

第三步：从可行函数中选出与本次最匹配的函数。

​	从可行函数中筛选，如果有且只有一个函数满足下列条件，则匹配成功。

​	条件1：该函数的每个实参的匹配都不劣于其他可行函数需要的匹配。

   条件2：至少有一个实参的匹配优于其他可行函数提供的配置。

注意事项：

​	调用重载函数是应尽量避免强制类型转换，如果在实际应用中确实需要强制类型转换，则说明我们设计的形参集合不合理。

是谁完成函数匹配？

​	答：编译器

实参类型到形参类型的转换等级：

1.精确匹配

​	实参类型与形参类型相同。

​    实参从数组类型或函数类型转换成对应的指针类型。

​    向实参添加顶层const或者从实参中删除顶层const

2.通过const转换成实现的匹配。

3.通过类型提升实现的匹配。

4.通过算数类型转换或指针转化实现的匹配。

5.通过类类型转换实现的匹配。

## 函数指针

什么是函数指针？

顾名思义，指向函数的指针。

怎么定义函数指针？

简单理解就是把函数名成的部分换成一个“（指针）”

举例说明：

bool (*fp) (int a, int b);

为什么要用括号将指针部分包含？

可以不用括号，那就不是函数指针了。

举例说明：

bool *fp(int a, int b)；

怎样是用函数指针？

1.赋值时：

​	pf=func;

​	pf=&func;

2.调用时

​	bool a = pf(a, b);

```c++
#include  <iostream>

bool func(int a, int b)
{
    std::cout << "input a = "<< a << ", b = " << b << std::endl;
    return a > b;
}

int main(int argc, char const *argv[])
{
    /* code */
    int a = 100, b = 1002;
    func(a,b);

    //bool (*pf)(int a, int b);
    //bool (*pf)(int, int); //函数声明中可以省略形参名（又省点宽度）
    bool (*pf)(int, int) = func; //定义并赋值，看起来怪异，但就是这么怪
    //pf = func; //为什么不用pf=&func? 把函数名作为一个值使用时，自动转换成指针。
    pf(a, b);
    
    //另外的调用方式
    (*pf)(1000, 999);
    return 0;
}

//执行结果
//input a = 100, b = 1002
//input a = 100, b = 1002
//input a = 1000, b = 999
```

某一函数有多个重载函数，如何定义其函数指针？

指针类型与重载函数中的某一个精确匹配。（返回类型，形参列表完全匹配）

使用typedef和decltype定义函数指针别名？

typedef 跟着函数指针的定义

decltye(目标函数) 函数指针定义

举例说明：

​	typedef bool (*pf)(int, int);

​	pf 就变成了一个类型，pf就代表 bool（*pf）(int, int)

```C++
#include <iostream>

bool func(int a, int b)
{
    std::cout << "input a = " << a << ", b = " << b << std::endl;
    return a > b;
}

typedef bool (*PF)(int, int);
int main(int argc, char const *argv[])
{
    /* code */
    PF pf1 = func, pf2 = func;
    pf1(1,2);
    pf2(1,4);
    return 0;
}
```

函数指针可以作为形参/返回值吗?

可以

```c++
#include <iostream>

bool func(int a, int b)
{
    std::cout << "input a = " << a << ", b = " << b << std::endl;
    return a > b;
}

typedef bool (*PF)(int, int); 
typedef decltype(func) *DPF;

//bool pFuncParam(int a, bool pff(int a, int b)) //编译自动将funcA转换成函数指针
//bool pFuncParam(int a, bool (*pff)(int a, int b))   
//bool pFuncParam(int a, PF pff)   
bool pFuncParam(int a, DPF pff)   
{
    std::cout << "input a = " << a << std::endl;
    std::cout << "func param " << pff(666, 777) << std::endl;
    return false; 
}

DPF pFuncReturn()
{
    //return func; //返回类型不会自动地转换成指针，需要显示的指明。
    DPF dpf = func;
    return dpf;
}

int main(int argc, char const *argv[])
{
    /* code */
    PF pf1 = func, pf2 = func;
    decltype(func) *df = func; 
    pf1(1,2);
    pf2(1,4);
    df(1,3);
    pFuncParam(1000, df); //传一个函数指针
    pFuncParam(1000, func);//传一个函数名（实际也是函数指针，编译器将函数名转换成函数指针）
    DPF dpf = pFuncReturn();
    dpf(1001, 1002);
    return 0;
}

//执行结果：
//input a = 1, b = 2
//input a = 1, b = 4
//input a = 1, b = 3
//input a = 1000
//func param input a = 666, b = 777
//0
//input a = 1000
//func param input a = 666, b = 777
//0
//input a = 1001, b = 1002
```