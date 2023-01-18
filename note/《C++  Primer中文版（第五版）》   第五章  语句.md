---
typora-copy-images-to: ./
---

# 《C++  Primer中文版（第五版）》   第五章  语句

## 简单语句

C++语句中大多数的语句以分号结束。（一不小心就会漏掉分号）

#### 什么时候使用空语句？

如果在程序的某个地方，语法上需要一条语句，但是逻辑上不需要，此时应该使用空语句

```c++
//重复读入数据直至文件末尾或某次输入的值等于sougth(这样写有点看不惯)
while (std::cin >> s && s != "sougth")
    ;

//还是这种格式 
while (std::cin >> s && s != "sougth")
{}

//还是说是这种格式
while (std::cin >> s && s != "sougth")
{
	;
}
```

#### 什么是复合语句？

指用花括号括起来的（可能为空）语句和声明序列(这样就相当于把多条语句当成一条语句来处理)。

补充说明：

复合语句也被称为块，一个块就是一个作用域。

## 语句作用域

在复合语句内部定义的变量，仅在相应语句的内部能够可见，一旦语句结束，变量也就超出其作用范围了。

## 条件语句

#### 什么是条件语句？

字面意思，在一定条件下才会执行的语句。

#### C++中有那些条件语句？

一种是if语句，一种是switch

### if语句

if语句有那些形式？

if和if..else

```C++
if(condition)
	statement
    
if(condition)
{
	statement
}
else
{
	statement
}
```

补充说明：

**C++中它规定else与离他最近的尚未匹配的if匹配。**

```c++
//这是人为写成这样的，看起来第一个if和else缩进一致，应该是一组
if (grade % 10 >= 3)
    if (grade % 10 > 7)
        lettergrade += '+';
else
    lettergrade += '-';

//这是vscode 格式化工具，格式化后第二个if和else才是真的一对。
if (grade % 10 >= 3)
    if (grade % 10 > 7)
        lettergrade += '+';
    else
        lettergrade += '-';

//关于“{”加在什么地方，这是两大派系。
//if...else嵌套的时候，尽量加上"{}"。如果if后面跟了"{}"，那么else后也跟"{}"(个人习惯，感觉这样好看些）
if (grade % 10 >= 3)
{
   if (grade % 10 > 7)
   {
       lettergrade += '+';
   }
}
else
{
    lettergrade += '-';
}
```

### switch语句

格式

```C++
switch(expr)//expr 能够转换成整数
{
	case标签1：//case 1: case关键字和其对应的值一起被称为case标签 
        statement
        break;//执行case标签1下的内容后就退出，若不加会继续执行case标签2下的内容
    case标签2：
        statement
        break;
    case标签n:
        statement
        break;
    default:
        statement
        break;
}
```

补充说明：

switch后面紧跟的表达式必须是能够转换成**整数**。

case关键字对应的值必须是**整型常量表达式**。

case关键字对应的值**不能够重复**。

## 迭代语句

### while语句

形式：

```c++
while(condition)
	staement
```



### 传统的for语句

```c++
for(init-statement; condition; expression)
    statement
```

补充说明：

init-statement 可以同时定义多个对象，但是这些对象的在同一条声明语句中。

for语句头能省略（写成空语句）init-statement/condition/expression中的任意一个（或者是全部）。

### 范围for语句

C++11引入了一种更简单的for语句，这种语句能够遍历容器或其他序列的所有元素。

```C++
for(declaration:expression)
	statement
```

#### 什么是范围变量？

declaration中定义的那个变量。

#### expression有什么要求？

expression 必须是一个序列。（拥有能返回迭代器的begin和end成员）

补充说明：

在declaration定义变量的时候：确保序列中的每个元素都能转换成该变量的类型（最简单的是直接用auto）

如果需要对序列中的元素进行写操作，那么范围变量必须是引用。

### do while语句

形式：

```c++
do
	statement
while (condition);
```

do while 和while的区别？

do while会先执行一次循环体，然后再判断循环条件。 而while是先判断循环条件，然后再执行循环体。

```c++
//自己写了一段体会一下
std::vector<int> vec = {0, 1, 2, 3};
for (auto ri : vec)
{
    std::cout << "ri = " << ri << std::endl;
    int i = ri;
    do
    {
        i++;
        std::cout << "do while  i = " << i << std::endl;
    } while (i < 2);

    i = ri;
    while (i < 2)
    {
        i++;
        std::cout << "while  i = " << i << std::endl;
    }
    std::cout << std::endl;
}
```

