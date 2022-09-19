/***********************************************************
  > File Name: EchoServer.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月11日 星期日 16时37分29秒
  > Modified Time:2022年09月11日 星期日 16时37分29秒
 *******************************************************/
#include "../../include/EchoServer.hh"
#include "../../include/ThreadTask.hh"
#include <functional>
#include <stdio.h>

EchoServer::EchoServer(const string & confPath)
    : _conf(Configuration::getInstance(confPath))
    , _jieba(_conf)
    , _webQuery(_jieba, _conf)
{
    size_t threadPollNum = ::atoi(_conf->getConfigMap().find("threadNum")->second.c_str());
    size_t taskSize = ::atoi(_conf->getConfigMap().find("tastQueueSize")->second.c_str());
    _poll = make_unique<ThreadPoll>(threadPollNum, taskSize);
    cout << "线程池初始化完成" << endl;

    string ip = _conf->getConfigMap().find("ip")->second.c_str();
    size_t port = ::atoi(_conf->getConfigMap().find("port")->second.c_str());
    _server = make_unique<TcpServer>(ip, port);
    cout << "服务器初始化完成" << endl;

    string redisIP = _conf->getConfigMap().find("ip_redis")->second.c_str();
    size_t redisPort = ::atoi(_conf->getConfigMap().find("port_redis")->second.c_str());
    _redis = make_unique<HiRedis>(redisIP, redisPort);
    cout << "redis启动完成" << endl;

    string dict = _conf->getConfigMap().find("path_dict")->second.c_str();
    string dictIndx = _conf->getConfigMap().find("path_dictIndex")->second.c_str();
    _dict = Dictionary::createInstance();
    _dict->init(dict, dictIndx);
    cout << "词典库和网页库初始化完成" << endl;
}

EchoServer::~EchoServer()
{
    Configuration::destroyIns();
}

void EchoServer::start()
{
    _poll->start();
    cout << "线程池启动！" << endl;
    _server->setAllCallBack(std::bind(&EchoServer::onConnection, this, std::placeholders::_1),
                           std::bind(&EchoServer::onMessage, this, std::placeholders::_1),
                           std::bind(&EchoServer::onClose, this, std::placeholders::_1));
    _server->start();
    cout << "服务器监听启动" << endl;
    
    //应该初始化所有的文件，包括单词查询和网页查询
}

void EchoServer::stop()
{
    _poll->stop();
    _server->stop();
}

//后面可以把这三个函数弄成虚函数，然后用继承类去进行执行
void EchoServer::onConnection(const TcpConnectionPtr &con)
{
    std::cout << con->toString() << ": connected !" << std::endl;
}

void EchoServer::onMessage(const TcpConnectionPtr &con)
{
    /* cout << "EchoServer::onMessage()" << endl; */
    std::string msg = con->recvmsg();
    std::cout << ">>recv: " << msg << std::endl;

    //添加任务
    ThreadTask task(msg, con, _redis.get(), _webQuery, _dict);
    _poll->addTask(std::bind(&ThreadTask::process, task));
}

void EchoServer::onClose(const TcpConnectionPtr &con)
{
    std::cout << con->toString() << ": closed !" << std::endl;
}

