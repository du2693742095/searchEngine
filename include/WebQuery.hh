/***********************************************************
  > File Name: WebQuery.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月16日 星期五 12时03分29秒
  > Modified Time:2022年09月16日 星期五 12时03分29秒
 *******************************************************/
#ifndef __WEBQUERY_HH__
#define __WEBQUERY_HH__

#include "SplitToolCppJieba.hh"
#include "Configuration.hh"
#include "WebPageOnline.hh"

//直接用来查询的类
class WebQuery
{
public:
    WebQuery(SplitToolCppJieba &jieba, Configuration *conf);
    string doQuery(const string &str);

private:
    vector<double> getQueryWordsWeight(vector<string> &queryWords);
    //resultVec存每个文章中的单词权重表
    bool executeQuery(const vector<string> &queryWords, 
                      vector<pair<int, vector<double>>> &resultVec);
    string createJSON(vector<int> &docId, const vector<string> &queryWords);
    string returnNoAnswer(const string & msg);
    

private:
    SplitToolCppJieba &_jieba;
    unordered_map<int, WebPageOnlie> _pageLib;//网页库
    //倒排索引
    unordered_map<string, set<pair<int, double>>> _invertIndexTable;

};

#endif