以上程序的执行结果

![image-20220216103330015](D:\看书笔记\C++ primer中文版（第五版）\dowhileandwhile.png)

#### 看到执行结果 是不是感觉有点发现？

do while 至少执行一次，while可能一次都不执行。

do while 若第一次条件为真，那么执行次数和while相同。

## 跳转语句

C++提供了四种跳转语句：break,continue,goto和return（return在函数相关章节介绍）

### break语句

#### break语句作用是什么？

负责中止离他**最近**的while，do while,for或switch语句，并从这些语句**之后的第一条**语句开始执行。

### continue语句

#### continue语句的作用是什么？

终止**最近**的<u>**循环**</u>中的当前迭代并立即开始下一次迭代。

### goto语句

#### goto语句的作用？

从goto语句无条件跳转到同一个函数内的另一条语句。（除了练习以外，在工作中从未使用过）

形式：

goto label;

label是用于标记语句的标识

label格式：

label:statement

补充说明：

label可以和程序中的其他实体标识符使用同一个名字。

## try语句和异常处理

#### try语句的作用？

当程序出现异常后，不至于直接挂掉。

### throw语句

**抛出异常将终止当前的函数，并把控制权转移给能处理该异常的代码**。

#### 如何抛出异常？

使用throw表达式

格式：

throw expression;



### try语句块

try语句块一开始是关键字try,后面紧跟着一个块，跟在try块之后的是**一个或多个**catch子句。

catch 可以有多个，但至少要有一个。

格式：

```c++
try
{
	program-statements
}
catch(exception-declaration)
{
	handler-statements
}// ...


```

#### 有多个catch子句时，怎么执行？

当选中了某个catch子句处理异常之后，执行与之对应的块。**catch一旦完成，程序跳转到try语句块最后一个catch子句之后的那条语句继续执行**。

#### 有没有能够捕获任何异常的catch子句？

有，使用catch(...)

### 标准异常

C++中定义了一组类，用于报告标准库函数遇到的问题。分别在四个头文件中。

exception头文件定义了最通用的异常类exception，只报告异常的发生，不提供任何信息。

**stdexcept**头文件定义了集中常用的异常类

**new**头文件中定义了bad_alloc异常类型。

**type_info**中定义了bad_cast异常类型。

![image-20220217094955657](D:\看书笔记\C++ primer中文版（第五版）\stdexcept.png)



try...catch 语句的执行过程是：

- 执行 try 块中的语句，如果执行的过程中没有异常拋出，那么执行完后就执行最后一个 catch 块后面的语句，所有 catch 块中的语句都不会被执行；
- 如果 try 块执行的过程中拋出了异常，那么拋出异常后立即跳转到第一个“异常类型”和拋出的异常类型匹配的 catch 块中执行（称作异常被该 catch 块“捕获”），执行完后再跳转到最后一个 catch 块后面继续执行。

```c++
void throwString()
{
    try
    {
        throw std::string("try catch test,throw string.");
    }
    catch (std::string str)
    {
        std::cout << str << std::endl;
    }
}

void throwInt()
{
    try
    {
        throw 111;
    }
    catch (int i)
    {
        std::cout << " catch int " << i << std::endl;
    }
}

void throwChar()
{
    try
    {
        throw 'c';
    }
    catch (char ch)
    {
        std::cout << "catch char " << ch << std::endl;
    }
}

void throwFloat()
{
    try
    {
        throw 3.14f;
    }
    catch (float fl)
    {
        std::cout << "catch float " << fl << std::endl;
    }
}

void throwDouble()
{
    try
    {
        throw 3.15;
    }
    catch (double dou)
    {
        std::cout << "catch double " << dou << std::endl;
    }
}

void throwCatchAny()
{
    try
    {
        throw "catch any...";
    }
    catch (...)
    {
        std::cout << "can not get str" << std::endl;
    }
}

void stdTryCatch()
{
    try
    {
        std::vector<int> vi = {1, 2, 3, 4, 5};
        std::cout << vi.at(100) << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "std::exception e.what() = " << e.what() << '\n';
    }
}

int main(int argc, char **argv)
{
    throwString();
    throwChar();
    throwDouble();
    throwFloat();
    throwInt();
    throwCatchAny();
    stdTryCatch();
    return 0;
}
```

执行结果：

try catch test,throw string.
catch char c
catch double 3.15
catch float 3.14
catch int 111
can not get str
std::exception e.what() = vector::_M_range_check: __n (which is 100) >= this->size() (which is 5)