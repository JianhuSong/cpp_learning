# 《C++  Primer中文版（第五版）》   第八章 IO类

C++语言不直接处理输入输出，而是通过一族定义在标准库中的类型来处理IO。

IO库定义了读写内置类型的操作。

## 8.1 IO类

标准库中的IO类型

![image-20221208165225632](E:\download\book\自学\看书笔记\C++ primer中文版（第五版）\表8-1.png)

IO类型间的关系

​	概念上设备类型和字符大小都不会影响我们要执行的IO操作。

注意事项：

​	我们不能拷贝或对IO对象赋值。

条件状态

​	IO操作与生俱来的问题是可能发生错误。

![image-20221208170007735](E:\download\book\自学\看书笔记\C++ primer中文版（第五版）\表8-2.png)

直接整练习题

练习8.1

​	编写函数，接受一个istream&参数，返回值类型也hi是istream&.此函数从给定的流中读取数据，直至遇到文件结束标识时停止。将读取的数据打印在标准输出上。

文件结束标识：（EOF）

  从键盘输入文件结束符(end-of-file):
  在Windows系统中，输入文件结束符的方法是按Ctrl+Z，然后按Enter或Return键。
  在Unix系统中，包括在Mac OS X系统中，文件结束符输入Ctrl+D。

```C++
std::istream &getIstream(std::istream & in)
{
    int input;
    while(in >> input)
    {   
        std::cout << input << std::endl;
    }
    
    return in;
}

int main(int argc, char const *argv[])
{
    getIstream(std::cin);
    return 0;
}

//写了一个最简单的，其实可以考虑使用表8.2中的内容
```



管理缓冲输出：

​	导致缓冲刷星（即，数据真正写到输出设备或文件）的原因有很多

​		1.程序正常结束，作为main函数的return操作的一部分，缓冲刷新立即被执行。

​		2.缓冲区满时，需要刷新缓冲，而后新的数据才能继续写入缓冲区。

​		3.可以使用endl来显式刷新缓冲区。

​		4.在每个输出操作后，可以使用unitbuf设置流的那日不状态，来清空缓冲区。默认情况下，对cerr是设置unitbuf的，因此

​			写到cerr的内容都是立即刷新的。

​		5.一个输出流可以关联到了另外一个流。在这种情况下，当读写被关联的流时，关联到流的缓冲区被刷新。

​			举例：cin和cout都是关联到cout，因此读cin和写cerr都会导致cout的缓冲区被刷新。

IO库中刷新输出缓冲区的操作符

​	endl //输出一个换行符，然后刷新缓冲区

​	ends //输出一个空字符，然后刷新缓冲区。

​	flush //刷新缓冲区

unitbuf操作符

如果想每次输出都刷新缓冲区

std::cout << unitbuf;

恢复正常的系统管理的缓冲区刷新机制

std::cout << nounitbuf;

```C++
int main(int argc, char const *argv[])
{
    std::cout << "hello world!! endl" << std::endl;
    std::cout << "hello world!! ends" << std::ends;
    std::cout << "hello world!! flush"<< std::flush;
    std::cout << "after flush." << std::endl;

    std::cout << std::unitbuf << "flush buf.";  
    std::cout << "flush buf 1";

    std::cout << std::nounitbuf << "flush by sys.";
    std::cout << "flush by sys 1";
    return 0;
}

//输出内容
//hello world!! endl
//hello world!! ends hello world!! flushafter flush.
//flush buf.flush buf 1flush by sys.flush by sys 1
```

警告：如果程序崩溃，输出缓冲区不会被刷新。

如果程序异常终止，输出缓冲区是不会被刷新的。当一个程序崩溃后，它所输出的数据可能停留在输出缓冲区中等待打印。

当调试一个已经崩溃的程序时，需要确认那些你认为已经输出的数据确实已经刷新了，否则，可能将大量的时间浪费在追踪为什么代码没有执行上，而实际上代码已经执行了，只是程序崩溃后缓冲区没有被刷新，输出数据被挂起没有打印而已。

关联输入和输出流

当一个输入流被关联到输出流时，任何试图从输入流中读取数据的操作都会**先**刷新关联的输出流。

```c++
int main(int argc, char const *argv[])
{
    std::cout << "..........\n" ; //11
    int aa;
    std::cin >> aa;
    int *a = nullptr;
    std::cout << *a;   //防止11是因为程序正常结束刷新缓冲
    std::cout << "crash????";
    return 0;
}

//可以看到先输出了“"..........”，然后输入正常数据后，程序死掉。
```

