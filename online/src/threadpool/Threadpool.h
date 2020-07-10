#ifndef __MY_THREADPOOL_H_
#define __MY_THREADPOOL_H_
#include "TaskQueue.h"
#include "Thread.h"
#include <vector>
#include <memory>
#include <iostream>
#include <unistd.h>

namespace wd
{
    class Threadpool
    {
    public:
        Threadpool(size_t, size_t); //(子线程数，任务队列容量)
        ~Threadpool();
        void start();
        void stop();
        void addTask(const ElemType&);
    private:
        void threadFunc(); //子线程的执行函数，等待并获取任务
        void createCache(); //每个线程创建属于自己的Cache

    private:
        size_t _threadNum;                             // 子线程数
        size_t _queSize;                               // 任务队列容量
        std::vector<std::unique_ptr<Thread>> _threads; // 子线程vector
        TaskQueue _taskQue;                            //任务队列
        bool _flag;                                    //线程池运行标志
    };
} // namespace wd

#endif