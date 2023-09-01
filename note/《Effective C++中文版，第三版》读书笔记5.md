# 条款26：尽可能延后变量定义式出现时间

原因：

​	只要你定义了一个变量而其类型带有一个构造和一个析构，程序控制流到达其定义时有构造成本，控制流离开该变量的作用域时有析构成本

尽可能延后

​	**不只因该延后变量的定义，直到非得使用该变量的前一刻为止，甚至应该尝试延后这份定义直到能够给它初值实参为止。**

只在循环内使用的变量

A：定义于循环外

B：定义于循环内

两种情况使用A
	1.赋值成本比‘’构造+析构‘成本低

​	2.正在处理代码中效率高度敏感的部分

其它情况请使用B方法

# 条款27：尽量少做转型动作

旧式转型

​	(T) expression     T(expression)

新式转型

​	const_cast_<T>_expression()/dynamic_cast_<T>_(expression)/reinterpret_cast_<T>_(expression)/static_cast_<T>_(expression)

四种新式转型简介：

​	const_cast通常被用来移除对象的常量属性，也是唯一由此能力的C++转型操作符

​	dynamic_cast主要是用来执行“安全向下转型”，也就是用来决定某对象是否归属继承体系中的某个类型。唯一一个无法用旧式转型执行的动作，也是唯一可能耗费重大运行成本的转型动作

​	reinterpret_cast意图执行低级转型，实际动作（即结果）可能取决于编译器，这也表示它不可移植

​	static_cast用来强迫隐式转换，将non-const->const, int->double,void*->type*,pointer-to-base->pointer-to-derived,但是无法从const->non-const

旧的风格任然合法，但是新的更受欢迎：

​	1.新的风格容易在代码中被辨识出来

​	2.各转型动作的目标愈窄化，编译器愈可能诊断出错误的运用。

何时使用旧式转型？

​	**唯一的**时机是，当我们调用**一个explicit构造函数**将一个对象**传递给一个函数**时。

任何一个类型转化（显式的还是隐式的）往往真的令编译器编译出运行期间执行的码

单一对象可能拥有一个以上的地址么？

​	以base_*_执行它的时的地址和以dervied_*_指向它时的地址

​	C++中可能，多重继承几乎一直发生，单一继承可能发生

dynamic_cast的许多实现版本执行速度相当慢

什么情况下可能会使用dynamic_cast?

​	通常是因为你想在一个你 认定为derived class对象身上执行derived class操作函数，但你手上只有一个”指向base“的pointer或reference

​	在这种情况下如何避免使用dynamic_cast?

​	1.使用容器并在其中存储直接指向derived  class对象的指针

​	2.在base class内提供virtual函数做你想对各个派生类做的事（其实就是利用动态绑定机制--个人看法）

请记住：

​	如果可以，尽量避免转型，特别是在注重效率的代码中避免dynamic_casts.

​	如果转型是必须的，那么请将它隐藏于某个函数背后。客户可以调用该函数，而不需要将转型代码放进他们自己的代码

​	宁可使用C++-stype(新式)转型，不要使用旧式转型。新式转型很容易辨识出来，而且也有着分门别类的功能

# 条款28：避免返回handles指向对象内部成分

原因：

​	指向内部成分的handle的寿命可能比对象长，意味着一旦对象被析构，那么那个handle就变成空悬、虚吊。

两个小知识点：

​	1.成员变量的封装性最多只能等于”返回其reference“的函数的访问级别

​		如果一个public成员函数返回了一个私有成员变量的reference,那么这个私有成员变量看起来是私有的（它被声明为私有），但是实际在类外也能访问（它的reference被返回）

​	2.如果const成员函数传出一个reference,后者所指数据与对象自身有关联，而它又被存储于对象之外，那么这个函数调用这可以修改那笔数据。

reference、指针和迭代器统统都是所谓的handles(号码牌，用来取得某个对象)，返回一个代表对象内部数据的handle，随之而来的便是降低对象封装性的风险

小知识点：

​	不被公开使用的成员函数（也就是被声明为protected或private）也是对象的内部的一部分

不是绝对，operator[]返回了reference指向容器内的数据，这些数据会随着容器被销毁而销毁

请记住：

