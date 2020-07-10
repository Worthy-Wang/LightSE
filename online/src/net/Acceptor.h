#ifndef __MY_BUFFER_H_
#define __MY_BUFFER_H_
#include "Socket.h"
#include "InetAddress.h"

namespace wd
{
    class Acceptor
    {
    public:
        Acceptor(const string& Ip, unsigned short port);
        Acceptor(unsigned short port);
        Acceptor(const sockaddr_in ser);
    
        void ready();//设置socket，bind，listen一气呵成
        int accept(); // 返回接收到的fd
        int fd();
    private:
        void reuseAddr(bool);
        void reusePort(bool);
        void bind();
        void listen(int);
        
    private:
        Socket _socket;
        InetAddress _addr;
    };
} // namespace wd


#endif