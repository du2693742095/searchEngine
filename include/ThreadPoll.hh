/***********************************************************
  > File Name: ThreadPoll.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月05日 星期一 16时26分24秒
  > Modified Time:2022年09月05日 星期一 16时26分24秒
 *******************************************************/
#ifndef __THREADPOLL_HH__
#define __THREADPOLL_HH__

#include "NonCopyable.hh"
#include "Thread.hh"
#include "TaskQueue.hh"
#include <functional>
#include <vector>
#include <memory>

using Task = std::function<void ()>;

class ThreadPoll: NonCopyable
{
public:
    ThreadPoll(size_t num, size_t size);
    ~ThreadPoll();

    void start();
    void stop();
    void addTask(const Task &);
    void addTask(const Task &&);

private:
    Task getTask();
    void threadFunc();

private:
    size_t _threadNum;
    size_t _queueSize;
    std::unique_ptr<std::vector<std::unique_ptr<Thread>>> _threads;
    std::unique_ptr<TaskQueue<Task>> _taskQueue;
    bool _isExit; //线程池退出的标志
};


#endif