​	避免返回handles(reference、指针、迭代器)指向对象内部。

写一段体验一下：

```cpp
class MyBase
{
public:
    virtual ~MyBase() = default;
    virtual void baseCout()
    {
        std::cout << "I am base" << std::endl;
    }
};

class MyDerived : public MyBase
{
public:
    MyDerived() {}
    MyDerived(int ina, int inb) : a(ina), b(inb) {}
    virtual void baseCout()
    {
        std::cout << "I am Dervied" << std::endl;
    }

    void printAllMember()
    {
        std::cout << "a = " << a << ", b = " << b << std::endl;
    }

    // int &getA() const {return a;} //这个版本无法通过编译  binding reference of type 'int&' to 'const int' discards qualifiers
    const int &getA() const { return a; }

private:
    int a;
    int b;
};

int main(int argc, char const *argv[])
{
    MyDerived *myderived = new MyDerived(11, 23);
    myderived->printAllMember();
    int &aaa = const_cast<int &>(myderived->getA());   // 换个花样，去掉其const属性
    std::cout << "&aaa 0 =======" << aaa << std::endl; // 这个时候值就是成员a的值
    aaa = 12312312;                                    // 修改
    myderived->printAllMember();                       // 从打印结果来看，对象内部的值已经被修改
    delete myderived;
    std::cout << "&aaa 1 =======" << aaa << std::endl; // 这个时候已经变成了随机值
    return 0;
}
// 输出结果
// a = 11, b = 23
// &aaa 0 =======11
// a = 12312312, b = 23
// &aaa 1 =======1507664
```

# 条款29：为“异常安全”而努力是值得的

异常安全函数提供三种保证之一：

基本承诺：

​	如果异常抛出，程序内的任何事物仍然保持在有效状态下。没有任何对象或数据结构会因此而败坏，所有对象都处于一种内部前后一致的状态。

强烈保证：

​	如果异常抛出，程序状态不改变。调用这样的函数需要有这样的认知，如果函数成功，就是完全成功，如果函数失败，程序会恢复到”调用之前“的状态。

不抛掷保证：

​	承诺绝不抛出异常，因为它们总是能够完成它们原先承诺的功能。内置类型身上的所有操作都提供nothrow保证

实际：

​	可能的话请提供nothrow保证，但对大部分函数而言，抉择往往落在基本保证和强烈保证之间。

请记住：

​	异常安全函数即使发生异常也不会泄漏资源或允许任何数据结构败坏。这样的函数区分为三种可能的保证：基本型，强烈性，不抛异常型

​	强烈保证，可以通过copy-and-swap实现出来，但是并非对所有函数都可实现或具备现实意义

​	函数提供的异常安全保证，通常最高等于其所调用的各个函数的异常安全保证最弱者。

# 条款30：透彻了解inling的里里外外

inline函数的优点：看起像函数，动作像函数，比宏好，可以调用但是没有函数调用所招来的额外开销。

关键知识点：

​	inline只是对编译器的一个申请，不是强制命令。这个申请可以显式或隐式

​	显式：	使用inline关键字

​	隐式：将函数定义于class定义式中

习惯：

​	inline函数通常别置于头文件中。

请记住：

​	将大多数inlining限制带小型、被频繁调用的函数身上。

​	不要因为function templates出现在头文件中，就将其声明为inline

# 条款31：将文件间的编译关系降至最低

简单的设计策略：

​	如果使用object references 或object points可以完成任务，就不要使用objects. 可以只靠一个类型声明就定义出指向该类型的references和pointers.但如果定义某个类型的objects,就需要用到该类型的定义式

​	如果能够，尽量以class声明替换class的定义式子。注意，当你声明一个函数而它用到某个class时，你并不需要class的定义；即便是使用by value方式传递参数（或返回值）也一样。

为声明式和定义式提供不同的头文件

声明式的头文件命名”功能fwd.h“,举例：_<_iosfwd>内含iostream个组件的声明式

请记住：

​	支持编译依存性最小化的一般构想是：相依于声明式，不要相依于定义式。基于此构想的两个手段是Handle classes 和interface classes

​	程序库头文件应该以”完全且仅有声明式“的形式存在。这种做法不论是否设计templates都适用。

