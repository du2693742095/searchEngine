/***********************************************************
  > File Name: WebPageSearch.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月16日 星期五 14时31分07秒
  > Modified Time:2022年09月16日 星期五 14时31分07秒
 *******************************************************/
#ifndef __WEBPAGESEARCH_HH__
#define __WEBPAGESEARCH_HH__

#include "TcpConnection.hh"

using std::string;

//接口函数，用来接收线程池的数据，然后执行查询，再发送给tcpconnetcion
class WebPageSearcher
{
public:
    WebPageSearcher(const string &key, const TcpConnection *conn);
    void doQuery();

private:
    string _sought;
    TcpConnection * _conn;

};

#endif
