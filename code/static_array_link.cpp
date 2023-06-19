/*
    这个文件是专门用来练习静态链表的
    1.链表的长度是固定的
    2.链表的第一个结点和最后一个结点比较特殊
*/
#include <iostream>

#define MAXSIZE 1000

using CODE = enum uint32_t{
    SUCCESS,
    FAILED,
    POS_INVALID,
    CODE_DEF,
};

typedef struct ArrLink
{
    int cur = 0;
    int data = 0;
} ARRLINK;

class MyStaticLink
{
public:
    MyStaticLink():_length(0){}
    ~MyStaticLink() = default;

    int getUsedSize()
    {
        return _length;
    }

    int getUnusedSize()
    {
        return MAXSIZE - 2 - _length;
    }

    int initStaticLink()
    {
        _arr_link[0].cur = 1;
        _arr_link[MAXSIZE- 1].cur = 0;

        return CODE::SUCCESS;
    }

    int insert(int pos, int value)
    {
        if(pos < 1 || pos >= MAXSIZE)
        {
            return CODE::POS_INVALID;
        }

        int index = _arr_link[0].cur;
        if(!_length)
        {
            _arr_link[MAXSIZE - 1].cur = pos;
        }

        _arr_link[pos].data = value;
        _arr_link[pos].cur = _arr_link[index - 1].cur;
        _arr_link[index - 1].cur = pos;
        _length++;
        _arr_link[0].cur = pos + 1;

        if(_arr_link[0].cur )
        return CODE::SUCCESS;
    }

    int del(int pos)
    {
       if(pos < 1 || pos >= MAXSIZE)
        {
            return CODE::POS_INVALID;
        }

        if(_arr_link[0].cur == pos)
        {
            /*该位置没有赋值*/
            return CODE::SUCCESS;
        }

        int index = _arr_link[MAXSIZE -1].cur;  /*第一个有值元素的位置*/
        
        /*找到cur是pos的那个结点*/
        int whoispos = 0;
        for(int i = 1; i < MAXSIZE; i++)
        {
            if(_arr_link[i].cur == pos)
            {
                whoispos = i;
            }
        }

        /* 没有找到指向pos这个位置的结点，说明没有这个位置没有值*/
        if(!whoispos)
        {

        }
    }



private:
    int _length = 0;
    ARRLINK _arr_link[MAXSIZE] = {{0,0}};
};

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
