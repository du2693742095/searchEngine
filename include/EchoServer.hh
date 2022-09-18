/***********************************************************
  > File Name: EchoServer.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月11日 星期日 16时25分00秒
  > Modified Time:2022年09月11日 星期日 16时25分00秒
 *******************************************************/
#ifndef __ECHOSERVER_HH__
#define __ECHOSERVER_HH__

#include "TcpServer.hh"
#include "ThreadPoll.hh"
#include "SplitToolCppJieba.hh"
#include "WebQuery.hh"
#include "Dictionary.hh"
#include "hiRedis.hh"
#include <string>
#include <memory>

class EchoServer
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

public:
    EchoServer(const string & confPath);
    ~EchoServer();

    void start();
    void stop();

private:
    void onConnection(const TcpConnectionPtr &);
    void onMessage(const TcpConnectionPtr &);
    void onClose(const TcpConnectionPtr &);

private:
    //配置文件指针
    Configuration * _conf;

    std::unique_ptr<ThreadPoll> _poll;
    std::unique_ptr<TcpServer> _server;

    std::unique_ptr<HiRedis> _redis;
    SplitToolCppJieba _jieba;//jieba库文件

    //初始化网页查询库和单词查询库
    WebQuery _webQuery;
    Dictionary * _dict;

};

#endif
