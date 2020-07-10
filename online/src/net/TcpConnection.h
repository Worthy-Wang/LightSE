#ifndef __MY_TCPCONNECTION_H_
#define __MY_TCPCONNECTION_H_
#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h" 
#include "Noncopyable.h"
#include <string>
#include <functional>
#include <memory>
using std::shared_ptr;
using std::string;
using std::function;

namespace wd
{
    class EventLoop;
    class TcpConnection;
    using TcpConnectionPtr = shared_ptr<TcpConnection>;
    using TcpConnectionCallBack = function<void(const TcpConnectionPtr&)>;

    class TcpConnection
    :Noncopyable
    ,public std::enable_shared_from_this<TcpConnection>
    {
    public:
        TcpConnection(int fd, EventLoop*);
        ~TcpConnection();
        string recv();
        void send(const string& s);
        void shutdownWrite();
        string Address() const;
        void sendInEventLoop(const string& msg); //将消息重新发回EventLoop中，让IO线程发送 


        void setConnectionCallBack(const TcpConnectionCallBack&);
        void setMessageCallBack(const TcpConnectionCallBack&);
        void setCloseCallBack(const TcpConnectionCallBack&);
        
        void handleConnectionCallBack();
        void handleMessageCallBack();
        void handleCloseCallBack();

    private:
        InetAddress getLocal(int);
        InetAddress getPeer(int);

    private:
        Socket _socket;
        SocketIO _sio;
        InetAddress _localAddress;
        InetAddress _peerAddress;
        bool _isShutdownWrite;
        EventLoop* _pEventLoop;
 
        TcpConnectionCallBack _connectionCB;
        TcpConnectionCallBack _messageCB;
        TcpConnectionCallBack _closeCB;
    };
}

#endif