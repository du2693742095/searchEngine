/***********************************************************
  > File Name: Condition.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年08月31日 星期三 21时20分00秒
  > Modified Time:2022年08月31日 星期三 21时20分00秒
 *******************************************************/
#ifndef __CONDITION_HH__
#define __CONDITION_HH__

#include "MutexLock.hh"

class Condition: NonCopyable
{
public:
    Condition(MutexLock & mutex)
        :_mutex(mutex)
    {
        if(pthread_cond_init(&_cond, NULL) != 0)
        {
            perror("pthread_cond_init");
        }
    }

    ~Condition()
    {
        if(pthread_cond_destroy(&_cond) != 0){
            perror("pthread_cond_destroy");
        }
    }

	void wait()
    {
        pthread_cond_wait(&_cond, &_mutex.getMutex());
    }

	void notify()
    {
        pthread_cond_signal(&_cond);
    }

	void notifyall()
    {
        pthread_cond_broadcast(&_cond);
    }

 private:
    pthread_cond_t _cond;
    MutexLock & _mutex;
};


#endif
