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
#include <string>
#include <memory>

class EchoServer
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

public:
    EchoServer(size_t numi, size_t size,
               const std::string &ip, unsigned short port);
    ~EchoServer();

    void start();
    void stop();

private:
    void onConnection(const TcpConnectionPtr &);
    void onMessage(const TcpConnectionPtr &);
    void onClose(const TcpConnectionPtr &);

private:
    std::unique_ptr<ThreadPoll> _poll;
    std::unique_ptr<TcpServer> _server;
};

#endif
