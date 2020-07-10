#include "TcpConnection.h"
#include "EventLoop.h"
#include <string.h>
#include <string>
using std::to_string;

namespace wd
{
    TcpConnection::TcpConnection(int fd, EventLoop *pEventLoop)
        : _socket(fd), _sio(fd), _localAddress(getLocal(fd)), _peerAddress(getPeer(fd)), _isShutdownWrite(false), _pEventLoop(pEventLoop)
    {
    }

    TcpConnection::~TcpConnection()
    {
        shutdownWrite();
    }

    string TcpConnection::recv()
    {
        char buf[65535] = {0};
        _sio.readline(buf, sizeof(buf));
        return string(buf);
    }

    void TcpConnection::send(const string &s)
    {
        _sio.writen(s.c_str(), strlen(s.c_str()));
    }

    string TcpConnection::Address() const
    {
        string s;
        s = _localAddress.getIp() + ":" + to_string(_localAddress.getPort()) + " >> " + _peerAddress.getIp() + ":" + to_string(_peerAddress.getPort());
        return s;
    }

    void TcpConnection::shutdownWrite()
    {
        if (!_isShutdownWrite)
        {
            _socket.shutdown_write();
            _isShutdownWrite = true;
        }
    }

    void TcpConnection::sendInEventLoop(const string& msg)
    {
        _pEventLoop->runInEventLoop(std::bind(&TcpConnection::send, this, msg));
    }


    void TcpConnection::setConnectionCallBack(const TcpConnectionCallBack &cb)
    {
        _connectionCB = std::move(cb);
    }

    void TcpConnection::setMessageCallBack(const TcpConnectionCallBack &cb)
    {
        _messageCB = std::move(cb);
    }

    void TcpConnection::setCloseCallBack(const TcpConnectionCallBack &cb)
    {
        _closeCB = std::move(cb);
    }

    void TcpConnection::handleConnectionCallBack()
    {
        if (_connectionCB)
            _connectionCB(shared_from_this());
    }

    void TcpConnection::handleMessageCallBack()
    {
        if (_messageCB)
            _messageCB(shared_from_this());
    }

    void TcpConnection::handleCloseCallBack()
    {
        if (_closeCB)
            _closeCB(shared_from_this());
    }

    InetAddress TcpConnection::getLocal(int fd)
    {
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        socklen_t len = sizeof(sockaddr);
        int ret = getsockname(fd, (sockaddr *)&local, &len);
        if (ret)
        {
            perror("getsockname");
        }
        return InetAddress(local);
    }

    InetAddress TcpConnection::getPeer(int fd)
    {
        struct sockaddr_in peer;
        bzero(&peer, sizeof(peer));
        socklen_t len = sizeof(sockaddr);
        int ret = getpeername(fd, (sockaddr *)&peer, &len);
        if (ret)
        {
            perror("getpeername");
        }
        return InetAddress(peer);
    }
} // namespace wd