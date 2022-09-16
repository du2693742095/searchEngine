/***********************************************************
  > File Name: keyrecommander.h
  > Author: Huberyee
  > Mail: 923506068@qq.com
  > Created Time: Wed 14 Sep 2022 08:49:20 PM CST
  > Modified Time:Wed 14 Sep 2022 08:49:20 PM CST
 *******************************************************/

#ifndef __KEYRECOMMANDER_HH__
#define __KEYRECOMMANDER_HH__

#include <string>
#include <vector>
#include <set>
#include <queue>

#include "EditDistance.hh"
#include "TcpServer.hh"
#include "CandidateResult.hh"
#include "Dictionary.hh"

using std::string;
using std::vector;
using std::set;
using std::priority_queue;


struct MyCompare
{
    bool operator()(const CandidateResult& lhs, const CandidateResult& rhs) const
    {
        if(lhs._dist > rhs._dist){
            return true;
        }
        else if(lhs._dist < rhs._dist){
            return false;
        }
        else if(lhs._freq < rhs._freq){
            return true;
        }
        else if(lhs._freq > rhs._freq){
            return false;
        }
        else if(lhs._word > rhs._word){
            return true;
        }
        else{
            return false;
        }

    }
};


class KeyRecommander
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
public:
    //构造函数，传入查询词和连接
    KeyRecommander(string& queryWord, const TcpConnectionPtr& conn)
    :_queryWord(queryWord)
    ,_conn(conn)
    {}

    //执行查询
    void execute(Dictionary& dict);
    //响应客户端的请求
    void response();
private:
    //查询索引
    set<int> queryIndexTable(Dictionary& dict);
    //计算最小编辑距离
    int distance(const string & rhs);
private:
    //查询词
    string _queryWord;
    //索引集合
    set<string> _idxSet;
    //连接
    TcpConnectionPtr _conn;
    //保存候选结果集的优先级队列
    priority_queue<CandidateResult,vector<CandidateResult>,MyCompare> _resultQue;
};

#endif
