/***********************************************************
  > File Name: KeySearcher.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月17日 星期六 20时36分35秒
  > Modified Time:2022年09月17日 星期六 20时36分35秒
 *******************************************************/
#ifndef __KEYSWORDERACHER_HH__
#define __KEYSWORDERACHER_HH__

#include "TcpConnection.hh"
#include "Dictionary.hh"
#include "hiRedis.hh"

class KeywordsSearcher
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
public:
    KeywordsSearcher(const string &key, TcpConnectionPtr conn,
                    Dictionary *diction, HiRedis *hiredis);
    void doQuery();

private:
    string _sought;
    TcpConnectionPtr _conn;
    Dictionary * _diction;
    HiRedis * _hiredis;

};

#endif
