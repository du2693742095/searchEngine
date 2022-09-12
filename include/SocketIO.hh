/***********************************************************
  > File Name: SocketIO.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月07日 星期三 12时40分52秒
  > Modified Time:2022年09月07日 星期三 12时40分52秒
 *******************************************************/
#ifndef __SOCKETIO_HH__ 
#define __SOCKETIO_HH__

#include "NonCopyable.hh"
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

//负责套接字的读写操作
class SocketIO: public NonCopyable
{
public:
    SocketIO(int fd);
    ~SocketIO(){}

    int readn(char *info, int len);
    int readLine(char *info, int len);
    int writen(const char *info, int len);

private:
    int _fd;
};

#endif