以上是默认的关联

还有一种使用tie将流关联的方式

推荐一个网站：https://legacy.cplusplus.com/

搜索ios::tie 

## 8.2 文件输入输出

头文件fstream中定义了三个类型来支持文件IO

1.ifstream从给定的文件中读取数据

2.ofstream向给定的文件中写入数据

3.fstream可以读写给定文件

除了继承自iostream类型的行为外，fstream中定义的类型还增加了一些新的成员来管理与流关联的文件。

![image-20221209115948915](E:\download\book\自学\看书笔记\C++ primer中文版（第五版）\表8-3.png)

自动构造和析构

当一个fstream对象离开其作用域时，与之关联的文件会自动关闭。

当一个fstream被销毁时，close会自动被调用。

文件模式

每个流都有一个关联的文件模式，用来指出如何使用文件。

![](E:\download\book\自学\看书笔记\C++ primer中文版（第五版）\表8-4.png)



练习：打开一个文件，把里面的内容输出到屏幕上，然后写入另外一个文件中。

```C++
#include <iostream>
#include <string>
#include <fstream>
int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cout << "run like this, ./a.exe infilename outfilename" << std::endl;
        return 0;
    }

    std::ifstream in(argv[1]);
    std::ofstream out(argv[2]);
    std::string content;
    while (std::getline(in, content))
    {
        static int lineNum = 1;
        /* code */
        std::cout << "Line:" << lineNum << "\n"
                  << content << std::endl;
        lineNum++;
        out << content << "\n";
    }

    in.close();   //可以不手动关闭，对象销毁时自动close.
    out.close();
    return 0;
}
//文件的内容是：
//Constructs an ofstream object that is not associated with any file.
//Internally, its ostream base constructor is passed a pointer to a newly constructed filebuf object (the internal file stream //buffer).
```

**如何避免以out模式打开文件丢失已有数据的情况？**

​	使用模式app, ofstream out("filename", ostream::app) 

注意事项：

​	每次打开文件时，都要设置文件模式，可能是显式地设置，也可能是隐式的设置。当程序未指定时使用默认值。

## 8.3 string流

sstream头文件定义了三个类型来支持内存IO.s

1.istringstream从string读取数据。

2.ostringstream向string中写入数据

3.stringstream可从string读数据，也可以向string中写数据

![](E:\download\book\自学\看书笔记\C++ primer中文版（第五版）\表8-5.png)

结合课后题：

给定一个 电话簿，获取其中的电话号码，将电话号码格式化后(除去号码中非数字的内容)，以指定格式（name,numbers,numbers...）写入另外的电话簿中

```C++
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

class PersionInfo
{
public:
    std::string fromatOut(const char split = ',')
    {
        std::string info = _name;
        for (auto phone : _phones)
        {
            info += std::string(1, split) + phone;
        }
        return info;
    }

    void setName(std::string name) { _name = name; }
    void addPhone(std::string phone) { _phones.push_back(phone); }

private:
    std::string _name;
    std::vector<std::string> _phones;
};

void removeNonnumbers(std::string &str)
{
    std::string tmp;
    for (auto ch : str)
    {
        if (std::isdigit(ch))
        {
            tmp += std::string(1, ch);
        }
    }
    
    if (!tmp.empty())
    {
        str = tmp;
    }
}

int main(int argc, char const *argv[])
{
    // 从文件中读取
    if (argc < 2)
    {
        return 0;
    }
    
    std::ifstream in(argv[1]);
    std::ofstream out(argv[2]);
    std::string info;
    std::vector<PersionInfo> people;
    
    while (getline(in, info))
    {
        PersionInfo men;
        std::istringstream record(info);
        std::string name, phone;
        record >> name;
        men.setName(name);

        while (record >> phone)
        {
            removeNonnumbers(phone);
            men.addPhone(phone);
        }

        std::cout << "info:" << men.fromatOut() << std::endl;
        out << men.fromatOut() << "\n";
        people.push_back(men);
    }

    return 0;
}
```

txt1.txt

goudan 111111111111 11131111111 1141115*116
ergouzi 12345678944 01234578961 23456789101
zhifubao 22233344455 11122233344

## 小结

C++使用标准库类来处理面向流的输入和输出：

iostream 处理控制台io。

fstream 处理命令文件io

stringstream完成内存string的io

类fstream和stringstream都是继承自类iosteam的。

每个IO对象都维护一组条件状态，用来指出此对象上是否可以进行IO操作。