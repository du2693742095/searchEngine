/***********************************************************
  > File Name: WebPageSearcher.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月16日 星期五 14时35分35秒
  > Modified Time:2022年09月16日 星期五 14时35分35秒
 *******************************************************/
#include "../../include/WebPageSearcher.hh"

WebPageSearcher::WebPageSearcher(const string &key, TcpConnectionPtr conn, 
                                 WebQuery &query, HiRedis *hiredis)
    : _sought(key)
    , _conn(conn)
    , _query(query)
    , _hiredis(hiredis)
{}

void WebPageSearcher::doQuery()
{
    string result = _hiredis->get(_sought + "_2");
    //现在redis里面查，如果查到了，就让result为redis的值
    //如果没有，那就在数据库里面去查
    if(result.size() == 0){
        result = _query.doQuery(_sought);
        _hiredis->set(_sought + "_2", result);
    }
    
    //发送
    _conn->sendToEpollLoop(result);
}
