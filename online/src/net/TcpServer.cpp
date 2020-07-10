#include "TcpServer.h"

namespace wd
{
    TcpServer::TcpServer(const string& Ip, unsigned short port)
    :_acceptor(Ip, port)
    ,_eventLoop(_acceptor)
    {
    }

    void TcpServer::start()
    {
        _acceptor.ready();
        _eventLoop.Loop();
    }

    void TcpServer::setConnectionCallBack(const TcpConnectionCallBack &cb)
    {
        _eventLoop.setConnectionCallBack(std::move(cb));
    }

    void TcpServer::setMessageCallBack(const TcpConnectionCallBack &cb)
    {
        _eventLoop.setMessageCallBack(std::move(cb));
    }

    void TcpServer::setCloseCallBack(const TcpConnectionCallBack &cb)
    {
        _eventLoop.setCloseCallBack(std::move(cb));
    }
} // namespace wd