#include "SocketIO.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

namespace wd
{
    SocketIO::SocketIO(int sockfd)
        : _socket(sockfd)
    {
    }

    SocketIO::~SocketIO()
    {
        _socket.shutdown_write();
    }

    int SocketIO::readn(void *buf, int len)
    {
        int left = len;
        char *p = (char *)buf;
        while (left > 0)
        {
            int ret = ::read(_socket.getFd(), p, left);
            if (ret == -1 && errno == EINTR)
                continue;
            else if (ret == -1)
            {
                perror("read");
                return len - left;
            }
            else if (ret == 0)
            {
                return len - left;
            }
            else
            {
                left -= ret;
                p += ret;
            }
        }
        return len - left;
    }

    int SocketIO::writen(const char *buf, int len)
    {
        int left = len;
        char *p = (char *)buf;
        while (left > 0)
        {
            int ret = ::write(_socket.getFd(), p, left);
            if (ret == -1 && errno == EINTR)
                continue;
            else if (ret == -1)
            {
                perror("write");
                return len - left;
            }
            else if (ret == 0)
            {
                return len - left;
            }
            else
            {
                left -= ret;
                p += ret;
            }
        }
        return len - left;
    }

    //每一次读取一行数据
    int SocketIO::readline(void *buff, int maxlen)
    {
        int left = maxlen - 1;
        char *p = (char *)buff;
        int ret = 0;
        int total = 0;
        {
            ret = recvPeek(p, left);
            //查找 '\n'
            for (int idx = 0; idx != ret; ++idx)
            {
                if (p[idx] == '\n')
                {
                    int sz = idx + 1;
                    readn(p, sz);
                    total += sz;
                    p += sz;
                    *p = '\0';
                    return total;
                }
            }
            //如果没有发现 '\n'
            readn(p, ret);
            left -= ret;
            p += ret;
            total += ret;
        }
        *p = '\0'; // 最终没有发现'\n'
        return total;
    }

    int SocketIO::recvPeek(char *buf, int len) //将数据从内核态中复制出来（注意MSG_PEEK代表不删除内核中复制的那部分）
    {
        int ret;
        do
        {
            ret = recv(_socket.getFd(), buf, len, MSG_PEEK);
        } while (-1 == ret && errno == EINTR); //读取多少算多少，但如果遇到系统中断则必须重新读
        if (-1 == ret)
        {
            perror("recv MSG_PEEK");
        }
        return ret;
    }

} // namespace wd