/***********************************************************
  > File Name: EventLoop.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月08日 星期四 14时54分45秒
  > Modified Time:2022年09月08日 星期四 14时54分45秒
 *******************************************************/
#include "../../include/Acceptor.hh"
#include "../../include/TcpConnection.hh"
#include "../../include/EventLoop.hh"
#include <sys/eventfd.h>

EventLoop::EventLoop(Acceptor & listen)
    : _epollfd(createEpollFd())
    , _eventfd(createEventFd())
    , _isLoop(false)
    , _listen(listen)
    , _eventList(128)
{
    //监听listenfd
    addEpollReaddFd(_listen.getSockFd());
    addEpollReaddFd(_eventfd);
}

EventLoop::~EventLoop()
{
    close(_eventfd);
    close(_epollfd);
}

void EventLoop::loop()
{
    _isLoop = true;
    while(true){
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    _isLoop = false;
}

void EventLoop::waitEpollFd()
{
    int nready = 0;
    do{
        nready = epoll_wait(_epollfd, &*_eventList.begin(), _eventList.size(), 5000);
    }while(-1 == nready && errno == EINTR);

    if(-1 == nready){
        perror("epoll_wait");
        return ;
    }
    else if(0 == nready){
        printf("epoll time out.\n");
    }
    else{
        //超容量则扩容
        if(nready >= static_cast<int>(_eventList.size())){
            _eventList.reserve(1.5 * nready);
        }
        
        //事件处理
        for(int i = 0; i < nready; ++i){
            int fd = _eventList[i].data.fd;

            //新链接
            if(fd == _listen.getSockFd()){
                //是读事件
                if(_eventList[i].events & EPOLLIN){
                    handNewConnection();
                }
            }
            //线程池发送过来的消息
            else if(fd == _eventfd){

printf("检测到有消息要发送\n");

                ReadEventFd();
                runPengdingFunc();
            }
            //旧链接发来消息
            else {
                if(_eventList[i].events & EPOLLIN){
printf("旧链接发消息了\n");
                    handleMessage(fd);
                }
            }
        }
    }
}

void EventLoop::handNewConnection()
{
    int peerfd = _listen.accept();
    if(peerfd < 0)
    {
        perror("peerfd in accept");
        return ;
    }

    addEpollReaddFd(peerfd);
    
    //将函数注册到TCPconnection中
    TcpConnectionPtr newNode = std::make_shared<TcpConnection>(peerfd, *this);

    newNode->setConnectionCallBack(_connectionCallBack);
    newNode->setMessageCallBack(_messageCallBack);
    newNode->setCloseCallBack(_closeCallBack);

    //将新节点加入到map中
    _cones.insert(std::pair<int, TcpConnectionPtr>(peerfd, newNode));

    newNode->handleConnectionCallBack();
printf("新连接开始了\n");
}

void EventLoop::handleMessage(int fd)
{
    auto it = _cones.find(fd);
    if(it != _cones.end()){
        if(it->second->isClosed()){
            it->second->handleCloseCallBack();
            delEpollReadFd(fd);
            _cones.erase(fd);
        }
        else{
            /* printf("处理新消息\n"); */
            it->second->handleMessageCallBacl();
        }
    }
    else{
        //没有这个链接
        perror("This Connection not there");
    }
/* printf("新消息来了\n"); */
}

void EventLoop::addEpollReaddFd(int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    int ret = epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &ev);
    if(ret != 0){
        perror("epoll_ctl in addEpollReaddFd");
    }
}

void EventLoop::delEpollReadFd(int fd)
{
    epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    int ret = epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &ev);
    if(ret != 0){
        perror("epoll_ctl in delEpollReadFd");
    }
}

int EventLoop::createEpollFd()
{
    int ret = epoll_create1(0);
    if(ret == -1){
        perror("epoll_create1");
        return -1;
    }
    return ret;
}

void EventLoop::setConnectionCallBack(callBackFunc &&cb)
{
    _connectionCallBack = std::move(cb);
}

void EventLoop::setMessageCallBack(callBackFunc &&cb)
{
    _messageCallBack = std::move(cb);
}

void EventLoop::setCloseCallBack(callBackFunc &&cb)
{
    _closeCallBack = std::move(cb);
}

/* ==============eventfd的函数============ */
void EventLoop::ReadEventFd()
{
    uint64_t data = 0;
    int ret = read(_eventfd, &data, sizeof(uint64_t));
    if(ret != sizeof(uint64_t)){
        perror("read In ReadEventFd");
        return ;
    }
}

void EventLoop::wakeupEventFd()
{
    uint64_t data = 1;
    int ret = write(_eventfd, &data, sizeof(uint64_t));
    if(ret != sizeof(uint64_t)){
        perror("read In writeEventFd");
        return ;
    }
}

int EventLoop::createEventFd()
{
    int ret = eventfd(0,0);
    if(ret == -1){
        perror("eventfd In EventLoop::createEpollFd");
        return -1;
    }
    return ret;
}

void EventLoop::runInLoop(sendFunctions && cb)
{
    {
        MutexLockGuard lock(_mutex);
        _pengdingsCb.push_back(cb);
    }
    wakeupEventFd();
}

void EventLoop::runPengdingFunc()
{
    std::vector<sendFunctions> temp;
    {
        MutexLockGuard lock(_mutex);
        temp.swap(_pengdingsCb);
    }
    //执行发送函数
    for(auto &cb : temp){
        printf("发送消息的函数入队\n");
        cb();
    }
}

