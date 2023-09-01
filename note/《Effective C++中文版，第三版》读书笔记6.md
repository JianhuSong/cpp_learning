# 条款32：确定你的public继承塑模出is-a关系

简单知识点回顾（若不知道那就是扫盲了）：

is-a关系：指的是类的父子继承关系。比如说A继承了B。我们可以说A是B的一种特殊情况

has-a关系：指的是一种组合关系，是关联关系中的一种（一个类中有另一个类的实例），是整体和部分的关系。

更通俗点讲：其实就是这两种关系的字面意思。不用特别记忆。

真理：代码编译通过并不代表就可以正确运行

“public继承”意味着is-a

​		适用于base classes身上的每一件事情也一定适用于derived classed身上。

是不是可以这么理解：

​	我们在编写base classes时，应该让其更普遍一点。

# 条款33：避免遮掩继承而来的名字

derived classes 内的名称会遮掩base classes内的名称

为了让被遮掩的名称再见天日，可以适用using声明式或转交函数

using声明式会令继承而来的某个给定名称的所有同名函数在derived class都可见

何时使用这两种方式？

**using声明式子**

​	如果你继承base class并加上重载函数，而你又希望重新定义或覆写（推翻）其中的一部分，那么就需要为那些原本被遮掩的每个名称引入一个using声明式子。

**转交函数：**

​	有时候并不想继承（不能是public继承）base classes的所有函数，只想继承某一个版本的时候。就可以些一个转交函数，在函数内以"base class名称::函数名"的方式。

​	并不是所有编译器都支持using式子，这个时候可以考虑使用转交函数

# 条款34：区分接口继承和实现继承

public继承由两部分组成：函数接口继承和函数实现继承

对于基类中的接口，子类往往可能有三个方面的考虑;

1.子类只希望继承基类成员函数的接口 （基类中的纯虚函数）

2.子类希望同时继承基类中的接口和实现，还希望能够覆写。（基类中的虚函数（非纯虚））

3.子类希望同时继承函数的接口和实现，并且不允许覆写任何东西。（基类中的non-virtual函数）

类的设计者常犯的两个错误：

1.基类中的所有函数都声明为non-virtual

2.基类中的所有成员函数都声明为virtual(构造函数除外)

有些时候使用非纯的虚函数定义的接口，子类可能忘记实现，子类特异性的行为可能用基类的函数不适用：

这个时候可以考虑带定义纯虚函数，这样这个纯虚函数必须在子类中实现，子类无法忘记。

写一段体会一下（完善书中的例子）

```cpp
class Shape
{
public:
    // 纯虚函数不能将其定义写在类内：pure-specifier on function-definition
    // 子类若想被实例化，那么继承了Shape，它就必须实现draw1,draw
    virtual void draw() const = 0;

    // 这个纯虚函数，在基类这里不提供定义
    virtual void draw1() const = 0;

    // 非纯虚函数的目的是为了让子类继承该函数的接口和缺省实现
    virtual void error(const std::string &msg)
    {
        std::cout << "Shape error msg = " << msg << std::endl;
    }

    int objectID() const
    {
        std::cout << "Hello world" << std::endl;
        return 0;
    }

    // 基类必须有一个虚的析构函数
    virtual ~Shape() = default;
};

// 可以在类外实现纯虚函数的
void Shape::draw() const
{
    std::cout << "base pure virtual draw" << std::endl;
}

class Rectangle : public Shape
{
public:
    Rectangle() {}
    ~Rectangle() = default;

    // 继承自抽象class的子类，应该定义抽象基类中的纯虚函数，不定义的话，那么这个子类也是抽象class,无法实例化。
    virtual void draw() const override
    {
        std::cout << "Rectangls draw" << std::endl;
    }

    virtual void draw1() const override
    {
        std::cout << "Rectangls draw1" << std::endl;
    }
};

class Ellipse : public Shape
{
public:
    Ellipse() {}
    ~Ellipse() = default;

    virtual void draw() const override
    {
        std::cout << "Ellipse draw" << std::endl;
    }

    virtual void draw1() const override
    {
        std::cout << "Ellipse draw1" << std::endl;
    }

    virtual void error(const std::string &msg) override
    {
        std::cout << "Ellipse error msg = "<< msg << std::endl;
    }
};

int main(int argc, char const *argv[])
{
    Rectangle test;
    Ellipse ellipse;
    test.draw();

    // 有定义的纯虚函数，可以通过这样的方式调用
    test.Shape::draw();

    // 非纯虚函数
    // 个人理解的缺省实现，意思就是你要实现就实现，不实现就算了。
    //  Rectangle 中没有实现自己的error,所以实际调用的是Shape的error
    test.error(std::string("XXXXXXXXXX"));

    // Ellipse中实现了自己的error, 这样调用实际上调的是它自己的error
    ellipse.error(std::string("XXXXXXXXXX"));

    // 可以这样调用基类中的函数
    ellipse.Shape::error(std::string("XXXXXXXXXX"));

    // non-virtual 意味着基类不希望这个操作因为子类的不同而不同。
    test.objectID();
    ellipse.objectID();
    return 0;
}
```



