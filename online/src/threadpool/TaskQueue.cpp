#include "TaskQueue.h"
namespace wd
{
    TaskQueue::TaskQueue(size_t queSize)
        : _que(), _queSize(queSize), _mutexlock(), _fullAndWait(_mutexlock), _emptyAndWait(_mutexlock), _flag(true)
    {
    }

    TaskQueue::~TaskQueue()
    {
    }

    void TaskQueue::push(const ElemType &e)
    {
        MutexLockGuard autolock(_mutexlock);
        while (full())
        {
            _fullAndWait.wait();
        }
        _que.push(e);
        _emptyAndWait.notify();
    }

    ElemType TaskQueue::pop()
    {
        ElemType ret;
        MutexLockGuard autolock(_mutexlock);
        while (_flag && empty())
        {
            _emptyAndWait.wait();
        }
        if (_flag)
        {
            ret = _que.front();
            _que.pop();
            _fullAndWait.notify();
            return ret;
        }
        else
            return nullptr;
    }

    void TaskQueue::wakeup()
    {
        if (_flag)
            _flag = false;
        _emptyAndWait.notifyAll();
    }

    bool TaskQueue::full() const { return _que.size() == _queSize; }

    bool TaskQueue::empty() const { return _que.empty(); }
} // namespace wd