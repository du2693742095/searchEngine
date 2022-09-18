/***********************************************************
  > File Name: TcpServer.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月17日 星期六 22时54分28秒
  > Modified Time:2022年09月17日 星期六 22时54分28秒
 *******************************************************/
#include "../../include/TcpServer.hh"

TcpServer::TcpServer(const std::string &ip, unsigned short port)
    : _acceptor(ip, port)
    , _loop(_acceptor)
{}

TcpServer::TcpServer(const std::string && ip, unsigned short port)
    : _acceptor(std::move(ip), port)
    , _loop(_acceptor)
{}

TcpServer::~TcpServer()
{}

void TcpServer::start()
{
    _acceptor.ready();
    _loop.loop();
}

void TcpServer::stop()
{
    _loop.unloop();
}

void TcpServer::setAllCallBack(callBackFunc && newConnetionFunc,
                    callBackFunc && messageFunc, 
                    callBackFunc && closeFunc)
{
    _loop.setConnectionCallBack(std::move(newConnetionFunc));
    _loop.setMessageCallBack(std::move(messageFunc));
    _loop.setCloseCallBack(std::move(closeFunc));
}
