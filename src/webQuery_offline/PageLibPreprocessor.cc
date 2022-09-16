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
    cout << "创建倒排索引库" << endl;
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
    unordered_map<int, pair<int, int>> _offsetLib;//偏移库
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
    _pageLib.push_back(WebPage());
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


#if 1
/* 倒排索引库结构：map<string, vector<pair<int, double>>> 
 * string为单词，int为文章id， double为单词在文章中的权重 */
void PageLibPreProcessor::buildInvertTable()
{
    vector<map<string, int>> freqence;//本文中的出现次数
    freqence.reserve(512);
    map<string, double> inverseDocumentFrequence;//逆文档频率
    
    int idx = 0;
    //这里_freqence[count]就代表第count篇文章
    //初始化文章的频类和所有文章次数
    for(auto &page: _pageLib){
        vector<string> pageWords = _jieba.cut(page.getContent());
        /* cout << pageWords.size() << " "; */
        freqence.push_back(map<string, int>());

        //创建词频
        for(auto &word: pageWords){
            //去停用词
            if(!_jieba.isStopWord(word) && (word.find_first_not_of("0123456789.") != string::npos)){
                freqence[idx][_jieba.transferToLower(word)]++;
            }
        }

        //_totalFrequence入队
        for(auto &word: freqence[idx]){
            _invertIndexTable[word.first].push_back(std::make_pair(idx + 1, word.second));
        }
        ++idx;
    }

    map<size_t, double> sumOfSquares;

    double IDF = 0.0, w = 0.0;
    size_t TF = 0, DF = 0;
    size_t N = _pageLib.size();//文章总数

    //初始化倒排索引库
    for(auto &index: _invertIndexTable){
        DF = index.second.size(); //包含该词的Page数量
        IDF = ::log2((N+0.1) / DF); //逆文档频率(防止DF==N)
        for(auto &it : index.second) {
            TF = it.second; //该词在该Page中出现的次数
            w = TF * IDF; //权重系数
            it.second = w; //更新权重系数
            sumOfSquares[it.first] += ::pow(w, 2); //求权重平方和
        }
    }

    /* 对权重系数归一化处理 */
    for(auto &index : _invertIndexTable) {
        for(auto &it : index.second) {
            it.second = it.second / ::sqrt(sumOfSquares[it.first]);
        }
    }
}
#endif

void PageLibPreProcessor::store()
{
    cout << "倒排索引库存储ing！" << endl;
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

#if 0
/* 倒排索引库结构：map<string, vector<pair<int, double>>> 
 * string为单词，int为文章id， double为单词在文章中的权重 */
void PageLibPreProcessor::buildInvertTable()
{
    vector<map<string, int>> freqence;//本文中的出现次数
    freqence.reserve(512);
    map<string, double> inverseDocumentFrequence;//逆文档频率
    
    int idx = 0;
    //这里_freqence[count]就代表第count篇文章
    //初始化文章的频类和所有文章次数
    for(auto &page: _pageLib){
        vector<string> pageWords = _jieba.cut(page.getContent());
        /* cout << pageWords.size() << " "; */
        freqence.push_back(map<string, int>());

        //创建词频
        for(auto &word: pageWords){
            //去停用词
            if(!_jieba.isStopWord(word)){
                freqence[idx][_jieba.transferToLower(word)]++;
            }
        }

        //_totalFrequence入队
        for(auto &word: freqence[idx]){
            _invertIndexTable[word.first].push_back(std::make_pair(idx + 1, word.second));
        }
        ++idx;
    }

    double IDF = 0.0;
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
#endif
