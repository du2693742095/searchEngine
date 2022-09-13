/***********************************************************
  > File Name: InetAddress.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月12日 星期一 09时32分48秒
  > Modified Time:2022年09月12日 星期一 09时32分48秒
 *******************************************************/
#include "InetAddress.hh"

InetAddress::InetAddress(const std::string &ip, unsigned short port)
{
    ::memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

InetAddress::InetAddress(const struct sockaddr_in &rhs)
{
    _addr.sin_family = rhs.sin_family;
    _addr.sin_port = rhs.sin_port;
    _addr.sin_addr.s_addr = rhs.sin_addr.s_addr;
}

InetAddress::~InetAddress()
{  }

std::string InetAddress::getIp()
{
    return inet_ntoa(_addr.sin_addr);
}

unsigned short InetAddress::getPort()
{
    return ntohs(_addr.sin_port);
}

const struct sockaddr_in * InetAddress::getInetAddrPtr()
{
    return &_addr;
}
