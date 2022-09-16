/***********************************************************
  > File Name: WebQuery.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月16日 星期五 15时08分16秒
  > Modified Time:2022年09月16日 星期五 15时08分16秒
 *******************************************************/
#include "../../include/WebQuery.hh"

    WebQuery(SplitToolCppJieba &jieba, Configuration &conf);
    string doQuery(const string &str);

private:
    void loadLibrary();
    vector<double> getQueryWordsWeight(vector<string> &queryWords);
    //resultVec存每个文章中的单词权重表
    bool executeQuery(const vector<string> &queryWords, 
                      vector<pair<int, vector<double>>> &resultVec);
    string createJSON(vector<int> &docId, vector<string> &docUrl, 
                      const vector<string> &queryWords);
    string returnNoAnswer();

