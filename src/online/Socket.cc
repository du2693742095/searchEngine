/***********************************************************
  > File Name: Socket.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月12日 星期一 09时34分44秒
  > Modified Time:2022年09月12日 星期一 09时34分44秒
 *******************************************************/
#include "../../include/Socket.hh"

Socket::Socket()
{
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(_fd < 0){
        perror("socket");
        return ;
    }
}

Socket::Socket(int fd): _fd(fd) {}

Socket::~Socket()
{
    if(_fd)
    {   close(_fd);     }   
}

int Socket::getFd() const
{   return _fd;     }

