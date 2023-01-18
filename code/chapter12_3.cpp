#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <sstream>
#include <fstream>

using line_no = std::vector<std::string>::size_type;   //在两个例子中都有使用，干脆直接在外面写好了。

class QueryResult
{
    friend std::ostream &print(std::ostream &, const QueryResult &);

public:
    QueryResult(std::string s,
                std::shared_ptr<std::set<line_no>> p, 
                std::shared_ptr<std::vector<std::string>> f) : sought(s), lines(p), file(f) {}

private:
    std::string sought;                             //查询单词
    std::shared_ptr<std::set<line_no>> lines;       //出现的行号，原例子中，这里这个line_no 并未定义
    std::shared_ptr<std::vector<std::string>> file; //输入文件
};


class QueryResult;

class TextQuery
{
public:
    TextQuery(std::ifstream &is) : file(new std::vector<std::string>)
    {
        std::string text;
        while (std::getline(is, text))   
        {
            file->push_back(text);
            std::cout <<text<<std::endl;
            int n = file->size() - 1;
            std::istringstream line(text);
            std::string word;
            while (line >> word)
            {
                auto &lines = wm[word];
                if (!lines)
                {
                    lines.reset(new std::set<line_no>);
                }

                lines->insert(n);
            }
        }
    }

    QueryResult query(const std::string &sought) const
    {
        static std::shared_ptr<std::set<line_no>> nodata(new std::set<line_no>);
        auto loc = wm.find(sought);
        if (loc == wm.end())
            return QueryResult(sought, nodata, file);
        else
            return QueryResult(sought, loc->second, file);
    }

private:
    std::shared_ptr<std::vector<std::string>> file;
    std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;
};

//该函数为6.3.2-值是如何返回的 书中例子（没记住，百度查的）
//如果ctr大于1,返回word的复数形式
std::string make_plural(std::size_t ctr, const std::string &word, const std::string &ending)
{
    return (ctr > 1) ? word + ending : word;
}

std::ostream &print(std::ostream &os, const QueryResult &qr)
{
    os << qr.sought << " occurs " << qr.lines->size() << " "
       << make_plural(qr.lines->size(), "time", "s") << std::endl;

    for (auto num : *qr.lines)
    {
        os << "\t(line " << num + 1 << ")" << *(qr.file->begin() + num) << std::endl;
    }
    
    return os;
}

// 与用户交互,打印给定单词的查询结果
void runQueries(std::ifstream &infile)
{
    // infile 是一个ifstream指向我们要处理的文件
    TextQuery tq(infile);     //保存文件并建立查询map
    while (true)
    {
        std::cout << "enter word to look for, or q to quit: ";
        std::string s;

        //若遇到文件尾或用户输入了‘q’时循环退出
        if (!(std::cin >> s) ||s == "q")
            break;

        //指向查询并打印结果
        print(std::cout, tq.query(s)) << std::endl;
    }
}

int main(int argc, const char **argv)
{
    if(argc <= 1)
    {
        std::cout <<"The number of parameters is incorrect.  eg: ./a.exe filename"<<std::endl;
        return -1;
    }

    std::ifstream infile(argv[1]);
    runQueries(infile);

    return 0;
}