# 条款35：考虑virtual函数以外的其它选择

这个条款涉及到了设计模式，这个相关内容是我的弱项，但是已经纳入了强化学习内容。后续会出设计模式的学习

# 条款36：绝不重新定义继承而来的non-virtual函数

原因：

1.non-virtual 不是动态绑定的，无法通过基类指针访问子类的non-virtual函数，这样只能访问到基类的vritual

2.public继承是is-a的，但是重定义non-virtual后，通过子类访问non_virtual的表现出来的行为已经和基类的non_virtual不一样了。

写一段：

```cpp
#include <iostream>

class Base
{
public:
    void non_virtual()
    {
        std::cout << "It is Base non_virtual" << std::endl;
    }

    virtual void virtual1() const = 0;
    virtual ~Base() = default;
};

// 要想为纯虚函数添加定义，只能在类外，类内会报错。
void Base::virtual1() const
{
    std::cout << "It is Base virtual1" << std::endl;
}

class sun1 : public Base
{
public:
    sun1() {}
    ~sun1() = default;
    void non_virtual()
    {
        std::cout << "It is sun1 non_virtual" << std::endl;
    }

    virtual void virtual1() const
    {
        std::cout << "It is sun1 virtual1" << std::endl;
    }
};

int main(int argc, char const *argv[])
{
    Base *bp1 = new sun1;
    bp1->non_virtual();
    bp1->virtual1();

    // 按照public is-a 的这种思维，我们通过sun11访问non_virtual()的行为应该与基类中的一致
    // 但是实际上访问的是sun1自己的non_virtual(),那就违反了is-a这种关系了。
    sun1 sun11;
    sun11.non_virtual();
    
    // sun11.Base::non_virtual();
    return 0;
}

// 输出结果
// It is Base non_virtual
// It is sun1 virtual1
// It is sun1 non_virtual
```



# 条款37：绝不重新定义继承而来的缺省参数值

静态类型：
	在程序中被声明时所采用的类型
动态类型：
	在运行的时候才能确定其类型
静态绑定：（前期绑定 early bingding）
	在声明时就已经确定了其运行期间的样子。
动态绑定：(后期绑定，late binding)
	在运行的时候才决定其真实的类型。
知识点：
	virtual函数是动态绑定的，但是其缺省参数是静态绑定的。
为什么C++坚持virtual函数是动态绑定，但是缺省参数值是静态绑定呢？
运行期效率。为了程序的执行速度和编译器实现上的简易度。
请记住：
	绝对不要重新定义一个继承而来的缺省参数值。

写一段：

