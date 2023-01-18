# 《C++  Primer中文版（第五版）》   第11章 关联容器

关联容器和顺序容器有着根本的不同：关联容器中的元素是按照关键字来保存和访问的。

虽然关联容器的很多行为与顺序容器相同，但其不同之处反映了关键字的作用。

关联容器支持高效的关键字查找和访问。两个主要的关联容器类型是map和set。

标准库提供了8个关联容器。这8个容器间的不同体现在三个维度上：

(1).每个容器或者是一个set,或者是一个map

(2).或者要求不重复的关键字，或者允许重复的关键字

(3).按顺序保存元素，或无序保存。

关联容器类型总结：

允许重复关键字的容器的名字中都包含单词multi

不保持关键字按顺序存储的容器的名字都一单词unordered开头

| 按关键字有序保存元素 |                                  |
| -------------------- | -------------------------------- |
| map                  | 关联数组：保存关键字-值对        |
| set                  | 关键字即值，即只保存关键字的容器 |
| multimap             | 关键字可重复出现的map            |
| multiset             | 关键字可重复出现的set            |
| **无序集合**         |                                  |
| unordered_map        | 用哈希函数组织的map              |
| unordered_set        | 用哈希函数组织的set              |
| unordered_multimap   | 哈希组织的map;关键字可重复出现   |
| unordered_multiset   | 哈希组织的set;关键字可重复出现   |



## 使用关联容器

map是关键字-值对的集合。map类型通常被称为关联数组。关联数组与“正常”数组类似，不同之处在于其下标不必是整数。

set就是关键字的简单组合。当只想知道一个值是否存在时，set是最有用的。

类似顺序容器关联容器也是模板。

**使用map**

为了**定义**一个**map**,我们**必须指定关键字和值的类型**

**使用set**

​	为了定义一个set，**必须指定其元素类型**。

```c++
#include <iostream>
#include <map>
#include <set>

int main(int argc, char const *argv[])
{
    std::map<std::string, std::size_t> word_count;
    std::set<std::string> exclude = {"The", "But", "And", "Or", "An", "A", "the", "but", "and", "or", "an", "a"};
    std::string word;
    std::cout << "please input the word:" << std::endl;

    while (std::cin >> word)
    {
        if (word == "XXXX")
        {
            break;
        }

        if (exclude.find(word) == exclude.end())
        {
            ++word_count[word];
        }
    }

    for (const auto &w : word_count)
        std::cout << w.first << " occurs " << w.second << ((w.second > 1) ? " times." : " time.") << std::endl;

    return 0;
}
```

### 关联容器概述

关联容器（有序的和无序的）都支持9.2节中介绍的普通容器操作。

关联容器不支持顺序容器的位置相关操作。

关联容器也不支持构造函数或插入操作这些接受一个元素和一个数量值的操作。

关联容器的迭代器都是双向的。



#### 定义关联容器

每个关联容器都定义了一个默认的构造函数，它创建一个指定类型的空容器。

我们可以将关联容器初始化为另一个同类型容器的拷贝，或是从一个值范围来初始化关联容器，只要这些值可以转化为容器所需类型就可以。

**初始化multimap或multiset**

一个map或set中的关键字必须是唯一的。multimap和multiset没有此限制

```c++
    std::vector<int> ivec;
    for (std::vector<int>::size_type i = 0; i != 10; ++i)
    {
        ivec.push_back(i);
        ivec.push_back(i);
    }

    std::set<int> iset(ivec.begin(), ivec.end());
    std::multiset<int> imset(ivec.begin(), ivec.end());
    std::cout << ivec.size() << std::endl;   //20
    std::cout << iset.size() << std::endl;   //10
    std::cout << imset.size() << std::endl;  //20 
```

#### 关键字类型的需求

关联容器对其关键字类型有一些限制：

对于有序容器（map/multimap/set/multiset），关键字类型必须定义元素比较的方法。

默认情况下，标准库使用关键字类型的<运算符来比较两个关键字。

在集合类型中，关键字类型就是元素类型；

在映射类型中，关键字类型就是元素的第一个部分的类型。

##### 有序容器的关键字类型

