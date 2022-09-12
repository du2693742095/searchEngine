/***********************************************************
  > File Name: Acceptor.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月12日 星期一 09时33分34秒
  > Modified Time:2022年09月12日 星期一 09时33分34秒
 *******************************************************/
#include "Acceptor.hh"

Acceptor::Acceptor(const std::string &ip, unsigned short port)
    : _listenSock()
    , _servAddr(ip, port)
{}

Acceptor::~Acceptor()
{}

void Acceptor::ready()
{
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}

void Acceptor::setReuseAddr()
{
    int opt = 1;
    if(-1 == setsockopt(_listenSock.getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("sersockopt In setReuseAddr");
    }
}

void Acceptor::setReusePort()
{
    int opt = 1;
    if(-1 == setsockopt(_listenSock.getFd(), SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("sersockopt In setReusePort");
    }
}

void Acceptor::bind()
{
    const struct sockaddr *addr = (struct sockaddr*)_servAddr.getInetAddrPtr();
    if(-1 == ::bind(_listenSock.getFd(), addr, sizeof(struct sockaddr))){
        perror("socket bind");
    }
}

void Acceptor::listen()
{
    if(-1 == ::listen(_listenSock.getFd(), MAXBACKLOG)){
        perror("listen");
    }
}

int  Acceptor::accept()
{
    return ::accept(_listenSock.getFd(), NULL, NULL);
}

int Acceptor::getSockFd() const
{
    return _listenSock.getFd();
}

