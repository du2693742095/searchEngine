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
#include <unordered_map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::vector;
using std::map;
using std::set;
using std::pair;
using std::string;
using std::unordered_map;
using std::cout;
using std::endl;

class SplitToolCppJieba;

class DirtProducer
{
public:
    DirtProducer(Configuration *);
    DirtProducer(Configuration * conf, SplitToolCppJieba * splitTool);//处理中文
    ~DirtProducer();

    void buildDict_EN();
    void buildDict_ZH();
    void storeDict(const string & filePath);
    void storeDict();//如果不指明地址，就默认用配置文件里面的

    void showDirt();
    
private:
    Configuration  * _conf;
    vector<pair<string, int>> _dict;
    SplitToolCppJieba * _splitTool;
    map<string, int> _index;
    vector<string> _files;
};

#endif