可以向一个算法提供我们自己定义的比较操作，与之类似，也可以提供自己定义的操作符来代替关键字上的<运算符。所提供的操作必须在关键字类型上定义一个严格弱序。它必须具备以下基本性质：

​	两个关键字不能同时“小于等于”对方

​	如果k1"小于等于"k2，且k3"小于等于"k3，那么k1必须“小于等于”k3

​	如果存在两个关键字，任何一个都不"小于等于"另一个，那么我们称这两个关键字是"等价"的。如果k1“等价于"k2，且k2"等价于"k3，那么k1必须"等价于"k3。

如果两个关键字是等价的，那么容器将它们视作相等来处理。当作为map关键字时，只能有一个元素与这两个关键字关联，我们可以用两者中任意一个来访问对应的值

##### 使用关键字类型比较函数

为了指定使用自定义的操作，必须在定义容器类型时提供此操作的类型。如前所述，用尖括号指出要定义那种类型的容器，自定义的操作必须在尖括号中紧跟元素类型给出。

```c++
bool compareIsbn(const Sales_data &lhs, const Sales_data &rhs)
{
	return lhs.isbn() < rhs.isbn();
}

std::multiset<Sales_data, decltype(compareIsbn)*> bookstore(compareIsbn);
//用compareIsbn来初始化对象，这表示当我们向bookstore添加元素时使用compareIsbn来为这些元素排序。
```

#### pair类型

名为pair的标准库类型，它定义在头文件utility中

一个pair保存两个数据成员。类似容器，pair是一个用来生成特定类型的模板。当创建一个pair时，我们必须提供两个类型名，pair数据成员将具有对应的类型。

两个类型不要求一样。

pair的默认构造函数对数据成员进行值初始化。

我们也可以为每个成员提供初始化器。

与其他标准库类型不同，pair的数据成员时public的，两个成员分别命名为first和second.

| pair上的操作              |                                                              |
| ------------------------- | ------------------------------------------------------------ |
| pair<T1, T2>p;            | p是一个pair,两个类型分别为T1和T2的成员都进行了值初始化 。    |
| pair<T1, T2>p(v1, v2);    | p是一个成员类型为T1和T2的pair,firs和second成员分别用v1和v2进行初始化。 |
| pair<T1, T2>p = {v1, v2}; | 等价于p(v1, v2)                                              |
| make_pair(v1, v2);        | 返回一个用v1和v2初始化的pair,pair的类型从v1和v2类型推断出来。 |
| p.first                   | 返回p的名为first的公有数据成员                               |
| p.second                  | 返回p的名为second的公有数据成员                              |
| p1 relop p2               | 关系运算符（<、>、<=、>=）按字典序定义，当p1.first < p2.first或!(p2.first < p1.first) && p1.second < p2.second成立时， p1 < p2为true。 |
| p1 == p2                  | 当first和second分别相等时，两个pair相等。 相等性             |
| p1 != p2                  | 判断利用元素的==来实现。                                     |

##### 创建pair对象的函数

想象有一个函数需要返回一个pair。在新标准下，我们可以对返回值进行列表初始化

```c++
std::pair<std::string, int> process(std::vector<std::string> &v)
{
    if (!v.empty())
        return {v.back(), v.back().size()}; // 列表初始化
    else
        return std::pair<std::string, int>(); // 隐式构建返回值
}

void pair_test()
{
    //std::vector<std::string> v{"hello", "world"};
    std::vector<std::string> v;
    std::vector<std::pair<std::string, int>> v1;
    for (int i = 0; i < 5; ++i)
        v1.push_back(process(v));
    std::cout << "pair v1.size() = " << v1.size() << std::endl;

    for (int i = 0; i != v1.size(); ++i)
    {
        std::cout << "v1[" << i << "].first = " << v1[i].first << ", v1[" << i << "].second = " << v1[i].second << std::endl;
    }
}
```

### 关联容器的操作

| 表11.3：关联容器额外的类型别名 |                                                              |
| ------------------------------ | ------------------------------------------------------------ |
| key_type                       | 此容器类型的关键字类型                                       |
| mapped_type                    | 每个关键字关联的类型：只适用于map                            |
| value_type                     | 对于set，与key_type相同； 对于map，为pair<const key_type, mapped_type> |

