#include <iostream>
#include <string>
#include <set>
#include <list>
#include <vector>
#include <iterator>

class Message;

class Folder
{
public:
    void remMsg(Message *m);
    void addMsg(Message *m)
    {
        std::cout << "folder addMsg" << std::endl;
        msgset.push_back(*m);
    }

private:
    std::vector<Message> msgset;
};



void Folder::remMsg(Message *m)
{
    std::cout << "Folder remMsg" << std::endl;
    using tit = std::vector<Message>::iterator;
    std::vector<tit> indexs;
    for (auto i = msgset.begin(); i != msgset.end(); ++i)
    {
        if (i->contents == m->contents)
        {
            std::cout << "find the " << m->contents << " ,at index " << std::distance(i, msgset.begin()) << std::endl;
            indexs.push_back(i);
        }
    }

    for (tit it : indexs)
    {
        msgset.erase(it);
    }
}

void swap(Message &lhs, Message &rhs)
{
    using std::swap;
    for (auto f : lhs.folders)
        f->remMsg(&lhs);
    for (auto f : rhs.folders)
        f->remMsg(&rhs);

    swap(lhs.folders, rhs.folders);
    swap(lhs.contents, rhs.contents);
    for (auto f : lhs.folders)
        f->addMsg(&lhs);
    for (auto f : rhs.folders)
        f->addMsg(&rhs);
}

int main(int argc, char const *argv[])
{
    Message m1(std::string("hello")), m2(std::string("word")), m3, m4(m2);
    m3 = m1;
    m3(m2);
    return 0;
}
