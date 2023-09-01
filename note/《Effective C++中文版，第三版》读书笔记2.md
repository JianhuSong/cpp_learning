# 条款06：若不想使用编译器自动生成的函数，就该明确拒绝

为驳回编译器自动（）提供的机能，可将相应的成员函数声明为私有的，同时不实现它。

```cpp
#include <iostream>

class MyClass
{
public:
    MyClass(int ina, int inb) : b(inb), a(b) {}
    ~MyClass() = default;

    // 可以在其声明后面加上delete修饰，调用时会编译报错
    // MyClass(const MyClass &) = delete;
    // MyClass &operator=(const MyClass &) = delete;

private:

    // 可以将其声明为私有的，但是不实现。 有权限调用的报链接错误，无权调用的编译报错说是private的
    // MyClass(const MyClass&);
    // MyClass& operator=(const MyClass&);
    int a; // 这是声明
    int b;

    // int a = 10;
    // int b = 11;
};

int main(int argc, char const *argv[])
{
    MyClass myclassb(11, 12);
    // MyClass myclassa = MyClass();  // 我们声明了自己的构造函数，编译器不再生成默认版本
    MyClass myclassc(13, 14);
    // myclassc = myclassb;           // = 被我们声明成了私有，类外调用报‘is private’,其它有权限调用的，会报连接错误（我们没实现）

    // 以下两种相当于都是调用了拷贝构造函数
    MyClass myclassa = MyClass(11,13);// 我以为这个调用的是=，但从编译器报错来看，实际调用的是拷贝构造函数
    MyClass myclassd(myclassb);
    return 0;
}
```

# 条款07： 为多态基类声明virtual析构函数

原因：

​	C++明确指出，当派生类对象经由一个基类指针被删除，而该基类带有一个non-virtual 析构函数，其结果未定义。

​	实际上：派生类中的派生类自己的部分没有被删除。仅删除了基类部分。

​	换一种说法，non-vitual的析构函数，无法触发动态绑定。在析构时，无法调用派生类的析构，只能调用自己的析构。

实际使用中，拒绝继承non-virtual析构函数的class

所有的stl容器都不带任何virtual析构函数

析构函数的运作方式是，最深层派生的哪个class其析构函数最先被调用，然乎其每一个base class的析构函数被调用。

关键知识：

带多态性质的基类应该声明一个virtual析构函数。如果class带有任何virtual函数，那么它就应该拥有一个virtual析构函数

如果一个类设计出来的目的不是作为基类，或者不是为了具有多态性，就别声明virtual析构函数。

# 条款08：别让异常逃离析构函数

C++不禁止析构函数吐出异常，但是它不喜欢析构函数吐出异常。

析构函数绝对不要吐出异常，如果非要吐异常，那么析构函数应该捕捉异常，然后吞下它们（不往外抛）或结束程序

# 条款09： 绝不在构造和析构过程中调用virtual函数

一个意思的两种说法：

​	派生类的基类构造期间virtual函数绝不会下降到派生类的阶层

​	在派生类的基类构造期间，virtual函数不是virtual函数（个人感觉：基类构造期间，其virtual函数失去动态绑定属性）

# 条款10：令operator=返回一个reference to *this

原因：

​	为了实现连锁赋值

所有赋值相关的运算都应该遵守这个协议

所有内置类型和标准程序库提供的类型都遵守这一协议

# 条款11: 在operator=中处理’‘自我赋值”

解决方法：

​	1.在确定’‘资源管理对象’在copy发生时由正确的举措，那么可以使用对象来管理资源。

​	2.写一段“证同测试”达到自我赋值检测的目的

​	3.关注"异常安全性"，从而精心安排语句。

​	4.使用copy and swap技术，为传入的数据做一个备份，然后把*this的数据和复件做交换

# 条款12:复制对象时勿忘其每一个成分

原因：

​	当你实现自己的copying函数，实现代码中几乎必然出错时，编译器不会告诉你。

copying 函数

​	copy构造函数和copy assignment操作符

具体解释：

​	当编写一个copying函数，确保复制所有local成员变量，调用所有基类的适当的copying函数

注意事项：

​	copy assignment操作符不要调用copy构造函数

​	copy构造函数不要调用copy assignment操作符

实际操作：

​	若copy构造函数和copy assignment操作符有相近的代码，那么就写个私有的函数（通常叫init）让他俩调用。