```C++
std::set<string>::value_type v1;       // v1是一个string
std::set<string>::key_type v2;		   // v2是一个string
std::map<string, int>::vlue_type v3;   // v3是一个pair<string, int>
std::map<string, int>::key_type v4;	   // v4是一个string
std::map<string, int>::mapped_type v5; // v5是一个int
```



### 关联容器迭代器

当解引用一个关联容器迭代器时，我们会得到一个类型为容器的value_type的值的引用。

必须记住,一个map的value_type是一个pair，我们可以改变pair的值，但不能改变关键字成员的值。

**set的迭代器时const的**

虽然set类型同时定义了iterator和const_iterator类型，但两种类型都只允许访问set中的元素。

**遍历关联容器**

map和set类型都支持begin和end操作。我们可以用这些函数获取迭代器，然后用迭代器来遍历容器。

**关联容器和算法**

在实际编程中，如果我们真要对一个关联容器使用算法，要么时将它当作一个源序列，要么当作一个目的位置。

### 添加元素

关联容器的insert成员向容器中添加一个元素或一个元素范围。由于map和set包含不重复的关键字，因此插入一个已存在的元素对容器没有任何影响。

insert有两个版本，分别接受一对迭代器，或是一个初始化器列表。

**向map添加元素**

对于一个map进行insert操作时，必须记住元素类型时pair。

```c++
//向word_count插入word的四种方法
word_count.insert({word, 1});
word_count.insert(make_pair(word, 1));
word_count.insert(pair<string, int>(word, 1));
word_count.insert(map<string, int>::value_type(word, 1));
```

| 表11.4:关联容器insert操作                         |                                                              |
| ------------------------------------------------- | ------------------------------------------------------------ |
| c.insert(v)          c.emplace(args)              | v时vlue_type类型的对象； args用来构造一个元素 ；对于map和set，只有当元素的关键字不在c中时才插入元素。 函数返回一个pair，包含一个迭代器，指向具有指定关键字的元素，以及一个指示插入是否成功的bool值。对于multimap和multiset,总会插入给定元素，并返回一个指向新元素的迭代器。 |
| c.insert(b, e)          c.insert(il)              | b和e是迭代器，表示一个c::value_type类型的值范围；il是这种值的花括号列表。函数返回void。对于map和set，只插入关键字不在c中的元素。对于multimap和multiset,则会插入范围中的每个元素 |
| c.insert(p, v)                 c.emplace(p, args) | 类似insert(v)（或emplace(args)），当将迭代器p作为一个提示，指出从哪里开始搜索新元素应该存储的位置。返回一个迭代器，指向具有给定关键字的元素。 |

**检测insert的返回值**

insert（emplace）返回值依赖于容器类型和参数。

向multiset或multimap添加元素

对于允许重复关键字的容器，接受单个元素的insert操作返回一个指向新元素的迭代器 。这里无须返回一个bool值，因为insert总是向这类容器中加入一个新元素。

### 删除元素

| 表11.5：从关联容器删除元素 |                                                              |
| -------------------------- | ------------------------------------------------------------ |
| c.erase(k)                 | 从c中删除每个关键字为K的元素， 返回一个size_type值，指出删除元素的数量。 |
| c.erase(p)                 | 从c中删除迭代器p指定的元素。p必须指向c中一个真实元素，不能等于c.end()。返回一个指向p之后元素的迭代器，若p指向c中的尾元素，则返回c.end() |
| c.erase(b, e)              | 删除迭代器对b和e所表示范围中的元素。返回e                    |

### map的下标操作

map下标运算符接受一个索引，获取与此关键字相关联的值。如果下标不在map中，会为它创建一个元素并插入到map中，关联值将进行值初始化

| c[k]    | 返回关键字为k的元素；如果k不在c中，添加一个关键字为k的元素，对其进行值初始化 |
| ------- | ------------------------------------------------------------ |
| c.at[k] | 访问关键字为k的元素，带参数检查；若k不在c中，抛出一个out_of_range异常 |

**使用下标操作的返回值**

