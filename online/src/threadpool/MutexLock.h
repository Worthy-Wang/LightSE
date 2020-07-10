#ifndef __MY_MUTEXLOCK_H_
#define __MY_MUTEXLOCK_H_

#include <pthread.h>
#include <stdio.h>
#include "Noncopyable.h"
/*
封装mutex类，并创建类MutexLockGuard用来实现锁（MutexLock类加解锁不再对外开放），这样可以有效防止死锁
*/

namespace wd
{
    class MutexLock
        : Noncopyable
    {
    public:
        MutexLock()
            : _isLocking(false)
        {
            if (pthread_mutex_init(&_mutex, NULL))
            {
                perror("pthread_mutex_init");
                return;
            }
        }

        ~MutexLock()
        {
            if (pthread_mutex_destroy(&_mutex))
            {
                perror("pthread_mutex_destroy");
                return;
            }
        }

        void lock()
        {
            if (pthread_mutex_lock(&_mutex))
            {
                perror("pthread_mutex_lock");
                return;
            }
            _isLocking = true;
        }

        void unlock()
        {
            if (pthread_mutex_unlock(&_mutex))
            {
                perror("pthread_mutex_unlock");
                return;
            }
            _isLocking = false;
        }

        bool isLocking() const { return _isLocking; }
        pthread_mutex_t *getMutexPtr() { return &_mutex; }

    private:
        pthread_mutex_t _mutex;
        bool _isLocking;
    };

    class MutexLockGuard
    {
    public:
        MutexLockGuard(MutexLock &mutexlock)
            : _mutexlock(mutexlock)
        {
            _mutexlock.lock();
        }

        ~MutexLockGuard()
        {
            _mutexlock.unlock();
        }

    private:
        MutexLock &_mutexlock;
    };
} // namespace wd

#endif