#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <string>
#include <memory>
#include <algorithm>

class QueryResult
{
    friend std::ostream &print(std::ostream &, const QueryResult &);

public:
    using line_no = std::vector<std::string>::size_type;
    QueryResult(std::string s,
                std::shared_ptr<std::set<line_no>> p,
                std::shared_ptr<std::vector<std::string>> f) : sought(s), lines(p), file(f) {}

private:
    std::string sought;                             // 查询单词
    std::shared_ptr<std::set<line_no>> lines;       // 出现的行号，原例子中，这里这个line_no 并未定义
    std::shared_ptr<std::vector<std::string>> file; // 输入文件
};

class QueryResult;

class TextQuery
{
public:
    using line_no = std::vector<std::string>::size_type;
    TextQuery(std::ifstream &is) : file(new std::vector<std::string>)
    {
        std::string text;
        while (std::getline(is, text))
        {
            file->push_back(text);
            std::cout << text << std::endl;
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

// 该函数为6.3.2-值是如何返回的 书中例子（没记住，百度查的）
// 如果ctr大于1,返回word的复数形式
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
    TextQuery tq(infile); // 保存文件并建立查询map
    while (true)
    {
        std::cout << "enter word to look for, or q to quit: ";
        std::string s;

        // 若遇到文件尾或用户输入了‘q’时循环退出
        if (!(std::cin >> s) || s == "q")
            break;

        // 指向查询并打印结果
        print(std::cout, tq.query(s)) << std::endl;
    }
}



class Query_base
{
    friend class Query;

protected:
    using line_no = TextQuery::line_no;
    virtual ~Query_base() = default;

private:
    virtual QueryResult eval(const TextQuery &) const = 0;
    virtual std::string rep() const = 0;
};

class Query
{
    friend Query operator~(const Query &);
    friend Query operator|(const Query &, const Query &);
    friend Query operator&(const Query &, const Query &);

public:
    Query(const std::string &s) : q(new WordQuery(s)) {}
    QueryResult eval(const TextQuery &t) const { return q->eval(t); }
    std::string rep() const { return q->rep(); }

private:
    Query(std::shared_ptr<Query_base> query) : q(query) {}
    std::shared_ptr<Query_base> q;
};

std::ostream &operator<<(std::ostream &os, const Query &query)
{
    return os << query.rep();
}

class WordQuery : public Query_base
{
    friend class Query;
    WordQuery(const std::string &s) : query_word(s) {}
    QueryResult eval(const TextQuery &t) const
    {
        return t.query(query_word);
    }
    std::string rep() const { return query_word; }
    std::string query_word;
};

class NotQuery : public Query_base
{
    friend Query operator~(const Query &);
    NotQuery(const Query &q) : query(q) {}
    std::string rep() const { return "~(" + query.rep() + ")"; }
    QueryResult eval(const TextQuery &text) const
    {
        auto result = query.eval(text);
        auto ret_lines = std::make_shared<std::set<line_no>>();
        auto beg = result.begin(), end = result.end();
        auto sz = result.get_file()->size();
        for (std::size_t n = 0; n != sz; ++n)
        {
            if (beg == end || *beg != n)
                ret_lines->insert(n);
            else if (beg != end)
            {
                ++beg;
            }
        }

        return QueryResult(rep(), ret_lines, result.get_file());
    }
    Query query;
};

inline Query operator~(const Query &operand)
{
    return std::shared_ptr<Query_base>(new NotQuery(operand));
}

class BinaryQuery : public Query_base
{
protected:
    BinaryQuery(const Query &l, const Query &r, std::string s) : lhs(l), rhs(r), opSym(s) {}
    std::string rep() const { return "(" + lhs.rep() + " " + opSym + "" + rhs.rep() + ")"; }
    Query lhs, rhs;
    std::string opSym;
};

class AndQuery : public BinaryQuery
{
    friend Query operator&(const Query &, const Query &);
    AndQuery(const Query &left, const Query &right) : BinaryQuery(left, right, "&") {}
    QueryResult eval(const TextQuery &text) const
    {
        auto right = rhs.eval(text), left = lhs.eval(text);
        auto ret_lines = std::make_shared<std::set<line_no>>();
        std::set_intersection(left.begin(), left.end(),
                              right.begin(), right.end(),
                              std::inserter(*ret_lines, ret_lines->begin()));
        return QueryResult(rep(), ret_lines, left.get_file());
    }
};

inline Query operator&(const Query &lhs, const Query &rhs)
{
    return std::shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}

class OrQuery : public BinaryQuery
{
    friend Query operator|(const Query &, const Query &);
    OrQuery(const Query &left, const Query &right) : BinaryQuery(left, right, "|") {}
    QueryResult eval(const TextQuery &text) const
    {
        auto right = rhs.eval(text), left = lhs.eval(text);
        auto ret_lines = std::make_shared<std::set<line_no>>(left.begin(), left.end());
        return QueryResult(rep(), ret_lines, left.get_file());
    }
};

inline Query operator|(const Query &lhs, const Query &rhs)
{
    return std::shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}