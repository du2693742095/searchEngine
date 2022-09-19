/***********************************************************
  > File Name: KeywordsSearcher.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月17日 星期六 20时41分21秒
  > Modified Time:2022年09月17日 星期六 20时41分21秒
 *******************************************************/
#include "../../include/KeywordsSearcher.hh"
#include "../../include/KeyRecommander.hh"

KeywordsSearcher::KeywordsSearcher(const string &key, TcpConnectionPtr conn, 
                                 Dictionary * diction, HiRedis *hiredis)
    : _sought(key)
    , _conn(conn)
    , _diction(diction)
    , _hiredis(hiredis)
{}

void KeywordsSearcher::doQuery()
{
    string result = _hiredis->get(_sought + "_1");
    //现在redis里面查，如果查到了，就让result为redis的值
    //如果没有，那就在数据库里面去查
    /* 去redis里面查找，底层用的是是trig类型，所以查找时候把查找的key用指令做区分
     * 单词查询的后面加_1，网页查询的加_2，用来查询 */
    if(result.size() == 0){
        cout << "在单词库里面查询" << endl;
        KeyRecommander k(_sought);
        k.execute(_diction);
        result = k.response();

        _hiredis->set(_sought + "_1", result);
    }
    
    cout << result << endl;
    //发送
    _conn->sendToEpollLoop(result);
}
