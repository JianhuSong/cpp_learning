#include <iostream>
#if 0
class Base
{
public:
    void pub_men();

protected:
    int prot_mem;

private:
    char priv_mem;
};

struct Pub_Derv : public Base
{
    int f() { return prot_mem; }

    // 错误:private 成员对于派生类来说是不可访问的。
    // 编译也会报错：'char Base::priv_mem' is private within this context
    char g() { return priv_mem; }
};

struct Priv_Derv : private Base
{
    // private 继承不会影响派生类的访问权限
    int f1() { return prot_mem; }
};
#endif

class Base
{
private:
    int priv = 1;

protected:
    int port = 0;

public:
    Base(/* args */) = default;
    ~Base() = default;
    int return_priv() { return priv + port; }
};

class Derv1 : public Base
{
private:
    /* data */
public:
    Derv1(/* args */) = default;
    virtual ~Derv1() = default;
    friend void d1_friend();
    void pub_d_b()
    {
        Derv1 d1;
        Base &b1 = d1;
    }
};

class Derv2 : private Base
{
private:
    /* data */
public:
    Derv2(/* args */) = default;
    ~Derv2() = default;
    friend void d2_friend();
    void priv_d_b()
    {
        Derv2 d2;
        Base &b2 = d2;
    }
};

class Derv3 : protected Base
{
private:
    /* data */
public:
    Derv3(/* args */) = default;
    ~Derv3() = default;
    friend void d3_friend();
    void prot_d_b()
    {
        Derv3 d3;
        Base &b3 = d3;
    }
};

void d1_frined()
{
    Derv1 d1;
    Base &b1 = d1;
}
void d2_friend()
{
    Derv2 d2;
    Base &b2 = d2;
}
void d3_friend()
{
    Derv3 d3;
    Base &b3 = d3;
}

class D1_1_pub: public Derv1
{
private:
    /* data */
public:
    D1_1_pub(/* args */) = default;
    ~D1_1_pub() = default;
    void d_b()
    {
        Derv1 d1;
        Base &b1 = d1; //无报错
    }
};

class D1_1_priv: private Derv1
{
private:
    /* data */
public:
    D1_1_priv(/* args */) = default;
    ~D1_1_priv() = default;
     void d_b()
    {
        Derv1 d1;
        Base &b1 = d1; //无报错
    }
};

class D1_1_prot: protected Derv1
{
private:
    /* data */
public:
    D1_1_prot(/* args */) = default;
    ~D1_1_prot() = default;
    void d_b()
    {
        Derv1 d1;
        Base &b1 = d1;    //无报错
    }
};

class D2_2_pub: public Derv2
{
private:
    /* data */
public:
    D2_2_pub(/* args */) = default;
    ~D2_2_pub() = default;
    void d_b()
    {
        Derv2 d2;
        Base &b2 = d2; //class Base Base::Base' is inaccessible within this context
    }
};

int main(int argc, char const *argv[])
{
    Derv1 D1;
    Derv2 D2;
    Derv3 D3;
    Base &b1 = D1;
    Base &b2 = D2;  //直接报错：Base' is an inaccessible base of 'Derv2'
    Base &b3 = D3;  //'Base' is an inaccessible base of 'Derv3'

    return 0;
}

