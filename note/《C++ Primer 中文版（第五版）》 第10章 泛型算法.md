# 第10章 泛型算法

​		标准库容器定义的操作集合惊人得小。标准库并未给每个容器添加大量功能，而是提供了一组算法，这些算法大多数都独立于任何特性的容器。这些算法是通用的：它们可用于不同类型的容器和不同类型的元素。

## 10.1 概述

**这些个算法在哪里？**

大多数的算法都定义在头文件algorithm中。头文件中numeric定义了一组数值泛型算法。

### **迭代器令算法不依赖于容器**

**如何访问值？**

利用迭代器的解引用运算符可以实现元素访问。

**如何移动？**

用迭代器的递增运算符可以移动到下一个元素

### 但是算法依赖于元素类型的操作。

大多数算法都是用了一个(或多个)元素类型上的操作。

## 10.2 初识泛型算法

标准库提供了多少算法？

超过100

这些个算法有什么特点？

除少数例外，标准库算法都是对一个范围内的元素进行操作。我们将此元素范围称为“输入范围”，接受输入范围的算法**总是用前两个参数**来表示此范围，两个参数分别是指向要处理的第一个元素和**尾元素之后**的位置。

### 只读算法

find/count/accumulate

find:

"Find value in range

**Returns an iterator to the first element** in the range **`[first,last)`** that compares equal to val. **If no** such element is found, the function **returns last.**"

```c++
#include <iostream>  // std::cout
#include <algorithm> // std::find
#include <vector>    // std::vector
#include <iterator>  // std::distance

int main(int argc, char const *argv[])
{
    int myints[] = {10, 20, 30, 30, 30, 40};
    int *p;

    p = std::find(myints, myints + 5, 30);
    std::cout << "last = " << *p << std::endl;

    std::vector<int> ints(myints, myints + 5);
    auto it = std::find(ints.begin(), ints.begin() + 5, 30);
    std::cout << "index = " << std::distance(ints.begin(), it) << std::endl;
    return 0;
}

//last = 30
//index = 2
```

count

"Count appearances of value in range

Returns the number of elements in the range `[first,last)` that compare equal to val."

```c++
#include <iostream>  // std::cout
#include <algorithm> // std::count
#include <vector>    // std::vector

int main(int argc, char const *argv[])
{
    int myints[] = {10, 20, 30, 30, 30, 40};
    int times = std::count(myints, myints + 5, 30);
    std::cout << "times = " << times << std::endl;

    std::vector<int> ints(myints, myints + 5);
    int times2 = std::count(ints.begin(), ints.begin() + 5, 30);
    std::cout << "times2 = " << times2 << std::endl;
    return 0;
}
//输出结果
//times = 3
//times2 = 3
```

accumulate

"Accumulate values in range

Returns the result of accumulating all the values in the range `[first,last)` to init.

The default operation is to add the elements up, but a different operation can be specified as binary_op."

```c++
#include <iostream>   // std::cout
#include <functional> // std::minus
#include <numeric>    // std::accumulate

int myfunction(int x, int y) 
{
    std::cout << "using myfunction"<<std::endl; 
    return x + 2 * y; 
}

struct myclass
{
    int operator()(int x, int y) 
    {
        std::cout << "myclass()"<<std::endl;  
        return x + 3 * y; 
    }
} myobject;

int main(int argc, char const *argv[])
{
    int init = 100;
    int numbers[] = {10, 20, 30};

    std::cout << "using default accumulate: ";
    std::cout << std::accumulate(numbers, numbers + 3, init);
    std::cout << '\n';

    std::cout << "using functional's minus: ";
    std::cout << std::accumulate(numbers, numbers + 3, init, std::minus<int>());
    std::cout << '\n';

    std::cout << "using custom function: ";
    std::cout << std::accumulate(numbers, numbers + 3, init, myfunction);
    std::cout << '\n';

    std::cout << "using custom object: ";
    std::cout << std::accumulate(numbers, numbers + 3, init, myobject);
    std::cout << '\n';

    return 0;
}

//输出结果
// using default accumulate: 160
// using functional's minus: 40
// using custom function: using myfunction
// using myfunction
// using myfunction
// 220
// using custom object: myclass()
// myclass()
// myclass()
// 280
```

### 操作两个序列的算法

