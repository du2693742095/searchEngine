/***********************************************************
  > File Name: EchoServer.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月11日 星期日 16时37分29秒
  > Modified Time:2022年09月11日 星期日 16时37分29秒
 *******************************************************/
#include "EchoServer.hh"
#include "ThreadTask.hh"
#include <functional>
#include <iostream>

EchoServer::EchoServer(size_t num, size_t size, 
                       const std::string &ip, unsigned short port)
    : _poll(new ThreadPoll(num, size))
    , _server(new TcpServer(ip, port))
{}

EchoServer::~EchoServer()
{

}

void EchoServer::start()
{
    _poll->start();
    _server->setAllCallBack(std::bind(&EchoServer::onConnection, this, std::placeholders::_1),
                           std::bind(&EchoServer::onMessage, this, std::placeholders::_1),
                           std::bind(&EchoServer::onClose, this, std::placeholders::_1));
    _server->start();                            
}

void EchoServer::stop()
{
    _poll->stop();
    _server->stop();
}

//后面可以把这三个函数弄成虚函数，然后用继承类去进行执行
void EchoServer::onConnection(const TcpConnectionPtr &con)
{
    std::cout << con->toString() << "connected !" << std::endl;
}

void EchoServer::onMessage(const TcpConnectionPtr &con)
{
    std::string msg = con->recvmsg();
    std::cout << ">>recv: " << msg << std::endl;
    ThreadTask task("hello", *con);
    _poll->addTask(std::bind(&ThreadTask::process, task));
}

void EchoServer::onClose(const TcpConnectionPtr &con)
{
    std::cout << con->toString() << " closed !" << std::endl;
}

