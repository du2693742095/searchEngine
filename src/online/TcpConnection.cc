/***********************************************************
  > File Name: TcpConnection.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月08日 星期四 16时36分18秒
  > Modified Time:2022年09月08日 星期四 16时36分18秒
 *******************************************************/
#include "TcpConnection.hh"
#include "EventLoop.hh"
#include <sstream>

TcpConnection::TcpConnection(int fd, EventLoop &loop)
    : _sock(fd)
    , _sockIO(fd)
    , _peerAddr(getPeerAddr())
    , _localAddr(getLocalAddr())
    , _loop(loop)
{}

TcpConnection::~TcpConnection()
{}

//这里发过来的是左值还是右值无所谓，因为SockIo.writen用的char*指针，
//无所谓左值还是右值
void TcpConnection::sendmsg(const std::string & msg)
{
    _sockIO.writen(msg.c_str(), msg.size());
}

std::string TcpConnection::recvmsg()
{
    char temp[65535] = {0};
    //直接全部读完
    _sockIO.readLine(temp, sizeof(temp));

    return std::string(std::move(temp));
}

bool TcpConnection::isClosed()
{
    char peekTemp;
    int ret = recv(_sock.getFd(), &peekTemp, sizeof(peekTemp), MSG_PEEK);
    return ret == 0;
}

//打印自己和对端的信息
std::string TcpConnection::toString()
{
    std::ostringstream ret;
    ret << _localAddr.getIp() << ":" <<  _localAddr.getPort() << "-->"
        << _peerAddr.getIp() << ":" << _peerAddr.getPort();
    return ret.str();
}

struct sockaddr_in TcpConnection::getPeerAddr()
{
    sockaddr_in ret;
    socklen_t len = sizeof(ret);
    int r = ::getpeername(_sock.getFd(), (struct sockaddr *)&ret, &len);
    if(r == -1){
        perror("getsockname");
    }
    return ret;
}

struct sockaddr_in TcpConnection::getLocalAddr()
{
    sockaddr_in ret;
    socklen_t len = sizeof(ret);
    int r = ::getsockname(_sock.getFd(), (struct sockaddr *)&ret, &len);
    if(r == -1){
        perror("getsockname");
    }
    return ret;
}

void TcpConnection::setConnectionCallBack(callBackFunc &cb)
{
    _connectionCallBack = cb;
}

void TcpConnection::setMessageCallBack(callBackFunc &cb)
{
    _messageCallBack = cb;
}

void TcpConnection::setCloseCallBack(callBackFunc &cb)
{
    _closeCallBack = cb;
}

void TcpConnection::handleConnectionCallBack()
{
    if(_connectionCallBack){
        //为了防止智能指针的误用（拷贝了智能指针）
        _connectionCallBack(shared_from_this());
    }
}

void TcpConnection::handleMessageCallBacl()
{
    if(_messageCallBack){
        _messageCallBack(shared_from_this());
    }
}

void TcpConnection::handleCloseCallBack()
{
    if(_closeCallBack){
        _closeCallBack(shared_from_this());
    }
}

void TcpConnection::sendToEpollLoop(const std::string & msg)
{
    _loop.runInLoop
        (std::bind((void(TcpConnection::*)(const std::string &))
                   &TcpConnection::sendmsg, this, msg));
}

void TcpConnection::sendToEpollLoop(const std::string && msg)
{

    _loop.runInLoop
        (std::bind((void(TcpConnection::*)(const std::string &))
                   &TcpConnection::sendmsg, this, std::move(msg)));

}
