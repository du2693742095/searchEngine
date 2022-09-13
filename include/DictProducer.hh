/***********************************************************
  > File Name: DictProducer.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月13日 星期二 14时34分10秒
  > Modified Time:2022年09月13日 星期二 14时34分10秒
 *******************************************************/
#ifndef __DICTPRODUCER_HH__
#define __DICTPRODUCER_HH__

#include "Configuration.hh"
#include <vector>
#include <map>
#include <set>
#include <string>

using std::vector;
using std::map;
using std::set;
using std::pair;
using std::string;

class SplitTool;

class DirtProducer
{
public:
    DirtProducer(const string &confPath);
    DirtProducer(const string &confPath, SplitTool * splitTool);//处理中文
    ~DirtProducer();

    void buildDict_EN();
    void buildDict_ZH();
    void storeDict(const string & filePath);
    void storeDict();//如果不指明地址，就默认用配置文件里面的

    void showDirt();

private:
    void getFiles();
    void pushDict(const string & word);

private:
    Configuration _conf;
    vector<pair<string, int>> _dict;
    SplitTool * _splitTool;
    map<string, set<int>> _index;
};

#endif
