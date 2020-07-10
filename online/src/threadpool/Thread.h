#ifndef __MY_THREAD_H_
#define __MY_THREAD_H_
#include <pthread.h>
#include <iostream>
#include <functional>
#include "Noncopyable.h"
using std::function;

/*
线程类(使用回调函数)：
调用start函数时，创建线程并调用静态threadFunc函数，在threadFunc函数中调用回调函数cb 
*/

namespace wd
{
    using ThreadCallBack = function<void()>;
    class Thread
        : Noncopyable
    {
    public:
        Thread(ThreadCallBack&&);
        ~Thread();
        void start();
        void join();

    private:
        pthread_t _pid;
        bool _isRunning;
        static void *threadFunc(void *arg);
        ThreadCallBack _cb;
    };

} // namespace wd

#endif