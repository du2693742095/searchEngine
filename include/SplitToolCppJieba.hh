#ifndef __SPLITTOOLCPPJIEBA_H_
#define __SPLITTOOLCPPJIEBA_H_

//分词工具类
#include "Configuration.hh"
#include "simhash/cppjieba/Jieba.hpp"

#include <vector>
#include <string>


using std::vector;
using std::string;
using namespace cppjieba;

class SplitToolCppJieba
{
public:
    SplitToolCppJieba(Configuration *);
    
    ~SplitToolCppJieba();
    vector<string> cut(const string & sentence);
    bool isStopWord(const string & word);

    bool isEnglish(const string & words);

    string transferToLower(string & words);
private:
    Configuration * _conf;
    cppjieba::Jieba _jieba;
    set<string> _stopword;
};


#endif
