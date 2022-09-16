/***********************************************************
  > File Name: WebQuery.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月16日 星期五 15时08分16秒
  > Modified Time:2022年09月16日 星期五 15时08分16秒
 *******************************************************/
#include "../../include/WebQuery.hh"
#include "../../include/Json.hh"
#include <nlohmann/json.hpp>
#include <iterator>

using json = nlohmann::json;

WebQuery::WebQuery(SplitToolCppJieba &jieba, Configuration *conf)
    : _jieba(jieba)
{
    //初始化网页库和倒排索引库
    string pageLibPath = conf->getConfigMap().find("path_newRipePage")->second;
    string offsetLib = conf->getConfigMap().find("path_newOffset")->second;
    string invertLib = conf->getConfigMap().find("path_invertIndex")->second;
    std::ifstream pageOfs(pageLibPath);
    std::ifstream offsetOfs(offsetLib);
    std::ifstream invertOfs(invertLib);
    if(!pageOfs.is_open() || !offsetOfs.is_open() || !invertOfs.is_open()){
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
    while(indx <= _offsetLib.size()){
        auto node = _offsetLib.find(indx++)->second;
        int pos = node.first;//文章首地址
        int size = node.second;//文章长度
        
        //读取一篇文章
        pageOfs.seekg(pos);
        char *page = new char[size + 1]();
        pageOfs.read(page, size);
        /* cout << pos  << " "<< strlen(page) << " " << size << endl; */
        _pageLib.insert(make_pair(indx, WebPageOnlie(std::move(string(page)))));
        delete [] page;
    }

    //读取倒排索引库
    string word;
    double w;
    int id;
    while(std::getline(invertOfs, line)){
        istringstream ist(line);
        invertOfs >> word;
        set<pair<int, double>> temp;
        while(ist >> id >> w){
            /* cout << word << " "<< id << " " << w; */
            temp.insert(make_pair(id, w));
        }
        _invertIndexTable.insert(make_pair(word, temp));
    }

        for(auto &p: _invertIndexTable["尽如人意"]){
            cout << p.first << " " << p.second << endl;
        }           
}

class PageCompare
{
public:
    PageCompare(vector<double> & queryWordsWeight)
        : _queryWordsWeight(queryWordsWeight)
    {}

    bool operator()(const pair<int, vector<double>> &lhs, 
                    const pair<int, vector<double>> &rhs)
    {
        double lhs_x = 0, lhs_y = 0;
        double rhs_x = 0, rhs_y = 0;

        const vector<double> &lhsVec = lhs.second;
        const vector<double> &rhsVec = rhs.second;
        for(size_t i = 0; i < lhsVec.size(); ++i){
            lhs_x += _queryWordsWeight[i] * lhsVec[i];
            rhs_x += _queryWordsWeight[i] * rhsVec[i];
            lhs_y += pow(lhsVec[i], 2);
            rhs_y += pow(rhsVec[i], 2);
        }
        double lshCos = lhs_x / sqrt(lhs_y); 
        double rshCos = rhs_x / sqrt(rhs_y); 
        if(lshCos > rshCos){
            return true;
        }
        else{
            return false;
        }
    }

private:
    vector<double> & _queryWordsWeight;
};

string WebQuery::doQuery(const string &str)
{
    //对待查询字符串分词
    vector<string> queryWords;
    queryWords = _jieba.cut(str);

    /* string words; */
    /* for(auto &it : queryWords) { */
    /*     words.append(it + ", "); */
    /* } */
    /* return words; */

    for(auto &it : queryWords) {
        if(_invertIndexTable.find(it) == _invertIndexTable.end()) {
            return returnNoAnswer(str);
        }
    }

    //计算每个待查询词的权重
    vector<double> queryWordsWeight = getQueryWordsWeight(queryWords);
    
    //格式是<docid, 关键词在这篇网页中的权重系数数组>
    vector<pair<int, vector<double>>> resultVec; 
    
    //如果找到了，那就排序网页
    //如果没找到，那就返回空
    if(executeQuery(queryWords, resultVec)) {
        //排序
        cout << "查出来的篇数: " << resultVec.size() << endl;
        PageCompare cmp(queryWordsWeight);
        std::sort(resultVec.begin(), resultVec.end(), cmp);

        vector<int> docidVec;
        for(auto &it : resultVec) {
            docidVec.push_back(it.first);
        }
        return createJSON(docidVec, queryWords);
    }
    else {
        return returnNoAnswer(str);
    }
}

//在逆文档索引库中计算查询词的权重
vector<double> WebQuery::getQueryWordsWeight(vector<string> &queryWords)
{
    //统计查询词的词频
    map<string, int> wordFreqMap;
    for(auto &it : queryWords) {
        ++wordFreqMap[it];
    }

    vector<double> weightVec;//每次词的权重
    double wTotal = 0;

    int TF = 0, DF = 1;//默认查询词就是一篇文章
    double IDF = 0.0, w = 0.0;
    int N = _pageLib.size();
    for(auto &it : queryWords) {
        TF = wordFreqMap[it];  //词频
        IDF = log2((double)(N + 0.1) / DF); 
        w = TF * IDF; 
        wTotal += w;
        weightVec.push_back(w);
    }

    //对权重系数归一化处理
    for(auto &it : weightVec) {
        it /= sqrt(wTotal);
    }

    return weightVec;
}

//resultVec存每个文章中的单词权重表，如果查到了，就把结果放到resultVec
//resultVec格式：pair(文章，文章中查询词的权重)
bool WebQuery::executeQuery(const vector<string> &queryWords, 
                  vector<pair<int, vector<double>>> &resultVec)
{
    if(queryWords.size() == 0){
        return false;
    }

    //保存每个单词的set表，下标就表示查询词
    using setIter = set<pair<int, double>>::iterator;
    vector<pair<setIter, int>> iterVec; //保存需要取交集的迭代器
    int minIterNum = -1;
    
    /* cout << queryWords.size() << endl; */
    /* for(auto &w: queryWords){ */
    /*     cout << w << endl; */
    /* } */

    //查找所有关键词对应的文章id和权重
    for(auto &it : queryWords) {
        int size = _invertIndexTable[it].size();
        
        /* cout << size << endl; */
        /* cout << it << endl; */
        /* for(auto &p: _invertIndexTable[it]){ */
        /*     cout << p.first << " "<<  p.second << endl; */
        /* } */

        if(size == 0) {
            return false;
        }
        if(minIterNum > size) {
            minIterNum = size;
        }
        iterVec.push_back(make_pair(_invertIndexTable[it].begin(), 0));
    }

    bool isContinue = true;
    while(isContinue) {
        size_t idx = 0;
        for(; idx != iterVec.size() - 1; ++idx)
        {
            if((iterVec[idx].first)->first != iterVec[idx+1].first->first)
                break;
        }

        if(idx == iterVec.size() - 1)
        {   // 找到相同的docid
            vector<double> weightVec;
            int docid = iterVec[0].first->first;
            for(idx = 0; idx != iterVec.size(); ++idx)
            {
                weightVec.push_back(iterVec[idx].first->second);
                ++(iterVec[idx].first);//相同时，将每一个迭代器++
                ++(iterVec[idx].second);//同时记录迭代器++的次数
                if(iterVec[idx].second == minIterNum)
                {   isContinue = false;   }

            }
            resultVec.push_back(make_pair(docid, weightVec));
        }
        else
        {   //找到最小的docid，并保存其所在的iter++
            int minDocId = -1;
            int iterIdx;//保存minDocId的迭代器位置
            for(idx = 0; idx != iterVec.size(); ++idx)
            {
                if(iterVec[idx].first->first < minDocId)
                {
                    minDocId = iterVec[idx].first->first;
                    iterIdx = idx;

                }

            }

            ++(iterVec[iterIdx].first);
            ++(iterVec[iterIdx].second);
            if(iterVec[iterIdx].second == minIterNum)
            {   isContinue = false;   }
        }

    }
    return true;
}

//vector<string>表示每篇文章
//string内第一个字段是文章标题 第二个字段是url，第三个字段是摘要
string WebQuery::createJSON(vector<int> &docId, const vector<string> &queryWords)
{
    Msg ret;
    ret._ID = 200;
    ret._msg.reserve(docId.size());

    for(size_t i = 0; i < docId.size(); ++i){
        int id = docId[i];
        auto page = _pageLib[id];
        ret._msg.push_back(page.getTitle());
        ret._msg.push_back(page.getUrl());
        ret._msg.push_back(page.getSummery(queryWords));
    }
    ret._msgLen = docId.size();
    /* cout << "查出来的篇数:" << docId.size()<< endl; */

    json j;
    to_json(j, ret);
    return j.dump();
}

string WebQuery::returnNoAnswer(const string & msg)
{
    Msg ret;
    ret._ID = 200;
    ret._msg = vector<string>();
    ret._msg.push_back("抱歉没有找到与“" + msg + "”相关的网页。");
    ret._msgLen = 0;
    json j;
    to_json(j, ret);
    return j.dump();
}

