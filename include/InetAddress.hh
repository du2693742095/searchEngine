/***********************************************************
  > File Name: InetAddress.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月07日 星期三 11时40分06秒
  > Modified Time:2022年09月07日 星期三 11时40分06秒
 *******************************************************/
#ifndef __INETADDRESS_HH__
#define __INETADDRESS_HH__

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <string>

class InetAddress
{
public:
    InetAddress(const std::string &ip, unsigned short port);
    InetAddress(const struct sockaddr_in & rhs);
    ~InetAddress();

    std::string getIp();
    unsigned short getPort();
    const struct sockaddr_in * getInetAddrPtr();

private:
    struct sockaddr_in _addr;
};

#endif
