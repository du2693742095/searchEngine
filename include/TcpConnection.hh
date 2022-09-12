/***********************************************************
  > File Name: TcpConnection.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月07日 星期三 12时41分54秒
  > Modified Time:2022年09月07日 星期三 12时41分54秒
 *******************************************************/
#ifndef __TCPCONNECTION_HH__
#define __TCPCONNECTION_HH__

#include "Socket.hh"
#include "InetAddress.hh"
#include "SocketIO.hh"
#include <memory>
#include <functional>

class EventLoop;

class TcpConnection
:public std::enable_shared_from_this<TcpConnection>
{
public:
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using callBackFunc = std::function<void(const TcpConnectionPtr)>;
    using sendFunctions = std::function<void()>;
    
public:
    TcpConnection(int fd, EventLoop & loop);
    /* TcpConnection(int fd); */
    ~TcpConnection();

    void sendmsg(const std::string & msg);
    std::string recvmsg();

    std::string toString();

    bool isClosed();

    void setConnectionCallBack(callBackFunc &cb);
    void setMessageCallBack(callBackFunc &cb);
    void setCloseCallBack(callBackFunc &cb);
    void handleConnectionCallBack();
    void handleMessageCallBacl();
    void handleCloseCallBack();

    void sendToEpollLoop(const std::string & msg);
    void sendToEpollLoop(const std::string && msg);

private:
    struct sockaddr_in getPeerAddr();
    struct sockaddr_in getLocalAddr();

private:
    Socket _sock;               //对端的fd
    SocketIO _sockIO;           //用来进行收发

    InetAddress _peerAddr;      //对端的地址
    InetAddress _localAddr;      //本端地址
    
    EventLoop &_loop; //用来将发送消息交给EventLoop进行操作

    callBackFunc _connectionCallBack;
    callBackFunc _messageCallBack;
    callBackFunc _closeCallBack;
};

#endif
