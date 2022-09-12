/***********************************************************
  > File Name: Socket.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月07日 星期三 11时34分28秒
  > Modified Time:2022年09月07日 星期三 11时34分28秒
 *******************************************************/
#ifndef __SOCKET_HH__
#define __SOCKET_HH__

#include "NonCopyable.hh"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

//创建套接字，保存socket函数返回的fd
class Socket: public NonCopyable
{
public:
    Socket();
    //直接将fd放入
    explicit Socket(int fd);
    ~Socket();

    int getFd() const;

private:
    int _fd;
};

#endif
