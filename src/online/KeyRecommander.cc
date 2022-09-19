/***********************************************************
  > File Name: keyrecommander.cc
  > Author: Huberyee
  > Mail: 923506068@qq.com
  > Created Time: Wed 14 Sep 2022 11:07:16 PM CST
  > Modified Time:Wed 14 Sep 2022 11:07:16 PM CST
 *******************************************************/

#include "../../include/Json.hh"
#include "../../include/KeyRecommander.hh"
#include "../../include/SplitToolCppJieba.hh"
#include "../../include/Configuration.hh"

//执行查询
void KeyRecommander::execute(Dictionary * dict){
    //切割，得到一个个索引
    for(size_t i=0; i<_queryWord.size();){
        //洋文
        if(isalpha(_queryWord[i])){
            _idxSet.insert(string(1,_queryWord[i]));
            ++i;
        }
        //汉字
        else if(_queryWord[i]&0x80 && _queryWord[i+1]&0x80){
            string str = _queryWord.substr(i,3);
            _idxSet.insert(str);
            i += 3;

        }
        //啥也不是
        else{
            ++i;
        }
    }

    /* cout<< "after cut:"<<endl; */
    /* for(auto & elem : _idxSet){ */
    /*     cout<< elem << endl; */

    /* } */ 

    //查索引库，得到行号集合
    set<int> rowSet = queryIndexTable(dict);



    //传候选词到优先级队列
    vector<pair<string,int>>& wordDict = dict->getDict();
    for(auto & row : rowSet){
        CandidateResult candidate(wordDict[row-1].first,
                                  wordDict[row-1].second,
                                  distance(wordDict[row-1].first));
        _resultQue.push(candidate);
    }

    /* cout<< "_resultQue:"<<endl; */
    /* while(!_resultQue.empty()){ */
    /*     cout<< _resultQue.top()._word<<endl; */
    /*     _resultQue.pop(); */
    /* } */
}

//查询索引
set<int> KeyRecommander::queryIndexTable(Dictionary* dict){
    set<int> rowSet;
    map<string,set<int>>& indexTable = dict->getIndexTable();
    for(auto & elem : _idxSet){
        rowSet.insert(indexTable[elem].begin(), indexTable[elem].end());
    }    
    return rowSet;
}

//响应客户端的请求
string KeyRecommander::response(){
    Msg msg;
    int Topk = 5;
    while(Topk>0 && !_resultQue.empty()){
        msg._msg.push_back(_resultQue.top()._word);
        _resultQue.pop();
        --Topk;
    }
    msg._ID = 100;
    msg._msgLen = 0;
    for(auto &elem : msg._msg){
        msg._msgLen += elem.size();
    }
    json j;
    to_json(j, msg);

    return j.dump();
}


//计算最小编辑距离
int KeyRecommander::distance(const string & rhs){
    return editDistance(_queryWord, rhs);
}

