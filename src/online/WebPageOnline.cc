/***********************************************************
  > File Name: WebPageOnline.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月16日 星期五 11时21分47秒
  > Modified Time:2022年09月16日 星期五 11时21分47秒
 *******************************************************/
#include "../../include/WebPageOnline.hh"
#include <sstream>

WebPageOnlie::WebPageOnlie()
    : _docId(0)
    , _docTitle()
    , _docUrl()
    , _docContent()
    , _docSummery()
{}

WebPageOnlie::WebPageOnlie(const string & doc)
{
    cut(doc);
}

/* #include <iostream> */

void WebPageOnlie::cut(const string &_doc)
{
    auto left = _doc.find("<docid>") + 7;
    auto right = _doc.find("</docid>");
    string id = _doc.substr(left, right - left);
    _docId = atoi(id.c_str());

    left = _doc.find("<url>") + 5;
    right = _doc.find("</url");
    string url = _doc.substr(left, right - left);
    _docUrl = std::move(url);

    left = _doc.find("<title>") + 7;
    right = _doc.find("</title>");
    string title = _doc.substr(left, right - left);
    _docTitle = std::move(title);

    left = _doc.find("<content>") + 9;
    string content = _doc.substr(left, _doc.size() - 16 - left);
    _docContent = std::move(content);
}

size_t WebPageOnlie::getDocId()
{
    return _docId;
}

string & WebPageOnlie::getTitle()
{
    return _docTitle;
}

string & WebPageOnlie::getUrl()
{
    return _docUrl;
}

// 获取所查询词的摘要信息
string & WebPageOnlie::getSummery(const vector<string> &queryWords)
{
    /* std::cout << _docContent<< std::endl << std::endl; */
    /* std::cout << _docTitle<< std::endl; */
    
    vector<string> summaryCount;
    std::istringstream is(_docContent);
    string line;
    //找到存在单词的行，然后返回首先出现的行号
    while(std::getline(is, line)) {
        for(auto &word : queryWords) {
            if(line.find(word) != string::npos && summaryCount.size() < 3) {
                summaryCount.push_back(line);
            }
        }
    }

    for(auto &line : summaryCount) {
        _docSummery.append(line);
    }

    return _docSummery;
}

//对文档的id进行比较
bool operator < (const WebPageOnlie &lhs, const WebPageOnlie &rhs)
{
    if(lhs._docId < rhs._docId){
        return true;
    }
    else{
        return false;
    }
}

