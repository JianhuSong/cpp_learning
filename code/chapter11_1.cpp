#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <stdexcept>
#include <string>
#include <sstream>
#include <fstream>

void map_and_set()
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
}

void multiset_test()
{
    std::vector<int> ivec;
    for (std::vector<int>::size_type i = 0; i != 10; ++i)
    {
        ivec.push_back(i);
        ivec.push_back(i);
    }

    std::set<int> iset(ivec.begin(), ivec.end());
    std::multiset<int> imset(ivec.begin(), ivec.end());
    std::cout << ivec.size() << std::endl;
    std::cout << iset.size() << std::endl;
    std::cout << imset.size() << std::endl;
}

std::pair<std::string, int> process(std::vector<std::string> &v)
{
    if (!v.empty())
        return {v.back(), v.back().size()}; // 列表初始化
    else
        return std::pair<std::string, int>(); // 隐式构建返回值
}

void pair_test()
{
    // std::vector<std::string> v{"hello", "world"};
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


int main(int argc, char const *argv[])
{
    // map_and_set();
    // multiset_test();
    // pair_test();
    // multimap_test();
    word_transform(argv[1], argv[2]);
    return 0;
}
