/***********************************************************
  > File Name: TaskQueue.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月01日 星期四 09时13分49秒
  > Modified Time:2022年09月01日 星期四 09时13分49秒
 *******************************************************/
#ifndef __TASKQUEUE_HH__
#define __TASKQUEUE_HH__

#include "Condition.hh"
#include <queue>
#include <memory>

template <typename T>
class TaskQueue
{
public:
    TaskQueue(size_t maxSize)
        : _maxSize(maxSize)
        , _mutex()
        , _isEmpty(_mutex)
        , _isFull(_mutex)
        , _exitFlag(false)
    {}

    ~TaskQueue(){}

    TaskQueue(const TaskQueue &) = delete ;
    TaskQueue & operator=(const TaskQueue &) = delete ;

    bool isEmpty() const;
    bool isFull() const;

    T & peek(void);
    void inQueue(const T & data);
    void inQueue(const T && data);
    T deQueue();

    void wakeUp(); 

private:
    size_t _maxSize;   //队列的最大长度
    MutexLock _mutex;   
    Condition _isEmpty;
    Condition _isFull;
    std::queue<T> _data;
    bool _exitFlag;     //用来让线程池退出
};


template <typename T>
bool TaskQueue<T>::isEmpty() const
{
    return _data.size() == 0;
}

template <typename T>
bool TaskQueue<T>::isFull() const
{
    return _data.size() == _maxSize;
}

template <typename T>
T & TaskQueue<T>::peek(void)
{
    MutexLockGuard lock(_mutex);
    while(isEmpty()){
        _isEmpty.wait();
    }
    return _data.front();
}

template <typename T>
void TaskQueue<T>::inQueue(const T & data)
{
    MutexLockGuard lock(_mutex);
    while(isFull()){
        _isFull.wait();
    }
    _data.push(data);
    _isEmpty.notify();
}

template <typename T>
void TaskQueue<T>::inQueue(const T && data)
{
    MutexLockGuard lock(_mutex);
    while(isFull()){
        _isFull.wait();
    }
    _data.push(std::move(data));
    _isEmpty.notify();
}

template <typename T>
T TaskQueue<T>::deQueue()
{
    MutexLockGuard lock(_mutex);
    //让线程池能够退出来
    while(isEmpty()){
        _isEmpty.wait();
        if(_exitFlag){
            return T(nullptr);
        }
    }
    auto ret = _data.front();
    _data.pop();
    _isFull.notify();
    return ret;
}

template <typename T>
void TaskQueue<T>::wakeUp()
{
    _exitFlag = true;
    _isEmpty.notifyall();
}

#endif

