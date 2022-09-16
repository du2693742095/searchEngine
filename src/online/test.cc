/***********************************************************
  > File Name: test.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月09日 星期五 09时16分17秒
  > Modified Time:2022年09月09日 星期五 09时16分17秒
*******************************************************/
#include "../../include/Acceptor.hh"
#include "../../include/TcpConnection.hh"
#include "../../include/EventLoop.hh"
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::string;

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

void onConnection(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has connect！" << endl;
}

void onMessage(const TcpConnectionPtr &con)
{
    string msg = con->recvmsg();
    cout << ">>recv client " << msg << endl;
    con->sendmsg(msg);
}

void onClose(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has closed！" << endl;
}

void test0(){
    Acceptor a("127.0.0.1", 8888);
    a.ready();
    EventLoop loop(a);
    
    loop.setConnectionCallBack(onConnection);
    loop.setMessageCallBack(onMessage);
    loop.setCloseCallBack(onClose);
    
    loop.loop();
}

int main(){
    test0();
    return 0;
}

