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
#include "WebQuery.hh"
#include "Dictionary.hh"
#include "hiRedis.hh"
#include <iostream>

class ThreadTask
{
    using TcpConnectionPtr = shared_ptr<TcpConnection>;

public:
    ThreadTask(const std::string &msg, const TcpConnectionPtr &con, 
               HiRedis * redis, WebQuery & webQuery, Dictionary *dict);
 
    void process();

private:
    std::string _msg;//还没解析过的包
    TcpConnectionPtr _conn;

    HiRedis * _redis;
    //单词和网页的库查询文件
    WebQuery & _webQuery;
    Dictionary *_dict;
    
};

#endif