当对一个map进行下标操作是，会获得一个mapped_type对象；当解引用一个map迭代器时，会得到一个value_type对象。

map的下标运算符返回一个左值，由于返回的是一个左值，所以我们既可以读也可以写元素。

### 访问元素

关联容器提供多种查找一个指定元素的方法。

lower_bound和upper_bound不适用于无须容器

下标和at操作只适用于非const的map和unordered_map

| c.find(k)        | 返回一个迭代器，指向第一个关键字值为k的元素，若k不在容器中，返回尾后迭代器。 |
| ---------------- | ------------------------------------------------------------ |
| c.count(k)       | 放回关键字等于k的数量。对于不允许重复关键字的容器，返回值永远是0或1 |
| c.lower_bound(k) | 返回一个迭代器，指向第一个关键字不小于k的元素                |
| c.upper_bound(k) | 返回一个迭代器，指向第一个关键字大于k的元素                  |
| c.equal_range(k) | 返回一个迭代器pair，表示关键字等于k的元素范围，若k不存在，pair的两个成员均等于c.end() |

**对map使用find代替下标操作**

当我们只想知道一个给定关键字是否在map中，而不想改变map。在这种情况下，应该使用find

**在multimap或multimap中查找元素**

如果一个multimap或multiset中有多个元素具有给定关键字，则这些元素在容器中会相邻存储。

```c++
void multimap_test()
{
    std::multimap<std::string, int> mymultimap;
    for (int i = 0; i < 5; i++)
    {
        mymultimap.insert(std::make_pair("hello", i));
        mymultimap.insert(std::pair<std::string, int>(std::string("XXXX"), i + 100)); // 连续插入两个值，最后看看相同的key是否连续存储。
    }

    int keycount = mymultimap.count("hello");
    std::cout << "mymultimap.count(\"hello\") = " << mymultimap.count("hello") << std::endl;

    auto iter = mymultimap.find("hello");
    while (keycount)
    {
        std::cout << iter->second << " "; // 0 1 2 3 4
        ++iter;                           // 如果一个multimap或multiset中有多个元素具有给定的关键字，则这些元素在容器中会相邻存储。
        --keycount;
    }

    std::cout << std::endl;
}
```

**一种不同的，面向迭代器的解决方法**

使用lower_bound或upper_bound,如果关键字在容器中，lower_bound返回的迭代器指向第一个具有给定关键字的元素，而upper_bound返回最后一个戈丁关键字元素之后的位置。如果不在，这两个都返回一个指向一个不影响排序的关键字插入位置。

注意事项：lower_bound和upper_bound返回的迭代器可能是容器的尾后迭代器。

**equal_range函数**

此函数接受一个关键字，返回一个迭代器pair。若关键字存在，第一个迭代器指向第一个与关键字匹配的元素，第二个迭代器指向最有一个匹配元素之后的位置。若未找到匹配元素，则两个迭代器都指向关键字可以插入的位置。

### 一个单词转换的map

本节就是一个程序

```c++
std::map<std::string, std::string> buildMap(std::ifstream &map_file)
{
    std::map<std::string, std::string> trans_map;
    std::string key;
    std::string value;

    while (map_file>> key && std::getline(map_file, value))
    {
        if (value.size() > 1)
            trans_map[key] = value.substr(1);
        else
            throw std::runtime_error("no rule for" + key);
    }
    return trans_map;
}

const std::string& transform(const std::string &s, const std::map< std::string, std::string> &m)
{
    auto map_it = m.find(s);
    if(map_it != m.cend())
        return map_it->second;
    else
        return s;
}

void word_transform(const char* map_file_name, const char* input_file_name)
{
    try
    {
        std::ifstream map_file(map_file_name, std::ifstream::in);
        std::ifstream input(input_file_name, std::ifstream::in);

        auto trans_map = buildMap(map_file);
        std::string text;
        while (std::getline(input, text))
        {
            std::istringstream stream(text);
            std::string word;
            bool firstword = true;
            while(stream >> word)
            {
                if(firstword)
                    firstword  = false;
                else
                    std::cout << " ";
                std::cout << transform(word, trans_map);
            }
            std::cout << std::endl;
        }
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
```



