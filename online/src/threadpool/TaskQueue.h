#ifndef __MY_TASKQUEUE_H_
#define __MY_TASKQUEUE_H_
#include "Condition.h"
#include "MutexLock.h"
#include <queue>
#include <functional>
using std::function;

namespace wd
{
    using Task = function<void()>; //任务是需要执行的函数
    using ElemType = Task;
    class TaskQueue
    {
    public:
        TaskQueue(size_t queSize = 10);
        ~TaskQueue();
        bool full() const;
        bool empty() const;
        void push(const ElemType&);
        ElemType pop();
        void wakeup(); // 唤醒所有等待任务的线程,让它们退出

    private:
        std::queue<ElemType> _que;
        size_t _queSize;
        MutexLock _mutexlock;
        Condition _fullAndWait;
        Condition _emptyAndWait;
        bool _flag;
    };


} // namespace wd

#endif