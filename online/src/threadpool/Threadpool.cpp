#include "Threadpool.h"

namespace wd
{
    Threadpool::Threadpool(size_t threadNum, size_t queSize)
        : _threadNum(threadNum), _queSize(queSize), _threads(), _taskQue(), _flag(false)
    {
    }

    Threadpool::~Threadpool()
    {
    }

    void Threadpool::start()
    {
        if (!_flag)
        {
            _flag = true;
            for (size_t i = 0; i < _threadNum; i++)
            {
                std::unique_ptr<Thread> pth(new Thread(std::bind(&Threadpool::threadFunc, this)));
                _threads.push_back(std::move(pth));
            }
            for (size_t i = 0; i < _threadNum; i++)
            {
                _threads[i]->start();
            }
        }
    }

    void Threadpool::stop()
    {
        if (_flag)
        {
            _flag = false;
            while (!_taskQue.empty())
            {
                std::cout << "请等待！任务队列中还有任务没有完成!" << std::endl;
                sleep(1);
            }

            _taskQue.wakeup();

            for (size_t i = 0; i < _threadNum; i++)
            {
                _threads[i]->join();
            }
        }
    }

    void Threadpool::addTask(const ElemType &e)
    {
        _taskQue.push(e);
    }

    void Threadpool::threadFunc()
    {
        while (_flag)
        {
            ElemType task = _taskQue.pop();
            if (task)
            {
                task();
            }
        }
    }
} // namespace wd