```cpp
#include <iostream>
#include <string>

// 完善例子部分
class Shape
{
public:
    enum ShapeColor
    {
        Red,
        Green,
        Blue
    };
    void draw1(ShapeColor color = Red) const
    {
        doDraw(color);
    }

    //为什么要定义成纯虚函数？ 目的是为了让子类必须实现，不实现无法实例化。防止忘了。
    virtual void draw(ShapeColor color = Red) const = 0;
    virtual ~Shape() = default;

private:
    virtual void doDraw(ShapeColor color) const = 0;
};

class Rectangle : public Shape
{
public:
    Rectangle() {}
    ~Rectangle() = default;
    virtual void draw(ShapeColor color = Green) const
    {
        std::cout << "Rectangle draw color = " << color << std::endl;
    }

private:
    virtual void doDraw(ShapeColor color) const
    {
        std::cout << "Rectangle doDraw color = " << color << std::endl;
    }
};

class Circle : public Shape
{
public:
    Circle() {}
    ~Circle() = default;
    virtual void draw(ShapeColor color) const
    {
        std::cout << "Circle draw color = " << color << std::endl;
    }

private:
    virtual void doDraw(ShapeColor color) const
    {
        std::cout << "Circle doDraw color = " << color << std::endl;
    }
};

void drawFunc(Shape *sp)
{
    sp->draw();
}

void drawFunc2(Shape *sp)
{
    sp->draw1();
}

int main(int argc, char const *argv[])
{
    Rectangle rectangle, *rp = &rectangle;
    Circle circle, *rc = &circle;

    // 缺省参数是静态绑定的，所以这样定义绑定的静态参数就是自己的
    rectangle.draw();
    circle.draw(circle.Green);

    // 导致有问题的用法
    drawFunc(rp);
    drawFunc(rc);

    // 替代方法: 将带有默认参数的定义为non-virtual, 然后调用一个私有的纯虚函数不带默认参数，子类覆写这个私有的virtual就行。
    drawFunc2(rp);
    drawFunc2(rc);

    // 是不是可以将所有的draw()的默认参数都改成red是不是就行了？
    // 但是如果某一天修改了基类的draw()的默认参数，那么所有子类都要跟着改。不修改的话一样的存在“重复定义一个继承而来的缺省参数值”问题
    return 0;
}
```



# 条款38：通过复合塑模出has-a或‘根据某物实现出’

复合是类型之间的一种关系，当某种类型的对象内含有它种类型的对象。两种类型之间就是has-a关系

请记住：

复合的意义和public继承完全不同

在应用域，复合意味has-a.在实现域，复合意味着根据某物实现出

# 条款39：明智而审慎地适用private继承

两条规则：

​	如果class之间的继承关系是private的那么编译器不会自动的将子类对象转换成父类对象。

​	通过private继承的而来的所有成员，不管其在父类中是什么属性，在子类中都会变成private属性。

private继承意味着子类对象根据基类对象实现而得。（这点和复合类型（has-a）很想）

如果可在复合类型（has-a）和private二者之间选择，那么优先选择复合

什么情况下使用private继承？

当子类想要访问基类的protect成分，或者为了重新定义一个或多个virtual函数

基类不含任何数据。没有non-static成员变量，没有virtual函数，没有virtual base class

小知识点：

C++裁定凡是独立（非附属）对象都必须有非零大小的空间。

标题解释：在考虑所有其它方案之后，如果仍然认为private继承是最佳办法，这是才使用private

请记住：

​	private继承意味着根据某物实现出。它通常比复合的级别低。如果子类要访问基类的protect成员。或者要重新定义继承而来的virtual函数时，这么设计时合理的

private继承可以造成empty base最优化。这个对于致力于“对象尺寸最小化”的程序库开发人员而言，可能很重要。

# 条款40：明智而审慎地适用多继承

请记住：

多重继承比单一继承复杂。他可能导致歧义性，以及对virtual继承的需要

virtual继承会增加大小、速度、初始化（及赋值）复杂度等等成本。如果virtual base classes 不带任何数据，将是最具实用价值的情况。

多重继承的确有正当用途。 其中一个情节涉及“public继承某个interface class”和 “private继承某个协助实现class”的两相组合