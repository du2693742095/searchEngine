/***********************************************************
  > File Name: TcpServer.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月10日 星期六 23时12分06秒
  > Modified Time:2022年09月10日 星期六 23时12分06秒
 *******************************************************/
#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "Acceptor.hh"
#include "EventLoop.hh"
#include <string>

class TcpServer
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using callBackFunc = std::function<void(TcpConnectionPtr)>;

public:
    TcpServer(const std::string &ip, unsigned short port);
    TcpServer(const std::string &&ip, unsigned short port);
    ~TcpServer();

    void start();
    void stop();
    void setAllCallBack(callBackFunc && newConnetionFunc,
                        callBackFunc && messageFunc, 
                        callBackFunc && closeFunc);
    
private:
    Acceptor _acceptor;
    EventLoop _loop;
};

#endif
