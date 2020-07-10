#ifndef __MY_INETADDRESS_H_
#define __MY_INETADDRESS_H_
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
using std::string;

namespace wd
{
    class InetAddress
    {
    public:
        InetAddress(const string& Ip, unsigned short Port);
        InetAddress(unsigned short Port);
        InetAddress(const sockaddr_in& ser);
        sockaddr_in* getAddressPtr();
        string getIp() const;
        unsigned short getPort() const;
    private:
        sockaddr_in _ser;
    };
} // namespace wd


#endif