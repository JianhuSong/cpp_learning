# 条款13：以对象管理资源

原因：

我们在函数中申请了资源，也在函数最后进行了释放。但是实际上，该函数可能在某个点就退出。就会导致资源没有及时释放掉

条款关键点：

1.获得资源后立即放进管理对象

2.管理对象运用析构函数确保资源被释放（利用了C++的析构函数自动调用机制）

何时将资源放进对象？

​	获得资源后立马放进管理对象。 RAII-（资源取得时机便是初始化时机-Resources Acquisition Is Initialization）

关键记忆点：

为防止资源泄漏，请使用RAII对象，它们在构造函数中获取资源并在析构函数中释放资源。

两个常用的RAII classes 分别是std::shared_ptr 和std::auto_ptr。前者通常是较佳选择，因为其copy行为比较直观。若选择auto_ptr，复制动作会导致它指向null

(发生了所有权转移，资源所有权从被复制对象到复制后生成的对象)

```cpp
# 简单构造一下场景
void test()
{
    try
    {
        MyClass *myclassa = new MyClass(123,124);
        if (myclassa == nullptr)
        {
            std::cout << "创建对象失败"<< std::endl;
            return;
        }
		
        # 从这里退出后，堆上申请的资源就没有释放
        if (myclassa->isMemABiggerThanMenB())
        {
            return;
        }

        std::cout <<"程序执行到最后了"<< std::endl;
        delete myclassa;
    }
    catch(const std::exception& e)
    {
        #抛异常后，也不会去释放new申请的资源
        std::cerr << e.what() << '\n';
    }
}

# 利用对象管理资源（例子中用shared_ptr）,当引用计数归零后，自动释放资源。不管test1在任何地方退出，都没有问题。
void test1()
{
    try
    {
        std::shared_ptr<MyClass> mcp = std::make_shared<MyClass>(123,1234);
        if (mcp == nullptr)
        {
            std::cout << "创建对象失败"<< std::endl;
            return;
        }
        if (mcp->isMemABiggerThanMenB())
        {
            return;
        }

        std::cout <<"程序执行到最后了"<< std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

# 看这本书有点折磨，有点枯燥。例子不好想。。。。。。。。。。
```

# 条款14：在资源管理类中小心coping行为

对于堆上的资源可以使用atuo_ptr/shared_ptr来管理

对于非堆上的资源，可能需要建立自己的资源管理类。

当一个RAII对象被复制，会发生什么事情？

1.禁止复制

​	许多时候RAII对象被复制并不合理

2.对底层资源祭出“引用计数法”

​	通常只要含有一个tr1::shared_ptr成员变量,RAII  classes便可实现reference-counting copying行为.

coping函数有可能被编译器自动创建出来,因此除非编译器产生的版本做了你想做的事情,否则你得自己编写它们

复制RAII对象必须一并复制它所管理的资源，所以资源额copying行为决定RAII对象的copying行为

普遍而常见的RAII class copying行为是：抑制copying 施加引用计数法

# 条款15: 在资源管理类中提供对原始资源的访问.

原因:

​	许多的APIs直接指涉资源,我们又通过资源管理类来管理了这些资源.那么就需要资源管理类提供访问原始资源的方法.

​	设计良好的classes会隐藏可不不需要看的部分,但备妥客户需要的所有东西.

手段:

​	提供隐式转换,将RAII class转换为其底部资源

​	提供显式转换,将RAII class转为其底部资源

推荐: 使用显式转换的方法将RAII class转换为其底部资源.

关键知识点:

1.APIs往往要求访问原始资源,所以每一个RAII  class应该提供一个"取得其所管理资源"的办法

2.对原始资源的访问可能经由显式转换或隐式转换,一般而言显式转换比较安全.但是隐式转换对客户而言比较方便.

# 条款16: 成对使用new和delete时要采取相同的形式

原因:

​	对一个指针使用delete,唯一能够让delete知道内存中是否存在一个"数组大小记录"的方法就是:由你来告诉它. 

举个例子:

```cpp
    std::string* strp = new std::string;
    std::string* strap = new std::string[100];
    
    delete strp;       //删除一个对象
    delete [] strap;   //删除一个由对象组成的数组
```

成对关系:

​	当你在new时使用了[ ],那么在释放资源的时候,就必须使用delete [] 

​	当你在new时没有使用[],那么在释放资源的时候,就使用delete

# 条款17: 以独立语句将newed对象置入智能指针

这个条款是不是可以理解对智能指针的赋值方式????

还是写一段代码吧,不写写没感觉.

```cpp
class MyClass
{
public:
    MyClass(int a, int b): mema(a), memb(b) {}
    ~MyClass() = default;
    void memberPrint()
    {
        std::cout << "mema = " << mema << ", memb = " << memb << std::endl;
    }

 private:
    int mema;
    int memb;
};

bool func2(int a, int b)
{
    return a > b;
}

void func1(std::shared_ptr<MyClass> usp, bool bool_value)
{
    if (bool_value)
        usp->memberPrint();
    std::cout << "Hello world" << std::endl;
}

int main(int argc, char const *argv[])
{
    // 无法从原始指针一步转到智能指针
    // 第一种：报错：could not convert '(operator new(8), (<statement>, ((MyClass*)<anonymous>)))' from 'MyClass*' to 'std::shared_ptr<MyClass>'
    // func1(new MyClass(11,23));

    // 第二种：用就用一套的东西,终极推荐用法
    func1(std::make_shared<MyClass>(11,12),func2(11,12));

    // 第三种：这个就是书上的例子
    func1(std::shared_ptr<MyClass>(new MyClass(11,23)), func2(11,23));
    
    // 第四种：这个是书上的推荐用法。
    std::shared_ptr<MyClass> pm(new MyClass(12,13));
    func1(pm, func2(12,13));

    // 其实这里面就是一个‘异常安全’的概念： 保证在出现异常的时候资源正常释放。
    // 隐藏了一个小知识点：C++不保证参数求值顺序，以及内部表达式的求值顺序
    // 由于执行顺序不一定，那么在第3中写法中，若先执行new,然后执行func2,func2发生异常。直接跑到异常处理的地方去了，那么对于之前new申请的资源是不是就是泄露了。
    return 0;
}

//至于使用new和使用std::make_shared 给shared_ptr赋值的区别,网上博文很多.
//这也是看这本书有趣的地方,每个条款里面可能有许多知识点不懂,或者不明确的. 
```

