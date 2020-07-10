#include "Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

namespace wd
{
    Socket::Socket()
    {
        _fd = socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == _fd)
        {
            perror("socket");
        }
    }

    Socket::Socket(int fd)
    :_fd(fd)
    {
    }

    Socket::~Socket()
    {
        close(_fd);
    }

    int Socket::getFd() const
    {
        return _fd;
    }

    void Socket::shutdown_write()
    {
        shutdown(_fd, SHUT_WR);
    }


} // namespace wd