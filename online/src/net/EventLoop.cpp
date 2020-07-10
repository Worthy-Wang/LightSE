#include "EventLoop.h"
#include <errno.h>
#include <sys/eventfd.h>
#include <unistd.h>

namespace wd
{
    EventLoop::EventLoop(Acceptor &acceptor)
        : _epfd(createEpollFd()), _eventfd(createEventFd()), _acceptor(acceptor), _evs(1024), _functors(), _mutexlock(), _tcpMap(), _isLoop(false)
    {
        epollAdd(_acceptor.fd());
        epollAdd(_eventfd);
    }

    void EventLoop::Loop()
    {
        if (_isLoop)
            return;
        else
        {
            _isLoop = true;
            epollWait();
        }
    }

    void EventLoop::UnLoop()
    {
        if (_isLoop)
        {
            _isLoop = false;
        }
    }

    void EventLoop::runInEventLoop(Functor &&functor)
    {
        {
            MutexLockGuard mlg(_mutexlock);
            _functors.push_back(std::move(functor));
        }
        wakeup();
    }

    void EventLoop::setConnectionCallBack(const TcpConnectionCallBack &cb)
    {
        _connectionCB = std::move(cb);
    }

    void EventLoop::setMessageCallBack(const TcpConnectionCallBack &cb)
    {
        _messageCB = std::move(cb);
    }

    void EventLoop::setCloseCallBack(const TcpConnectionCallBack &cb)
    {
        _closeCB = std::move(cb);
    }

    void EventLoop::epollWait()
    {
        while (_isLoop)
        {
            int nready = epoll_wait(_epfd, &*_evs.begin(), _evs.size(), 10000);
            if (-1 == nready && errno == EINTR)
            {
                continue;
            }
            else if (-1 == nready)
            {
                perror("epoll_wait");
            }
            else if (0 == nready)
            {
                // cout << "TimeOut!" << endl;
            }
            else if ((int)_evs.size() == nready)
            {
                _evs.resize(_evs.size() * 2); //扩容
            }
            else
            {
                for (int i = 0; i < nready; i++)
                {
                    int fd = _evs[i].data.fd;
                    //有新的连接
                    if (_acceptor.fd() == fd)
                    {
                        addNewConnection(_acceptor.accept());
                    }
                    //子线程(计算线程)发来IO请求
                    else if (fd == _eventfd)
                    {
                        handleRead();//读取数据，防止_eventfd一直响应
                        doPendingFunctors();
                    }
                    //为客户端发来的处理信息
                    else
                    {
                        handleMessage(fd);
                    }
                }
            }
        }
    }

    void EventLoop::addNewConnection(int fd)
    {
        epollAdd(fd);
        TcpConnectionPtr p(new TcpConnection(fd, this));
        p->setConnectionCallBack(_connectionCB);
        p->setMessageCallBack(_messageCB);
        p->setCloseCallBack(_closeCB);
        _tcpMap.insert(std::make_pair(fd, p));
        p->handleConnectionCallBack(); // 调用回调函数，新连接
    }

    void EventLoop::handleMessage(int fd)
    {
        TcpConnectionPtr p = _tcpMap.at(fd);
        if (!p.get())
        {
            perror("TcpConnectionPtr ERROR");
        }
        //接收到客户端的信息有两种可能，一是真正的有用的信息，二是客户端退出接收到的0
        if (isClientQuit(fd))
        {
            p->handleCloseCallBack();
            epollDel(fd);
            _tcpMap.erase(fd);
        }
        else
        {
            p->handleMessageCallBack(); // 调用处理信息的回调函数
        }
    }

    int EventLoop::createEpollFd()
    {
        int epfd = epoll_create(1);
        if (-1 == epfd)
        {
            perror("epoll_create");
        }
        return epfd;
    }

    int EventLoop::createEventFd()
    {
        int ret = eventfd(0, 0);
        if (-1 == ret)
        {
            perror("eventfd");
        }
        return ret;
    }

    void EventLoop::epollDel(int fd)
    {
        epoll_event ev;
        ev.data.fd = fd;
        ev.events = EPOLLIN;
        int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &ev);
        if (-1 == ret)
        {
            perror("epoll_ctl_del");
        }
    }

    void EventLoop::epollAdd(int fd)
    {
        epoll_event ev;
        ev.data.fd = fd;
        ev.events = EPOLLIN;
        int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
        if (-1 == ret)
        {
            perror("epoll_ctl_add");
        }
    }

    bool EventLoop::isClientQuit(int fd)
    {
        int ret;
        do
        {
            char buf[1000];
            ret = recv(fd, buf, sizeof(buf), MSG_PEEK);
        } while (-1 == ret && errno == EINTR);
        return (0 == ret);
    }

    void EventLoop::wakeup()
    {
        uint64_t event_write = 1;
        if (write(_eventfd, &event_write, sizeof(uint64_t)) != sizeof(uint64_t))
        {
            perror("write eventfd");
        }
    }

    void EventLoop::handleRead()
    {
        uint64_t event_read;
        if (read(_eventfd, &event_read, sizeof(uint64_t)) != sizeof(uint64_t))
        {
            perror("read evfd");
        }
    }

    void EventLoop::doPendingFunctors()
    {
        vector<Functor> temp;
        {
            MutexLockGuard mlg(_mutexlock);
            std::swap(temp, _functors);
        }
        for (auto& e: temp)
        {
            e();
        }
    }

} // namespace wd