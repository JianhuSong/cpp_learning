# 条款01： 视C++为一个语言联邦

四个主要的次级语言：

1.C，区块，语句，预处理器，内置数据类型，数组，指针等统统来自C。

２.object-Oriented C++ :classes(),封装，继承，多态，virtual函数（）……等等

3.Template C++,这是C++的泛型编程部分。 它带来了TMP（模板元编程）

4.STL，STL是个template程序库。

使用哪一块就遵循哪一块的规则

# 条款02：尽量以const enum,inline替换#define

原因应该是两个：

1.若使用宏的时候出错，定位信息上看不到宏名称

2.预处理阶段的宏替换，可能导致目标码出现多份。

举例： #define ASPECT_RATIO 1.653

编译器也许从未看到ASPECT_RATIO ，只能看到1.653，一旦出错的话，错误信息中可能只提1.653，不提ASPECT_RATIO 

```cpp
#include <iostream>
#define ASPECT_RATIO  1.653

int main(int argc, char const *argv[])
{
    int aaa = 100000;
    std::cout << ASPECT_RATIO << std::endl;
    std::cout << aaa << std::endl;
    return 0;
}
```

使用g++ -E 生成预处理后的文件

```cpp
# 其它部分已经省略
int main(int argc, char const *argv[])
{
    int aaa = 100000;
    std::cout << 1.653 << std::endl;  # 经过预处理后这原先的ASPECT_RATIO已经被替换成了1.653
    std::cout << aaa << std::endl;
    return 0;
}
```

使用命令： g++  -c test.cpp 生成test.o, 然后再使用命令 objdump -t test.o (test.o 生成的符号表内容少些，a.exe 生成的符号表内容太多了)

```cpp
# 符号表的部分内容
[ 30](sec  2)(fl 0x00)(ty   0)(scl   2) (nx 0) 0x0000000000000000 aaa # 看到aaa了，并未看到ASPECT_RATIO，预处理时已经无了，后面看不到正常
[ 31](sec 10)(fl 0x00)(ty   0)(scl   2) (nx 0) 0x0000000000000000 .refptr._ZSt4cout
[ 32](sec  9)(fl 0x00)(ty   0)(scl   2) (nx 0) 0x0000000000000000 .refptr._ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
[ 33](sec  0)(fl 0x00)(ty  20)(scl   2) (nx 0) 0x0000000000000000 __main
[ 34](sec  0)(fl 0x00)(ty  20)(scl   2) (nx 0) 0x0000000000000000 _ZNSolsEd
[ 35](sec  0)(fl 0x00)(ty  20)(scl   2) (nx 0) 0x0000000000000000 _ZNSolsEPFRSoS_E
[ 36](sec  0)(fl 0x00)(ty  20)(scl   2) (nx 0) 0x0000000000000000 _ZNSolsEi
[ 37](sec  0)(fl 0x00)(ty  20)(scl   2) (nx 0) 0x0000000000000000 _ZNSt8ios_base4InitD1Ev
[ 38](sec  0)(fl 0x00)(ty  20)(scl   2) (nx 0) 0x0000000000000000 _ZNSt8ios_base4InitC1Ev
[ 39](sec  0)(fl 0x00)(ty  20)(scl   2) (nx 0) 0x0000000000000000 atexit
[ 40](sec  0)(fl 0x00)(ty   0)(scl   2) (nx 0) 0x0000000000000000 _ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
[ 41](sec  0)(fl 0x00)(ty   0)(scl   2) (nx 0) 0x0000000000000000 _ZSt4cout
```

这就是为什么说使用的宏名称可能从未进入记号表（symbol table）

# 条款03：尽可能使用const

const的功能（指定一个‘不可被改动’的兑现）由编译器实现。

const多才多艺。可在class的外部，内部。可以修饰指针

const语义解读：出现在星号左边，表示被指物是常量，如果在星号右边，表示指针自身是常量。若星号两边都有表示被指物以及指针两者都是常量。

当const出现在星号左边的时候有两种写法，但是都是一个意思： const 类型 * p ；类型 const * p 

const成员函数承诺绝不改变其对象的逻辑状态，non-const成员函数没有这般承诺

关键知识点：

将某些东西声明为const可帮助编译器侦测出错误用法。const可被施加于任何作用域内的对象、函数参数、函数返回类型、成员函数本体

编译器强制实施bitwise constness,又称为physical constness，但你编写程序时应该使用“概念上的常量性”（conceptual constness）

当const和non-const成员函数有着实质性等价的关系时，令non-const版本调用const版本可避免代码重复。

# 条款04：确定对象被使用前先被初始化

若不手动为成员变量赋初值，那么成员变量有时候被赋值为0，有时不会。

确保每一个构造函数都将对象的每一个成员初始化

对于类的成员变量而言，在构造函数中使用赋值语句赋值与使用初始化列赋值的区别：

​	赋值版本：首先调用成员变量对应的默认构造函数为其设初值，然后立马再对其赋新值

​	初始化列：初值列中的参数被传递给了成员变量的构造函数

注意事项：对于内置类型而言，上述两种初始化方式，成本相同。

必须使用初值列的情况：

1.需要初始化的数据成员是对象的情况

2.成员变量是reference/const

3.子类初始化父类的私有成员

使用初值列时，请总在初值列中列出所有成员变量

注意事项：有多个构造函数时，可将‘’赋值和初值列成本相同‘的对象写在一个单独的函数中，供构造函数调用。

成员初值列中的次序，最好总是其声明顺序：

C++有着十分固定的初始化顺序： base classes 早于derived classes,class的成员变量总是按其声明次序初始化。

static对象：

​	其寿命从被构造出来直到程序结束为止

local static对象：

​	函数内的static对象

non-local static对象：

​	不在函数内的static对象

编译单元：

​	产出单一目标文件的那些源码。基本上它是单一源码文件加上其所含入的头文件

跨编译单元的初始化次序：

问题描述：如果某个编译单元内的non-local static 对象的初始化动作使用了另一个编译单元的non-local static 对象，它所用到的这个对象可能尚未被初始化

原因：C++对于”定义与不同编译单元内的non-static对象“的初始化次序无明确定义

解决办法：将每个non-local static 对象搬到自己的专属函数内，返回一个reference指向它所含对象。这样的函数称为”仿真函数“

原理：C++保证，函数内的local static对象会在’‘该函数被调用期间“ ”首次遇上该对象的定义式“时被初始化。

好处： 如果从未调用"仿真"函数，就绝不会有构造和析构成本

避免在对象初始化之前就使用它们：

​	1.手工初始化内置型non-member对象

​	2.使用成员初值列对付对象的所有成分

​	3.在”初始化次序不确定性“氛围下加强你的设计

# 条款05：了解C++默默编写并调用哪些函数

对于一个类，如果你没有声明，那么编译器就会为它声明一个copy构造函数，一个copy assignment操作符和一个析构函数。如果你没有声明任何构造函数，编译器也会为你声明一个default构造函数。 这些函数是public 和inline

编译器版本的析构函数：

​	编译器生成的析构函数是non-virtual,但是如果其基类声明有virtual的析构函数，那么这个函数是virtual

编译器版本的copy构造函数和copy assignment操作符：

​	编译器创建的版本只是单纯的将来源对象的每一个non-static成员变量拷贝到目标对象

以下情况编译器不会自动生成copy assignment操作符：

1.class内含reference成员/const 成员

2.基类将copy assignment 声明为private，那么编译器不会为其派生类生成copy asignment操作符

关键点：

编译器可以暗自为class创建default构造函数、copy构造函数、copy asignment操作符以及析构函数

