/***********************************************************

  > File Name: EventLoop.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月08日 星期四 14时33分14秒
  > Modified Time:2022年09月08日 星期四 14时33分14秒
 *******************************************************/
#ifndef __EVENTLOOP_HH__
#define __EVENTLOOP_HH__

#include "MutexLock.hh"
/* #include "Acceptor.hh" */
/* #include "TcpConnection.hh" */
#include <sys/epoll.h>
#include <sys/types.h>
#include <memory>
#include <vector>
#include <map>
#include <functional>

class Acceptor;
class TcpConnection;

class EventLoop
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using callBackFunc = std::function<void(TcpConnectionPtr)>;
    using sendFunctions = std::function<void()>;

public:
    EventLoop(Acceptor & acceptor);
    ~EventLoop();
    void loop();
    void unloop();

    void setConnectionCallBack(callBackFunc &&cb);
    void setMessageCallBack(callBackFunc &&cb);
    void setCloseCallBack(callBackFunc &&cb);

    void ReadEventFd();//从evenfd中读出数据，阻塞
    void wakeupEventFd();//往eventfd中写数据

    void runInLoop(sendFunctions && cb);//处理从TcpConnection发来的函数

private:
    int createEpollFd();
    void waitEpollFd();
    void addEpollReaddFd(int fd);
    void delEpollReadFd(int fd);

    void handNewConnection();
    void handleMessage(int fd);

    int createEventFd();
    void runPengdingFunc();//执行发送函数

private:
    int _epollfd;
    int _eventfd;
    bool _isLoop;
    Acceptor & _listen;
    std::vector<struct epoll_event> _eventList;
    std::map<int, TcpConnectionPtr> _cones; 
    callBackFunc _connectionCallBack;
    callBackFunc _messageCallBack;
    callBackFunc _closeCallBack;

    std::vector<sendFunctions> _pengdingsCb;
    MutexLock _mutex;
};

#endif
