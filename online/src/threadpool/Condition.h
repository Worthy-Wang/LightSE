#ifndef __MY_CONDITION_H_
#define __MY_CONDITION_H_
#include "Noncopyable.h"
#include "MutexLock.h"

namespace wd
{
    class Condition
    : Noncopyable
    {
    public:
        Condition(MutexLock& mutexlock)
        :_mutexlock(mutexlock)
        {
            if (pthread_cond_init(&_cond, NULL))
            {
                perror("pthread_cond_init");
                return;
            }
        }

        ~Condition()
        {
            if (pthread_cond_destroy(&_cond))
            {
                perror("pthread_cond_destroy");
                return;
            }
        }

        void wait()
        {
            if (pthread_cond_wait(&_cond, _mutexlock.getMutexPtr()))
            {
                perror("pthread_cond_wait");
                return;
            }
        }

        void notify()
        {
            if (pthread_cond_signal(&_cond))
            {
                perror("pthread_cond_signal");
                return;
            }
        }

        void notifyAll()
        {
            if (pthread_cond_broadcast(&_cond))
            {
                perror("pthread_cond_broadcast");
                return;
            }
        }

    private:
        pthread_cond_t _cond;
        MutexLock& _mutexlock;
    };
} // namespace wd

#endif