equal用于确定两个序列是否保存相同的元素值。它将第一个序列中的每个元素与第二个序列中的对应元素进行比较。如果所有对应元素都相等，则返回true,否则返回false。

两个序列的元素类型必须相同么？

不是，只要能用==来比较两个元素类型即可。

注意事项：equal基于一个非常重要的假定：它假定第二个序列至少与第一个序列一样长。

|  equality (1) | `template <class InputIterator1, class InputIterator2>  bool equal (InputIterator1 first1, InputIterator1 last1,              InputIterator2 first2); ` |
| ------------: | ------------------------------------------------------------ |
| predicate (2) | `template <class InputIterator1, class InputIterator2, class BinaryPredicate>  bool equal (InputIterator1 first1, InputIterator1 last1,              InputIterator2 first2, BinaryPredicate pred);` |

Compares the elements in the range `[first1,last1)` with those in the range beginning at first2, and returns `true` if all of the elements in both ranges match.

The elements are compared using `operator==` (or pred, in version *(2)*).

first1, last1

[Input iterators](https://legacy.cplusplus.com/InputIterator) to the **initial and final positions** of the first sequence. The range used is `[first1,last1)`, which contains all the elements between first1 and last1, including the element pointed by first1 but not the element pointed by last1.

first2

[Input iterator](https://legacy.cplusplus.com/InputIterator) to the **initial position** of the second sequence. The comparison includes up to as many elements of this sequence as those in the range `[first1,last1)`.

pred

Binary function that accepts **two elements as argument** (one of each of the two sequences, in the same order), and **returns a value convertible to `bool`**. The value returned indicates whether the elements are considered to match in the context of this function.
The function shall **not modify** any of its arguments.
This can either be a **function pointer or a function object.**

```c++
#include <iostream>  // std::cout
#include <algorithm> // std::equal
#include <vector>    // std::vector

bool mypredicate(int i, int j)
{
    return (i > j);
}

int main(int argc, char const *argv[])
{
    int myints[] = {20, 40, 60, 80, 100};          //   myints: 20 40 60 80 100
    std::vector<int> myvector(myints, myints + 5); // myvector: 20 40 60 80 100
    std::vector<int> myvector1(5,101);             // myvector1: 101 101 101 101 101
    // using default comparison:
    if (std::equal(myvector.begin(), myvector.end(), myints))
        std::cout << "The contents of both sequences are equal.\n";
    else
        std::cout << "The contents of both sequences differ.\n";

    // using predicate comparison:
    if (std::equal(myvector1.begin(), myvector1.end(), myvector.begin(), mypredicate))
        std::cout << "All elements in the first sequence are greater than the second sequence.\n"; 
    else
        std::cout << "Not all elements in the first sequence are greater than the second sequence.\n";

    return 0;
}

//输出结果
//The contents of both sequences are equal.
//All elements in the first sequence are greater than the second sequence.
```

### 写容器的算法

一些算法将新值赋予序列中的元素。当我们使用这类算法时，必须注意确保序列的**原大小至少不小于**我们要求算法写入的元素数目。

一些算法会自己向输入范围写入元素。它们最多写入与给定序列一样多的元素

算法不检查写操作

一些算法接受一个迭代器来指出一个单独的目的位置。这些算法将新值赋予一个序列中的元素，该序列从目的位置迭代器指向的元素开始。

注意事项：**向目的位置迭代器写入数据的算法假定目的位置足够大，能容纳要写入的元素**。

#### 介绍back_inserter

一种保证算法有足够元素空间来容纳输出数据的方法是**插入迭代器**

通常怎么使用？

我们常常使用back_inserter来创建一个迭代器，作为算法的目的位置来使用。

```C++
std::vector<int> vec;
std::fill_n(back_inserter(vec),10,0);
```

### 拷贝算法

拷贝（copy）算法是另一种向目的位置迭代器指向的输出序列中的元素写入数据的算法。此算法接受三个迭代器，前两个表示输入范围，第三个表示目的序列的起始位置。

注意事项：**传递给copy的目的序列至少要包含与输入序列一样多的元素**。

```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
void myPrint(int a1[], int length)
{
    for (int i = 0; i < length; ++i)
    {
        std::cout << a1[i] << " ";
    }

    std::cout << std::endl;
}

void myPrint(const std::vector<int> &vec)
{
    for (auto ve : vec)
    {
        std::cout << ve << " ";
    }

    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    int a1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int a2[sizeof(a1) / sizeof(*a1)];
    std::vector<int> vec;

    auto ret = std::copy(std::begin(a1), std::end(a1), a2);
    std::cout << "copy return - begin: " << std::distance(a2, ret) << std::endl;
    std::cout << "after copy a2: ";
    myPrint(a2, sizeof(a2) / sizeof(*a2));

    std::replace(std::begin(a1), std::end(a1), 0, 42);
    std::cout << "after replace a1: ";
    myPrint(a1, sizeof(a1) / sizeof(*a1));

    std::replace_copy(std::begin(a1), std::end(a1), std::back_inserter(vec), 1, 55);
    std::cout << "after replace_copy a1: ";
    myPrint(a1, sizeof(a1) / sizeof(*a1));
    std::cout << "after replace_copy vec: ";
    myPrint(vec);

    std::replace_copy(std::begin(a1), std::end(a1), std::back_inserter(vec), 2, 66);
    std::cout << "after replace_copy agin vec: ";
    myPrint(vec);
    return 0;
}

//输出结果
// copy return - begin: 10
// after copy a2: 0 1 2 3 4 5 6 7 8 9
// after replace a1: 42 1 2 3 4 5 6 7 8 9
// after replace_copy a1: 42 1 2 3 4 5 6 7 8 9
// after replace_copy vec: 42 55 2 3 4 5 6 7 8 9
// after replace_copy agin vec: 42 55 2 3 4 5 6 7 8 9 42 1 66 3 4 5 6 7 8 9
```

### 重排元素的算法

某些算法会重排容器中元素的顺序，一个明显的例子就是sort。

```C++
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <string>
void elimDups(std::vector<std::string> &words)
{
    std::sort(words.begin(), words.end());
    auto end_unique = std::unique(words.begin(), words.end());
    words.erase(end_unique, words.end());
}

void myPrint(const std::vector<std::string> &words)
{
    for (auto word : words)
    {
        std::cout << word << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    std::vector<std::string> words = {"the", "quick", "red", "fox", "jumps", "over", "the", "slow", "red", "turtle"};
    std::cout << "Before: ";
    myPrint(words);
    elimDups(words);
    std::cout << "After: ";
    myPrint(words);
    return 0;
}

// 输出结果
// Before: the quick red fox jumps over the slow red turtle
// After: fox jumps over quick red slow the turtle

```

## 10.3 定制操作

很多算法都会比较输入序列中的元素。默认的情况下， 这类算法使用元素类型的<或==运算符完成比较。标准库还为这些算法定义了额外的版本，允许我们提供自己定义的操作来代替默认运算符。

为什么需要使用我们自己定义的操作？

可能我们希望的排序顺序与<所定义的顺序不同，或是我们序列可能保存的是未定义<运算符的元素类型。

### 向算法传递函数

可以查看想要使用的算法是否支持自定义操作(谓词)。然后按照相关的要求定义操作就行。

**什么是谓词**？

谓词是一个**可调用**的表达式，其返回结果是一个能用作条件的值。

**标准库算法所使用的谓词种类**？

标准库算法所使用的谓词有两类：一元谓词（只接受单一参数）和二元谓词（有两个参数）

**定义谓词时的参数要求是什么**？

参数的个数要符合该标准库算法所支持的谓词类型（一元/二元）

元素类型必须能转换成谓词的参数类型。

**什么是调用运算符？**

“()”,调用运算符是一对圆括号，里面放置实参列表（可能为空）

什么是可调用对象？

对于一个对象或表达式，如果可以使用调用运算符，则称它为可调用的。

### lambda表达式

**什么是lambda表达式？**

一个lambda表达式表示一个可调用的代码单元。

**lambda的格式**

[capture list] (parameter list) -> return type { function body}

capture list:捕获列表是一个lambda所在函数中定义的局部变量列表（通常为空）

parameter list:参数列表

return type: 返回值类型

function body:函数体

**lambda表达式与函数有什么区别？**

lambda可以定义在函数内部。

**定义一个lambda表达式必须严格按照 格式么？**

我们可以忽略参数列表和返回类型，但是必须永远包含捕获列表和函数体。

注意事项：

在lambda中忽略括号和参数列表等价于指定一个空参数列表

如果忽略返回类型，lambda根据函数体中的代码推断除返回类型。

如果lambda的函数体包含任何单一return之外的内容，且未指定返回类型，则返回void.

### 使用捕获列表

虽然lambda可以出现在一个函数中，使用其局部变量，但它**只能**使用那些**明确指明**的变量。

lambda如何使用函数中的多个变量？

我们可以在[]中提供一个以逗号分隔的名字列表，这些名字都是它所在函数中定义的。

### lambda捕获和返回

当向一个函数传递一个lambda时，同时听译了新类型和该类型的一个对象：传递参数就是此编译器生成的类类型的未命名对象。

### 值捕获

与传值参数类似，采用值捕获的前提时变量可以拷贝。与参数不同，被捕获的变量的值实在lambda创建时拷贝，而不是调用时拷贝。

```c++
void fcn2()
{
    size_t v1 = 42;
    auto f2 = [v1] {return v1;}; //创建lambda完成了值拷贝
    v1 = 0;                      //本次修改v1的值不会影响到lambda中保存的v1的值。
    auto j = f2();               //j=42
}
```

### 引用捕获

引用捕获和返回引用有着相同的问题和限制。如果我们采用引用方式捕获一个变量，就必须确保被保存的引用的对象在lambda执行时时存在的。

```c++
void fcn2()
{
    size_t v1 = 32;
    auto f2 = [&v1] {return v1;}; //f2中保存了v1的引用
    v1 = 0;                       //本次修改会影响到f2中的v1
    auto j = f2();                //j=0
}
```

### 隐式捕获

除了显式列出我们希望使用的来自所在函数的变量之外，还可以让编译器根据lambda体中的代码来推断我们要使用那些变量。&告诉编译器采用捕获引用的方式。

=告诉编译器采用值捕获的方式。

显式和隐式可以混用么？

可以，混用时捕获列表中第一个元素必须是=/&。后面在跟与第一个元素**不同**的捕获方式的显式捕获。

**建议：尽量保持lambda的变量捕获简单化**

一个lambda捕获从lambda被创建到自身执行（可能多次执行）这段时间内保存的相关信息。确保lambda**每次执行**的时候这些信息**都有预期的意义**，是**程序员的责任**。

捕获一个普通的比那连，如int、string或其他非指针类型，通常可以采用简单的值捕获方式

如果我们捕获一个指针或迭代器，或采用引用捕获方式，就必须确保在lambda执行时，绑定到迭代器、指针或引用的对象仍然存在。

### 指定lambda返回类型

默认情况下，如果一个**lambda体**包含**return之外的任何语句**，则编译器假定此lambda**返回void**

### 参数绑定

标准库bind函数

格式

auto newCallable = bind(callable, arg_list);

newCallable: 是一个可调用对象

callable: 原来的可调用对象

arg_list:是一个用逗号分隔的参数列表，其中的参数可能包含形如_n的名字，其中n是一个占位符，表示newCallable的参数。

使用placeholders名字

名字_n都定义在placeholders的命名空间中，而这个命名空间本身定义在std命名空间中

bind的参数

可以用bind绑定给定可调用兑现的参数或重新安排其顺序。

为什么有些时候要使用ref?

bind会拷贝其参数，有时对有些绑定的参数我们希望以引用方式传递，或是要绑定的参数的类型无法拷贝。这个时候使用标准库ref.

```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <string>
#include <functional> //bind/ref/cref

int myFunc(int &a, int &b)
{
    std::cout << a << std::endl;
    a = a + 100;
    b = a - b;
    return -1;
}

int main(int argc, char const *argv[])
{
    int a = 10, b = 100;
    // auto bf = std::bind(myFunc, std::placeholders::_1); //不管bf在调用时有多少实参,实际使用的就只有第一个参数
    // auto bf = std::bind(myFunc, std::placeholders::_2, b); //不会报错,但是b的值不会被改变，或者说改变的是bind拷贝的那个b
    auto bf = std::bind(myFunc, std::placeholders::_2, std::ref(b)); // b的值会改变
    bf(1, a);
    std::cout << a << std::endl;
    std::cout << b << std::endl;
}
```



## 10.4 再探迭代器

### 迭代器种类：

插入迭代器：这些迭代器绑定到一个容器上，可用来向容器插入元素

流迭代器：这些迭代器被绑定到输入输出刘尚，可用来遍历所关联的IO流。

反向迭代器：这些迭代器不是向前移动，而是向后移动。除了forward_list之外的标准容器都有反向迭代器。

移动迭代器：这些专用的迭代器不是拷贝其中的元素，而是移动它们。

 

### 插入迭代器

插入器是一种迭代器适配器，他接受一个容器生成一个迭代器，能实现向给定容器添加元素。

插入迭代器有三种类型，差异在于元素插入的位置

back_inserter

​	创建一个使用push_back的迭代器

front_inserter

​	创建一个使用push_front的迭代器

inserter

​	创建一个使用insert的迭代器。

注意事项：只有在容器支持push_front的情况下，我们才可以使用front_inserter。类似的，只有在容器支持push_back的情况下，我们才能使用back_inserter

```c++
#include <iostream>
#include <algorithm>
#include <iterator>
#include <list>

int main(int argc, char const *argv[])
{
    std::list<int> c(4, 100);
    auto it = std::inserter(c, c.begin());
    *it = 1234;                                                  //c里面是:1234 100 100 100 100
    std::list<int>  lst2, lst3;

    std::copy(c.cbegin(), c.cend(), std::front_inserter(lst2));  //lst2里面是:100 100 100 100 1234
    std::copy(c.begin(), c.end(), std::back_inserter(lst3));     //lst3里面是:1234 100 100 100 100
    return 0;
}
```



### iostream迭代器

通过流迭代器，我们可以使用泛型算法从流对象读取数据以及向其写入数据。

#### istream_iterator操作

当创建一个流迭代器时，**必须指定**迭代器将要**读写**的**对象类型**。

对于一个绑定到流的迭代器，一旦其关联的流遇到文件流或遇到IO错误，迭代器的值与尾后迭代器相等。

#### 使用算法操作流迭代器

由于算法使用迭代器操作来处理数据，而流迭代器又至少支持某些迭代器操作，因此我们至少可以用某些算法来操作流迭代器。

#### istream_terator允许使用懒惰求值

当我们将一个istream_iterator绑定到一个流时，标准库并不保证迭代器立即从流读取数据。标准库保证的是，在我们第一次解引用之前，从流中读取数据的操作已经完成。

#### ostream_iterator操作

我们可以对任何具有输出运算符的类型定义ostream_iterator。

必须将ostream_iterator绑定到一个指定的流，不允许空的或表示尾后位置的ostream_iterator

运算符*和++对ostream_iterator对象不做任何事情。

```c++
#include <iostream> 
#include <iterator>
#include <vector>
#include <algorithm>

void printElm(const std::vector<int> &ints)
{
    // std::ostream_iterator<int> out(std::cout);
    std::ostream_iterator<int> out(std::cout, " ");  //定义时必须绑定到流上
    for (auto ve : ints)
    {
        *out++ = ve;  //虽说*和++不会对out做任何事情,可以省略,但是建议写成这样
    }
    std::cout << std::endl;

    // for (auto ve : ints)
    // {
    //     std::cout << ve << " ";
    // }
}

int main(int argc, char const *argv[])
{
    // 定义时若未指定读取的数据类型,那么迭代器的值与尾后迭代器相等.
    std::istream_iterator<int> in_inter(std::cin), eof;
    std::vector<int> vec(in_inter, eof);
    printElm(vec);
    return 0;
}
```



### 反向迭代器

反向迭代器就是在容器中从尾部元素向首元素反向移动的迭代器。对于反向迭代器，递增（递减）操作的含义会颠倒过来。

除了forward_list外，其他容器都支持反向迭代器。

#### 反向迭代器需要递减运算符

我们只能从既支持++又支持--的迭代器来定义反向迭代器。

不能从一个forward_list或一个流迭代器创建反向迭代器。

#### 反向迭代器和其他迭代器间的关系

普通迭代器与反向迭代器的关系反应了左闭右合区间特性。

感觉练习题有点意思就写上了

```c++
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <list>
#include <functional>

void init_int_vector(std::vector<int> &vec, int init, int length)
{
    for (int i = 0; i < length; i++)
    {
        vec.push_back(init + i);
    }
}

// 练习10.34:使用reverse_iterator逆序打印一个vector
void reverse_pintf(std::vector<int> vec)
{
    using vector_int_it = std::reverse_iterator<std::vector<int>::iterator>;
    vector_int_it rev_end(vec.begin()); // 反向的,原来的begin是反向的end
    vector_int_it rev_begin(vec.end()); // 反向的,原来的end,是反向的begin

    for (; rev_begin != rev_end; ++rev_begin)
    {
        std::cout << *rev_begin << " ";
    }
    std::cout << std::endl;
}

// 练习10.35:使用普通迭代器逆序打印一个vector
void normal_reverse_pintf(std::vector<int> vec)
{
    // for (auto it = vec.rbegin(); it != vec.rend(); it++)
    for (auto it = vec.crbegin(); it != vec.crend(); it++)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

// 练习10.36:使用find在一个init的list中查找最后一个值为0的元素
int find_last_zero(const std::list<int> lst)
{
    auto it = std::find(lst.rbegin(), lst.rend(), 0);
    if (it != lst.rend())
    {
        // 反向的index + 正向index = index max
        return lst.size() - 1 - std::distance(lst.rbegin(), it);
    }

    return -1;
}

// 练习给定一个包含10个元素的vector,将位置3~7之间的元素逆序拷贝到一个list中.
void vecotr_reverse_copy_to_list(std::vector<int> vec,
                                 std::vector<int>::iterator first,
                                 std::vector<int>::iterator last,
                                 std::list<int> &lst)
{
    std::reverse_copy(first, last, std::insert_iterator<std::list<int>>(lst, lst.begin()));
}

int main(int argc, char const *argv[])
{
    std::vector<int> vec;
    init_int_vector(vec, 10, 5);
    reverse_pintf(vec);

    vec.clear();
    init_int_vector(vec, 0, 10);
    normal_reverse_pintf(vec);

    std::list<int> lst = {9, 5, 0, 1, 2, 0, 3, 0, 4, 0, 5, 0, 1};
    std::cout << "The index " << find_last_zero(lst) << " is the last 0." << std::endl;

    vec.clear();
    lst.clear();
    init_int_vector(vec, 0, 10);
    vecotr_reverse_copy_to_list(vec, vec.begin() + 3, vec.begin() + 7, lst);
    for (auto lst_em : lst)
    {
        std::cout << lst_em << " ";
    }
    std::cout << std::endl;
    return 0;
}

```

## 10.5 泛型算法结构

任何算法的最基本特性是它要求其迭代器提供哪些操作。

算法要求的迭代器操作可以分为5类

| 输入迭代器     | 只读，不写，单遍扫描，只能递增       |
| -------------- | ------------------------------------ |
| 输出迭代器     | 只写，不读，单遍扫描，只能递增       |
| 前向迭代器     | 可读写，多遍扫描，只能递减           |
| 双向迭代器     | 可读写，多遍扫描，可递增递减         |
| 随机访问迭代器 | 可读写，多遍扫描，支持全部迭代器运算 |

### 类迭代器

类似容器，迭代器也定义了一组公共操作。一些操作所有迭代器都支持，另外一些只有特定类别的迭代器才支持。

迭代器是按照它们所提供的操作来分类的，而这种分类形成了一种层次。除输出迭代器外，一个高层类别的迭代器支持低层迭代器的所有操作。

C++标准明确指明了泛型和数值算法的每个迭代器参数的最小类别。

**注意事项：对于向一个算法传递错误类别的迭代器的问题，很多编译器不会给出任何警告或提示。**

#### 输入迭代器

输入迭代器：可以读取序列中的元素，一个输入迭代器必须支持

​	用于比较两个迭代器的相等和不相等运算符（==、!=）

​	用于推进迭代器的前置和后置递增运算（++）

​	用于读取元素的解引用运算符（*）；解引用指挥出现在赋值运算符的右侧。

​	箭头运算符(),等价于(->).member,即解引用迭代器，并提取对象的成员

输入迭代器**只用于顺序访问**

istream_iterator是一种输入迭代器。

#### 输出迭代器

可以看作输入迭代器功能上的补集------只写而不读元素。输出迭代器必须支持

​	用于推进迭代器的前置和后置递增运算++

​	解引用运算符(*)，只出现在赋值运算的左侧（向一个已经解引用的输出迭代器赋值，就是将值写入它所指向的元素）

我们**只能**向一个输出迭代器**赋值一次**。

ostream_iterator类型也是输出迭代器。

#### 前向迭代器

可以读写元素。

这类迭代器只能在序列中沿一个方向移动。

前向迭代器支持所有输入和输出迭代器的操作，而且可以多次读写同一个元素。

算法replace要求前向迭代器，forward_list上的迭代器是前向迭代器。

#### 双向迭代器

可以正向/反向读写序列中的元素。

除了支持所有前向迭代器的才做之外，双向迭代器还支持前置和后置递减运算符（--）。

算法reverse要求双向迭代器，除了forward_list之外，其他标准库都提供符合双向迭代器要求的迭代器。

#### 随机访问迭代器

提供在常量时间内访问序列中任意元素的能力。

此类迭代器支持双向迭代器的所有功能，还支持

​	用于比较两个迭代器相对位置的关系运算符（<,<=,>和>=）

​	迭代器和一个整数值的加减运算（+，-，+=，-=）,计算结果是迭代器在序列中前进（或后退）给定整数个元素后的位置。

​	用于两个迭代器上的减法运算符(-)，得到两个迭代器的距离。

​	下标运算符（iter[n]），与*(iter[n])等价。

算法sort要求随机访问迭代器。array/deque/string和vector的迭代器都是随机访问迭代器，用于随机访问内置数组元素的指针也是。

### 算法形参模式

在任何其他算法分类之上，还有一组参数规范。

大多数算法具有如下四种形式之一：

```c++
    alg(beg, end, other args);
    alg(beg, end, dest, other args);
    alg(beg, end, beg2, other args);
    alg(beg, end, beg2, end2, other args);
```



#### 接受单个目标迭代器的算法

dset参数是一个表示算法可以写入目的位置的迭代器。

注意：向输出迭代器写入数据的算法都假定目标空间足够容纳写入的数据。

#### 接受第二个输入序列的算法

接受beg2或是接受beg2和end2的算法用这些迭代器表示第二个输入范围。

如果一个算法接受beg2和end2，这两个迭代器表示第二个范围。

只接受单独的beg2都算法将beg2作为第二个输入范围中的首元素。此范围的结束位置未指定，这些算法假定从beg2开始的范围与beg和end所表示的范围至少一样大。

### 算法命名规范

除了参数规范，算法还遵循一套命名和重载规范。

#### 一些算法使用重载形式传递一个谓词

接受谓词参数来代替<或==运算符的算法，以及哪些不接受参数的算法，通常都是重载函数。

#### _if版本的算法

接受一个元素值的算法通常有另一个不同名版本，该版本接受一个谓词代替元素值。接受谓词参数的算法都有附加_if前缀

#### 区分拷贝元素的版本和不拷贝元素的版本

默认情况下，重排元素的算法将重排后的元素写回给定的输入序列中。这些算法还提供另外一个版本，将元素写到一个指定的输出位置。

写到额外目的空间的算法都在名字后面加一个_copy.

一些算法同时提供了__copy和_if版本，这些版本接受一个目的位置迭代器和一个谓词

```c++
remove_if(v1.begin(), v1.end(),
         			  [] (int i) {return i%2;});
remove_copy_if(v1.begin(), v1.end(), back_inserter(v2),
              				[] (int i) {return i%2;});
```

## 10.6 特定容器算法

与其他容器不同，链表类型list和forward_list定义了几个成员函数形式的算法。它们定义了独有的sort,remove,reverse和unique.

链表类型定义的其他算法的通用版本可以用于链表，但代价太高。

建议：对于list和forward_list，应该优先使用成员函数版本的算法而不是通用算法。

| 这些操作都返回void    |                                                              |
| --------------------- | ------------------------------------------------------------ |
| lst.merge(lst2)       | 将来自lst2的元素合并入lst.lst和lst2都是有序的。              |
| lst.merge(lst2, comp) | 元素将从lst2中删除。在合并之后，lst2变为空。第一个版本使用<运算符；第二个版本使用给定的比较操作 |
| lst.remove(val)       | 调用erase删除掉与给定值相等每个元素                          |
| lst.remove_if(pred)   | 调用erase删除掉与给定值令一元谓词为真的每个元素              |
| lst.reverse()         | 反转lst中的元素                                              |
| lst.sort()            | 使用<比较排序元素                                            |
| lst.sort(comp)        | 使用给定比较操作排序元素                                     |
| lst.unique()          | 调用erase删除同一个值的连续拷贝，该版本使用==                |
| lst.unique(pred)      | 功能同上，该版本使用给定的二元谓词。                         |

#### splice成员

链表类型定义了splice算法，此算法为链表数据结构所特有的。

| lst.splice(args)或flst.splice_after(args) |                                                              |
| ----------------------------------------- | ------------------------------------------------------------ |
| (p, lst2)                                 | p是一个指向lst中元素的迭代器，或一个指向flst首前位置的迭代器。函数将lst2的所有元素移动到lst中p之前的位置或flst中p之后的位置。将元素从lst2中删除。lst2的类型必须与lst或flst相同，且不能是同一个链表。 |
| (p, lst2, p2)                             | p2是一个指向lst2中位置的有效迭代器。将p2指向的元素移动到lst中，获将p2之后的元素移动到flst中。lst2可以是与lst或flst相同的链表。 |
| (p, lst2, b, e)                           | b和e必须表示lst2中的合法范围。将给定范围的元素从lst2移动到lst或flst.lst2与lst(flst)可以是相同的链表，但p不能指向给定范围中的元素 |



#### 链表的操作会改变容器

多数链表特有的算法都与通用版本很相似，但不完全相同。链表特有版本与通用版本间的一个至关重要的区别是链表版本会改变底层的容器

```c++
#include <iostream>
#include <list>
#include <string>
#include <iterator>

void init_list(std::list<int> &lst, int init, int length)
{
    for (int i = 0; i < length; ++i)
    {
        lst.push_back(init + i);
    }
}

void list_printf(const std::list<int> &lst, std::string name = "")
{
    if (!name.empty())
    {
        std::cout << name << " : ";
    }

    for (auto _lst : lst)
    {
        std::cout << _lst << " ";
    }

    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    std::list<int> lst, lst2;
    init_list(lst, 5, 3);
    init_list(lst2, 0, 5);
    list_printf(lst, "lst");
    list_printf(lst2, "lst2");
    // lst.merge(lst2);
    lst.merge(lst2, [](int x, int y)
              { return x < y; });
    list_printf(lst, "lst"); // 输出0 1 2 3 4 5 6 7

    lst.remove(2);
    list_printf(lst, "lst"); // 输出0 1 3 4 5 6 7

    lst.remove_if([](int x)
                  { return x > 4; });
    list_printf(lst, "lst"); // 输出0 1 3 4

    lst.reverse();
    list_printf(lst, "lst"); // 4 3 1 0

    lst.push_back(3);
    lst.push_back(4);
    lst.push_back(3);
    list_printf(lst, "lst"); // 4 3 1 0 3 4 3

    lst.sort();
    list_printf(lst, "lst"); // 0 1 3 3 3 4 4

    lst.sort([](int x, int y)
             { return x > y; }); // 值大的元素排前面
    list_printf(lst, "lst");     // 4 4 3 3 3 1 0

    lst.unique();
    list_printf(lst, "lst"); // 4 3 1 0
    init_list(lst, 0, 10);
    lst.sort();
    list_printf(lst, "lst"); // 0 0 1 1 2 3 3 4 4 5 6 8 9

    lst.unique([](int x, int y)
               { return y == x + 1; }); // 后一个元素比前一个元素大1的话就删除
    list_printf(lst, "lst");            // 0 0 2 4 4 6 8
    lst.unique();

    init_list(lst2, 10, 2);

    auto it = lst.begin();
    // auto it = lst.begin() + 1; //没有定义+
    it++;
    lst.splice(it, lst2);

    list_printf(lst, "lst"); // 0 10 11 2 4 6 8

    init_list(lst2, 10, 2);
    lst.splice(it, lst2, lst2.begin());
    list_printf(lst, "lst"); // 0 10 11 10 2 4 6 8
    lst2.push_back(14);

    lst.splice(++it, lst2, lst2.begin(), lst2.end());
    list_printf(lst, "lst"); // 0 10 11 2 11 14 4 6 8

    return 0;
}
```



## 小结

标准库定义了大约100个类型无关的对序列进行操作的算法。序列可以是标准库容器类型中的元素，一个内置数组或者是通过读写一个流来生成的。

根据支持的操作不同，迭代器分为五类：输入，输出，前向，双向以及随机访问迭代器。如果一个迭代器支持某个迭代器类别的操作，则属于该类别。

如同迭代器根据操作分类一样，传递给算法的迭代器参数也按照所要求的操作进行分类。

算法不直接改变它们所操作的序列的大小。

虽然算法不能向序列中添加元素，但是插入迭代器可以做到。

容器forward_list和list对一些通用算法定义了自己特有的版本。