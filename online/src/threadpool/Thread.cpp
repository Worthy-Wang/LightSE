#include "Thread.h"

namespace wd
{
    Thread::Thread(ThreadCallBack&& cb)
        : _pid(0), _isRunning(false), _cb(std::move(cb))
    {
        std::cout << "Thread()" << std::endl;
    }

    Thread::~Thread()
    {
        if (_isRunning)
        {
            if (pthread_detach(_pid))
            {
                perror("pthread_detach");
                return;
            }
            _isRunning = false;
            std::cout << "~Thread()" << std::endl;
        }
    }

    void Thread::start()
    {
        if (!_isRunning)
        {
            if (pthread_create(&_pid, NULL, threadFunc, this))
            {
                perror("pthread_create");
                return;
            }
            _isRunning = true;
        }
    }

    void Thread::join()
    {
        if (_isRunning)
        {
            if (pthread_join(_pid, NULL))
            {
                perror("pthread_join");
                return;
            }
            _isRunning = false;
            std::cout << "join()" << std::endl;
        }
    }

    void *Thread::threadFunc(void *arg)
    {
        Thread *p = static_cast<Thread *>(arg);
        p->_cb();
        return NULL;
    }
} // namespace wd