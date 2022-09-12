/***********************************************************
  > File Name: Acceptor.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月07日 星期三 12时41分24秒
  > Modified Time:2022年09月07日 星期三 12时41分24秒
 *******************************************************/
#ifndef __ACCEPTOR_HH__
#define __ACCEPTOR_HH__

#include "Socket.hh"
#include "InetAddress.hh"
#include <string>

#define MAXBACKLOG 100

//对服务器的套接字进行监听和绑定
class Acceptor
{
public:
    Acceptor(const std::string &ip, unsigned short port);
    ~Acceptor();
    void ready();
    void setReuseAddr();    //设置地址重用
    void setReusePort();    //设置端口号可重用
    void bind();
    void listen();
    int accept();
    int getSockFd() const;

private:
    Socket _listenSock;
    InetAddress _servAddr;
};

#endif