## 无序容器

新标准定义了4个无序关联容器。这些容器不使用比较运算符来组织元素，而是使用一个哈希函数和关键字类型的==运算符。

**什么时候使用无序关联容器？**

如果关键字类型固有就是无序的，或者性能测试发现问题可以使用哈希技术解决，就可以使用无序容器。

**使用无序容器**

除了哈希管理操作之外，无序容器还提供了与有序容器相同的操作（find,insert等）。

无序容器也有允许重复关键子的版本。

**管理桶**

无序容器在存储上组织为一个桶，每个桶保存零个或多个元素。无序容器使用一个哈希函数将元素映射到桶。为了访问一个元素，容器首先计算元素的哈希值，它指出应该搜索哪个桶。容器将具有一个特定哈希值的所有元素都保存到一个桶中。如果容器允许重复关键字，所有具有相同关键字的元素也都会在同一个桶中。因此无序容器的性能依赖于焊锡函数的质量和桶的数量和大小。

对于相同的参数，哈希函数总是必须产生相同的结果。理想情况下，哈希函数还能将每个特定的值映射到唯一的桶。但是将不同关键字的元素映射到相同的桶也是允许的。

| 桶接口                 |                                                              |
| ---------------------- | ------------------------------------------------------------ |
| c.bucket_count()       | 正在使用的桶数目                                             |
| c.max_bucket_count()   | 容器能容纳的最多的桶的数量                                   |
| c.bucket_size(n)       | 第n个桶有多少个元素                                          |
| c.bucket(k)            | 关键字为k的元素在哪个桶中                                    |
| 桶迭代                 |                                                              |
| local_iterator         | 可以用来访问桶中元素的元素迭代器                             |
| const_local_iterator   | 桶迭代器的const版本                                          |
| c.begin(n), c.end(n)   | 桶n的首元素迭代器和尾后迭代器                                |
| c.cbegin(n), c.cend(n) | 于前两个函数类似，但返回const_local_iterator                 |
| 哈希策略               |                                                              |
| c.load_factor()        | 每个桶的平均元素数量，返回float值                            |
| c.max_loac_factor()    | c试图维护的平均桶大小，返回float值。c会在需要时添加新的桶，以使得load_factor<=max_load_factor |
| c.rehash(n)            | 重组存储，使得bucket_count>=n且bucket_count>size/max_load_factor |
| c.reserve(n)           | 重组存储，使得c可以保存n个元素且不必rehash                   |

无序容器的关键字要求

默认情况下，无序容器使用关键字类型的==运算符来比较元素，它们还使用一个hash<key_type>类型的对象来生成每个元素的hash值。标准库为内置类型（包括指针）提供了hash模板。还为一些标准库类型，包括string和智能指针定义了hash。因此我们可以直接定义关键字类型是内置类型（包括指针类型）、string还是智能指针类型的无序容器。

我们不能直接定义关键字类型为自定义类类型的无序容器。

我们不使用默认的hash，而是使用另一种方法，类似于为有序容器重载关键字类型的默认比较操作。

```c++
size_t hasher(const Sales_data &sd)
{
    return hash<string>()(sd.isbn);
}

bool eqOp(const Sales_data &lhs, const Sales_data &rhs)
{
    return lhs.isbn() == rhs.isbn();
}

using SD_multiset = unordered_multiset<Sales_data, decltype(hasher)*, decltype(eqOp)*>;
SD_multiset bookstore(42,hasher,eqOp);
```

## 小结

关联容器支持通过关键字高效查找和提取元素。

标准库提供了8个关联容器，每个容器

​	是一个map或是一个set。map保存关键字，set只保存关键字

​	要求关键字类型唯一或不要求

​	保持关键字有序或不保证有序

有序容器使用比较函数来比较（关键字类型的<）关键字，从而将元素按顺序存储。无序容器使用关键字类型的==和一个hash<key_type>类型的对象来组织元素

允许关键字重复的容器的名字中都有multi,使用hash技术的容器名字都是以unordered开头

关联容器和顺序容器有很多共同的元素。

无论在有序还是无序的关联容器中，具有相同关键字的元素都是相邻存储的。