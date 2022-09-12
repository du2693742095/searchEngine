/***********************************************************
  > File Name: Task.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月11日 星期日 17时48分56秒
  > Modified Time:2022年09月11日 星期日 17时48分56秒
 *******************************************************/
#ifndef __THREADTASK_HH__
#define __THREADTASK_HH__

#include "TcpConnection.hh"
#include <iostream>

class ThreadTask
{
public:
    ThreadTask(const std::string &msg, TcpConnection & con)
        : _msg(msg)
        , _con(con)
    {}

    void process()
    {
        std::cout << "线程执行任务完成！" << std::endl;
        _con.sendToEpollLoop(_msg);
    }

private:
    std::string _msg;
    TcpConnection & _con;
};

#endif
