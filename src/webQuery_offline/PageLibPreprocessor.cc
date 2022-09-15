/***********************************************************
  > File Name: PageLibPreprocessor.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月14日 星期三 21时20分10秒
  > Modified Time:2022年09月14日 星期三 21时20分10秒
 *******************************************************/
#include "../../include/PageLibPreprocessor.hh"
#include <cmath>

PageLibPreProcessor::PageLibPreProcessor(Configuration *conf)
    : _conf(conf)
    , _simhash(conf)
    , _jieba(conf)
{
    _pageLib.reserve(1000);
    cout << "PageLibPreProcessor初始化完成！" << endl;
}

void PageLibPreProcessor::doProcess()
{
    readInfoFromFile();
    deduplication();
    buildInvertTable();
}

void PageLibPreProcessor::readInfoFromFile()
{
    //打开网页库和偏移库
    string pageLibPath = _conf->getConfigMap().find("path_newRipePage")->second;
    string offsetLib = _conf->getConfigMap().find("path_newOffset")->second;
    std::ifstream pageOfs(pageLibPath);
    std::ifstream offsetOfs(offsetLib);
    if(!pageOfs.is_open() || !offsetOfs.is_open()){
        cerr << "fail to open in PageLibPreProcessor";
    }

    //读取偏移库
    string line;
    line.reserve(20);
    while(std::getline(offsetOfs, line)){
        std::stringstream str(line);
        int id, pos, size;
        str >> id >> pos >> size;
        _offsetLib.insert({id, pair<int, int>(pos, size)});
    }
    //读取网页库
    size_t indx = 1;
    while(indx <= _offsetLib.size()){
        auto node = _offsetLib.find(indx++)->second;
        int pos = node.first;//文章首地址
        int size = node.second;//文章长度
        
        //读取一篇文章
        pageOfs.seekg(pos);
        char *page = new char[size + 1]();
        pageOfs.read(page, size);
        /* cout << pos  << " "<< strlen(page) << " " << size << endl; */
        _pageLib.push_back(WebPage(std::move(string(page))));
        delete [] page;
    }
}

void PageLibPreProcessor::deduplication()
{
    SimHashUseToSplite sim(_conf);
    for(auto page = _pageLib.begin(); page != _pageLib.end(); ++page){
        if(sim.isRepeat(page->getContent())){
            //如果重复了，就置空
            *page = WebPage();
        }
    }
}

/* 倒排索引库结构：map<string, vector<pair<int, double>>> 
 * string为单词，int为文章id， double为单词在文章中的权重 */
void PageLibPreProcessor::buildInvertTable()
{
    vector<map<string, int>> freqence;//本文中的出现次数
    freqence.reserve(512);
    map<string, int> totalFrequence;//所有文章中的出现次数
    map<string, double> inverseDocumentFrequence;//逆文档频率
    
    int count = 0;

    //这里_freqence[count]就代表第count篇文章
    //初始化文章的频类和所有文章次数
    for(auto &page: _pageLib){
        vector<string> pageWords = _jieba.cut(page.getContent());
        /* cout << pageWords.size() << " "; */
        freqence.push_back(map<string, int>());

        //_freqence入队
        for(auto &word: pageWords){
            //去停用词
            if(!_jieba.isStopWord(word)){
                freqence[count][_jieba.transferToLower(word)]++;
            }
        }

        //_totalFrequence入队
        for(auto &word: freqence[count]){
            ++totalFrequence[word.first];
        }
        ++count;
    }

    double w = 0.0;
    long double wTotal = 0.0;
    int totalPageSize = freqence.size();
    int pageId = 1;

    //初始化倒排索引库
    for(auto p: totalFrequence){
        _invertIndexTable.insert({p.first, vector<pair<int, double>>()});
    }

    cout << "3" << endl;
    //遍历所有文章的词频
    for(auto &pageFreqence: freqence){
        //遍历词频
        for(auto &word: pageFreqence){
            w = std::log(totalPageSize / totalFrequence.find(word.first)->second);    
            inverseDocumentFrequence[word.first] = w;
            wTotal += std::pow(w, 2);
        }
        
        //统计最终的权重
        for(auto &word: inverseDocumentFrequence){
            w = word.second;
            double wReal = w / sqrt(wTotal);
            /* word.second = wReal; */

            //插入倒排索引库
            _invertIndexTable[word.first].push_back({pageId, wReal});
        }
        ++pageId;
    }
    cout << "3" << endl;

    /* cout << endl; */
    /* cout << freqence.size() << endl; */
    /* cout << totalFrequence.size() << endl; */
    /* cout << inverseDocumentFrequence.size() << endl; */
}

void PageLibPreProcessor::store()
{
    cout << "存储成功" << endl;
    string path = _conf->getConfigMap().find("path_invertIndex")->second;
    ofstream ifs(path);
    //输出一个单词
    for(auto &page: _invertIndexTable){
        ifs << page.first << " ";
        //输出一个单词对应的文章和权重
        for(auto &p2: page.second){
            ifs << p2.first << " "
                << p2.second << " ";
        }
        ifs << '\n';
    }
}

