#include "InetAddress.h"
#include <string.h>

namespace wd
{
    InetAddress::InetAddress(const string& Ip, unsigned short Port)
    {
        bzero(&_ser, sizeof(_ser));
        _ser.sin_addr.s_addr = inet_addr(Ip.c_str());
        _ser.sin_port = htons(Port);
        _ser.sin_family = AF_INET;
    }

    InetAddress::InetAddress(unsigned short Port)
    {
        bzero(&_ser, sizeof(_ser));
        _ser.sin_addr.s_addr = inet_addr("0.0.0.0");//localhost
        _ser.sin_port = htons(Port);
        _ser.sin_family = AF_INET;
    }

    InetAddress::InetAddress(const sockaddr_in& ser)
    {
        _ser = ser;
    }

    sockaddr_in* InetAddress::getAddressPtr() 
    {
        return &_ser;
    }

    string InetAddress::getIp() const
    {
        return string(inet_ntoa(_ser.sin_addr));
    }

    unsigned short InetAddress::getPort() const
    {
        return ntohs(_ser.sin_port);
    }

} // namespace wd