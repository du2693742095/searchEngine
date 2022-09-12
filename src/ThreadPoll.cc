/***********************************************************
  > File Name: ThreadPoll.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月06日 星期二 10时54分10秒
  > Modified Time:2022年09月06日 星期二 10时54分10秒
 *******************************************************/
#include "ThreadPoll.hh"
#include <unistd.h>

ThreadPoll::ThreadPoll(size_t num, size_t size)
    : _threadNum(num)
    , _queueSize(size)
    , _threads(new std::vector<std::unique_ptr<Thread>>())
    , _taskQueue(new TaskQueue<Task>(_queueSize))
    , _isExit(false)
{
    _threads->reserve(_threadNum);
}

ThreadPoll::~ThreadPoll() 
{
    if(!_isExit){
        stop();
        _isExit = true;
    }
}

void ThreadPoll::start()
{
    for(size_t i = 0; i < _threadNum; ++i){
        _threads->push_back
            (std::unique_ptr<Thread>
             (new Thread(std::bind(&ThreadPoll::threadFunc, this))));
    }
    for(auto & th: *_threads){
        th->start();
        /* std::cout << "添加进去了" << std::endl; */
    }
}

void ThreadPoll::stop()
{
    while(!_taskQueue->isEmpty()){
        sleep(1);
    }
    _isExit = true;
    _taskQueue->wakeUp();

    for(auto & th: *_threads){
        /* std::cout << "退出ing" << std::endl; */
        th->join();
    }
}

void ThreadPoll::addTask(const Task & task)
{
    if(task){
        _taskQueue->inQueue(task);
    }
}

void ThreadPoll::addTask(const Task && task)
{
    if(task){
        _taskQueue->inQueue(std::move(task));
    }
}

Task ThreadPoll::getTask()
{
    return _taskQueue->deQueue();
}

void ThreadPoll::threadFunc()
{
    while(!_isExit){
        Task task = getTask();
        if(task){
            task();
        }
    }
}


