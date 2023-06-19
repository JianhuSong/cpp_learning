#include <iostream>
#include <string>
#include <map>
enum ERROR_CODE
{
    SUCCESS,
    OUT_OF_RANGE,
    POSITION_INVALID,
    IS_EXIST,
    IS_NOT_EXIST,
    CODE_ERROR,
};

std::map<int, std::string> myErrCode =
    {
        {ERROR_CODE::SUCCESS, std::string("SUCCESS")},
        {ERROR_CODE::OUT_OF_RANGE, std::string("OUT_OF_RANGE")},
        {ERROR_CODE::POSITION_INVALID, std::string("POSITION_INVALID")},
        {ERROR_CODE::IS_EXIST, std::string("IS_EXIST")},
        {ERROR_CODE::IS_NOT_EXIST, std::string("IS_NOT_EXIST")},
        {ERROR_CODE::CODE_ERROR, std::string("CODE_ERROR")},
};

struct node
{
    int data = 0;
    struct node *next = NULL;
};

class myLinkList
{
public:
    myLinkList() : length(0), head() {}
    ~myLinkList() = default;
    int findPos(int pos)
    {
        node *p = &head;
        int j = 1;
        while (p && j < pos)
        {
            p = p->next;
            j++;
        }

        if (!p || j > pos)
        {
            return ERROR_CODE::OUT_OF_RANGE;
        }

        return p->data;
    }

    int findValue(int value)
    {
        node *p = &head;
        while (p)
        {
            if (p->data == value)
            {
                break;
            }
            p = p->next;
        }

        if (!p)
        {
            return ERROR_CODE::IS_NOT_EXIST;
        }
        return ERROR_CODE::IS_EXIST;
    }

    int insert(int i, int value)
    {
        node *p = &head;
        int j = 1;
        while (p && j < i)
        {
            p = p->next;
            j++;
        }

        if (!p || j > i)
        {
            return ERROR_CODE::OUT_OF_RANGE;
        }

        node *newnode = new node();
        newnode->data = value;

        newnode->next = p->next;
        p->next = newnode;
        length++;
        return ERROR_CODE::SUCCESS;
    }

    int delNode(int i)
    {
        node *p = &head, *q = p;
        int j = 1;
        while (p && j < i)
        {
            p = p->next;
            j++;
        }

        if (!p || j > i)
        {
            return ERROR_CODE::OUT_OF_RANGE;
        }

        q = p->next;
        p->next = q->next;
        delete q;
        length--;
        return ERROR_CODE::SUCCESS;
    }

    int clear()
    {
        node *p = head.next, *q = p;
        while (p)
        {
            q = p->next;
            delete p;
            p = q;
        }

        head.next = NULL;
        length = 0;
        return ERROR_CODE::SUCCESS;
    }

    int getLength() { return length; }
    int createLinkList(int num)
    {
        for (int i = 0; i < num; i++)
        {
            node *newnode = new node();
            newnode->data = i * 1000 + i * 100 + i * 10 + i;
            newnode->next = head.next;
            head.next = newnode;
            length++;
        }

        return ERROR_CODE::SUCCESS;
    }

    void printLinkList()
    {
        node *p = head.next;
        int i = 1;
        while (p)
        {
            std::cout << "The value of the " << i << "/" << length << " node is " << p->data << "." << std::endl;
            p = p->next;
            i++;
        }
    }

private:
    int length;
    node head;
};

int main(int argc, char const *argv[])
{
    myLinkList mlist;
    std::cout << "Before size = " << mlist.getLength() << std::endl;
    mlist.createLinkList(20);
    mlist.printLinkList();
    std::cout << "After size = " << mlist.getLength() << std::endl;

    for (int i = 0; i < 10; i++)
    {
        if (i % 2 == 0)
        {
            std::cout << "Insert " << i * 10 + 1
                      << " into the position of node " << i + 1 << "."
                      << myErrCode.at(mlist.insert(i + 1, i * 10 + 1)) << std::endl;
        }
    }

    std::cout << "After size = " << mlist.getLength() << std::endl;
    mlist.printLinkList();

    for (int i = 0; i < 5; i++)
    {
        std::cout << "del the em of pos " << i + 1
                  << " value[" << mlist.findPos(i + 1)
                  << "] : " << myErrCode.at(mlist.delNode(i + 1)) << std::endl;
        mlist.printLinkList();
    }

    std::cout << "After size = " << mlist.getLength() << std::endl;

    // std::cout << "find the value[110] : " << myErrCode.at(mlist.find(110)) << std::endl;
    return 0;
}
