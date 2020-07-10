#include "Acceptor.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>

namespace wd
{
    Acceptor::Acceptor(const string& Ip, unsigned short port)
        : _socket(), _addr(Ip, port)
    {
    }

    Acceptor::Acceptor(unsigned short port)
        : _socket(), _addr(port)
    {
    }

    Acceptor::Acceptor(const sockaddr_in ser)
        : _socket(), _addr(ser)
    {
    }

    void Acceptor::ready()
    {
        //设置端口为可重新绑定
        reusePort(true);
        //设置地址为可重绑定，同一时间多个客户端可绑定该服务器，服务器负载能力增强
        reuseAddr(true);
        bind();
        listen(10);
    }

    int Acceptor::accept()
    {
        sockaddr_in client;
        bzero(&client, sizeof(client));
        socklen_t len = sizeof(sockaddr);
        int ret = ::accept(_socket.getFd(), (sockaddr *)&client, &len);
        if (-1 == ret)
        {
            perror("accept");
        }
        return ret;
    }

    int Acceptor::fd()
    {
        return _socket.getFd();
    }

    void Acceptor::reuseAddr(bool flag)
    {
        int on = flag;
        int ret = setsockopt(_socket.getFd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        if (-1 == ret)
        {
            perror("setsockopt reuseAddr");
        }
    }

    void Acceptor::reusePort(bool flag)
    {
        int on = flag;
        int ret = setsockopt(_socket.getFd(), SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
        if (-1 == ret)
        {
            perror("setsockopt reusePort");
        }
    }

    void Acceptor::bind()
    {
        int ret = ::bind(_socket.getFd(), (sockaddr *)_addr.getAddressPtr(), sizeof(sockaddr));
        if (-1 == ret)
        {
            perror("bind");
        }
    }

    void Acceptor::listen(int listensize)
    {
        int ret = ::listen(_socket.getFd(), listensize);
        if (-1 == ret)
        {
            perror("listen");
        }
    }

} // namespace wd