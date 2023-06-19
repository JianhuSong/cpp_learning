#include <iostream>
#include <map>
#include <string>

typedef int status;

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

class MyList
{
public:
#define MAX_SIZE 20
    MyList()
    {
    }
    ~MyList() = default;
    status clear()
    {
        length = 0;
        for (int i = 0; i < MAX_SIZE; i++)
        {
            data[i] = 0;
        }
        return ERROR_CODE::SUCCESS;
    }

    status insert(int pos, int value)
    {
        if (length == MAX_SIZE)
        {
            return ERROR_CODE::OUT_OF_RANGE;
        }

        if (pos < 1 || pos > length + 1)
        {
            return ERROR_CODE::POSITION_INVALID;
        }

        if (pos <= length)
        {
            for (int k = length - 1; k >= pos - 1; k--)
            {
                data[k + 1] = data[k];
            }
        }

        data[pos - 1] = value;
        length++;
        return ERROR_CODE::SUCCESS;
    }

    int get(int pos)
    {
        if (pos >= MAX_SIZE || pos < 1)
        {
            return ERROR_CODE::OUT_OF_RANGE;
        }

        return data[pos - 1];
    }

    status isExsit(int value)
    {
        for (int i = 1; i <= length; i++)
        {
            if (data[i - 1] == value)
            {
                return ERROR_CODE::IS_EXIST;
            }
        }
        return ERROR_CODE::IS_NOT_EXIST;
    }
    int getLength() { return length; }
    int getSize() { return length * sizeof(int); }

private:
    int data[MAX_SIZE] = {0};
    int length = 0;
};

int main(int argc, char const *argv[])
{
    MyList mylist;
    for (int i = 0; i < 15; i++)
    {
        int pos = i + 1, value = i * 10 + 2000;
        std::cout << "mylist insert value[" << value << "] to pos[" << pos << "] " << myErrCode.at(mylist.insert(pos, value)) << "." << std::endl;
    }

    std::cout << "mylist length: " << mylist.getLength() << std::endl;
    std::cout << "mylist size: " << mylist.getSize() << std::endl;
    std::cout << "mylist pos 12's value: " << mylist.get(12) << std::endl;
    std::cout << "mylist insert[10000] to pos[166]: " << myErrCode.at(mylist.insert(166, 10000)) << std::endl;
    std::cout << "mylist pos 12's value: " << mylist.get(12) << std::endl;
    return 0;
}
