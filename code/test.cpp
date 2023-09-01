#include <iostream>
#include <string>
class CompanyA
{
public:
    CompanyA() {}
    ~CompanyA() = default;
    void sendClearText(const std::string &msg)
    {
        std::cout << "company A sendEncrypted msg = " + msg << std::endl;
    }

    void sendEncrypted(const std::string &msg)
    {
        std::cout << "company A sendEncrypted msg = " + msg << std::endl;
    }
};

class CompanyB
{
public:
    CompanyB() {}
    ~CompanyB() = default;
    void sendClearText(const std::string &msg)
    {
        std::cout << "company B sendEncrypted msg = " + msg << std::endl;
    }

    void sendEncrypted(const std::string &msg)
    {
        std::cout << "company B sendEncrypted msg = " + msg << std::endl;
    }
};

class MsgInfo
{
public:
    MsgInfo(const std::string &inmsg) : msg(inmsg) {}
    ~MsgInfo() = default;
    std::string getMsg() const
    {
        return msg;
    }

private:
    std::string msg;
};

template <typename Company>
class MsgSender
{
public:
    MsgSender() {}
    ~MsgSender() = default;
    void sendClear(const MsgInfo &info)
    {
        std::string msg = info.getMsg();
        Company c;
        c.sendClearText(msg);
    }

    void sendSecret(const MsgInfo &info)
    {
        std::string msg = info.getMsg();
        Company c;
        c.sendEncrypted(msg);
    }
};

// 以template为基类定义子类
template <typename Company>
class LoggingMsgSender : public MsgSender<Company>
{
public:
    LoggingMsgSender() {}
    ~LoggingMsgSender() = default;
    void sendClearMsg(const MsgInfo &info)
    {
        std::cout << "adasd" << std::endl;
        // g++报错： there are no arguments to 'sendClear' that depend on a template parameter,
        // so a declaration of 'sendClear' must be available
        // 同时给出了解决方案：使用编译参数：“-fpermissive”,使用该参数后从报错变成了告警。
        sendClear(info);
        std::cout << "adasdfffff" << std::endl;
    }
};

class CompanyZ
{
public:
    CompanyZ() {}
    ~CompanyZ() = default;

    void sendEncrypted(const std::string &msg)
    {
        std::cout << "company Z sendEncrypted msg = " + msg << std::endl;
    }
};

// template<>这个不是template也不是标准class,而是个特化版的MsgSender template,在template实参是CompanyZ时候使用。
// 这就是模板全特化
template <>
class MsgSender<CompanyZ>
{
public:
    MsgSender() {}
    ~MsgSender() = default;
    void sendSecret(const MsgInfo &info)
    {
        std::string msg = info.getMsg();
        CompanyZ c;
        c.sendEncrypted(msg);
    }
};

int main(int argc, char const *argv[])
{
    LoggingMsgSender<CompanyA> mca;
    mca.sendClear(std::string("hello"));
    mca.sendSecret(std::string("hello"));

    LoggingMsgSender<CompanyB> mcb;
    mcb.sendClear(std::string("hello"));
    mcb.sendSecret(std::string("hello"));

    // 模板全特化：针对某一个类型的全面特化。
    LoggingMsgSender<CompanyZ> mcz;
    mcz.sendClear(std::string("hello"));
    mcz.sendSecret(std::string("world"));
    return 0;
}
