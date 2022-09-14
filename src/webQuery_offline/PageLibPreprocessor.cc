/***********************************************************
  > File Name: PageLibPreprocessor.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月14日 星期三 21时20分10秒
  > Modified Time:2022年09月14日 星期三 21时20分10秒
 *******************************************************/
#include "../../include/PageLibPreprocessor.hh"

PageLibPreProcessor::PageLibPreProcessor(Configuration *conf)
    : _conf(conf)
    , _simhash(conf)
{}

void PageLibPreProcessor::doProcess()
{
    readInfoFromFile();
    buildInvertTable();
}

void PageLibPreProcessor::readInfoFromFile()
{
    //打开网页库和偏移库
    string pageLibPath = _conf->getConfigMap().find("path_newRipePage")->second;
    string offsetLib = _conf->getConfigMap().find("path_newOffset")->second;
    std::ifstream pageOfs("pageLibPath");
    std::ifstream offsetOfs("offsetLib");
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
    size_t indx = 0;
    while(indx < _offsetLib.size()){
        auto node = _offsetLib.find(indx)->second;
        int pos = node.first;
        int size = node.second;
        
        //读取一篇文章
        pageOfs.seekg(pos);
        string page;
        page.reserve(size);
        pageOfs.get(&*page.begin(), size);

        //看是否重复
        WebPage wp(std::move(page));
        bool isRepeat = _simhash.isRepeat(wp.getContent());
        
        if(isRepeat){
            continue;
        }
        else{
            _pageLib.push_back(std::move(wp));
        }
    }

}

/* 倒排索引库结构：map<string, vector<pair<int, double>>> 
 * string为单词，int为文章id， double为单词在文章中的权重 */
void PageLibPreProcessor::buildInvertTable()
{
    map<string, int> _freqence;//本文中的出现次数
    map<string, int> _totalFrequence;//所有文章中的出现次数
    map<string, int> _inverseDocumentFrequence;//逆文档频率

    for()
}

void PageLibPreProcessor::store()
{

}

