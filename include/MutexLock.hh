/***********************************************************
  > File Name: MutexLock.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年08月31日 星期三 20时38分19秒
  > Modified Time:2022年08月31日 星期三 20时38分19秒
 *******************************************************/
#ifndef __MUTEXLOCK_HH__
#define __MUTEXLOCK_HH__

#include "NonCopyable.hh"
#include <pthread.h>
#include <stdio.h>

class MutexLock: public NonCopyable
{
public:
    MutexLock()
    {
        if(pthread_mutex_init(&_mutex, NULL) != 0){
            perror("pthread_mutex_init");
        }
    }

    ~MutexLock()
    {
        if(pthread_mutex_destroy(&_mutex) != 0){
            perror("pthread_mutex_destory");
        }       
    }

	void lock(void)
    {
        pthread_mutex_lock(&_mutex);

    }

	void unlock(void)
    {
        pthread_mutex_unlock(&_mutex);
    }
    
    pthread_mutex_t & getMutex(void)
    {
        return _mutex;
    }

private:
    pthread_mutex_t _mutex;
};

/* #include <iostream> */

class MutexLockGuard: public NonCopyable
{
public:
    //禁止调用MutexLockGrand临时对象，临时对象不能守护临界区
    explicit
    MutexLockGuard(MutexLock & mutex)
        : _mutex(mutex)
    {
        _mutex.lock();
    }

    ~MutexLockGuard()
    {
        _mutex.unlock();
    }

private:
    MutexLock & _mutex;
};

#endif
