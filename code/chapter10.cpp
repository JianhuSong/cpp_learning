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
