/***********************************************************
  > File Name: PageLibPreprocessor.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月13日 星期二 23时25分40秒
  > Modified Time:2022年09月13日 星期二 23时25分40秒
 *******************************************************/
#ifndef __PAGELIBPREPROCESSOR
#define __PAGELIBPREPROCESSOR

#include "Configuration.hh"
#include "DirScanner.hh"
#include "WebPage.hh"
#include "SimhashUseToSplite.hh"

class PageLibPreProcessor
{
public:
    PageLibPreProcessor(Configuration *conf);
    void doProcess();

private:
    void readInfoFromFile();//读取网页和配置信息
    void buildInvertTable();
    void store();

private:
    Configuration *_conf;
    SimHashUseToSplite _simhash;
    vector<WebPage> _pageLib;
    unordered_map<int, pair<int, int>> _offsetLib;//偏移库
    //倒排索引库
    unordered_map<string, vector<pair<int, double>>> _invertIndexTable;

};

#endif
