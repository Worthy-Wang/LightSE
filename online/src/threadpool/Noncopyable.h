#ifndef __MY_NONCOPYABLE_H_
#define __MY_NONCOPYABLE_H_

/*
不可拷贝基类：继承此基类的派生类将不能使用拷贝构造函数与拷贝赋值运算符
*/

namespace wd
{
    class Noncopyable
    {
    protected:
        Noncopyable() {}
        ~Noncopyable() {}
        Noncopyable(const Noncopyable &lhs) = delete;
        const Noncopyable &operator=(const Noncopyable &lhs) = delete;
    };
} // namespace wd


#endif 