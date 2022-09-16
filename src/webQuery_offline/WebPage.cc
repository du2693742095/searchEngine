/***********************************************************
  > File Name: WebPage.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月14日 星期三 19时56分47秒
  > Modified Time:2022年09月14日 星期三 19时56分47秒
 *******************************************************/
#include "../../include/WebPage.hh"
#include <regex>

WebPage::WebPage(){}

#include <iostream>
using std::cout;
using std::endl;

#if 1
WebPage::WebPage(const string & doc)
{
    auto left = doc.find("<docid>") + 7;
    auto right = doc.find("</docid>");
    string id = doc.substr(left, right - left);
    _docId = atoi(id.c_str());

    left = doc.find("<url>") + 5;
    right = doc.find("</url");
    string url = doc.substr(left, right - left);
    _docUrl = std::move(url);

    left = doc.find("<title>") + 7;
    right = doc.find("</title>");
    string title = doc.substr(left, right - left);
    _docTitle = std::move(title);

    left = doc.find("<content>") + 9;
    string content = doc.substr(left, doc.size() - 16 - left);
    _docContent = std::move(content);
}

#endif

#if 0
void WebPage::cut()
{
    std::smatch result;
    std::regex pattern("<doc><docid>(.*)</docid><url>(.*)</url><title>(.*)</title><content>(.*)</content></doc>");
    cout << "进来了" << " ";
    if(regex_match(_doc, result, pattern))
    {
        /* cout << atoi(result.str(1).c_str()) << endl; */
        /* cout << result.str(2) << endl; */
        /* cout << result.str(3) << endl; */
        /* cout << result.str(4) << endl; */
        _docId = atoi(result.str(1).c_str());
        _docUrl = result.str(2);
        _docTitle = result.str(3);
        _docContent = result.str(4);
    }
}
#endif

size_t WebPage::getDocId()
{
    return _docId;
}

string & WebPage::getTitle()
{
    return _docTitle;
}

string & WebPage::getContent()
{
    return _docContent;
}

//对文档的id进行比较
bool operator < (const WebPage &lhs, const WebPage &rhs)
{
    if(lhs._docId < rhs._docId){
        return true;
    }
    else{
        return false;
    }
}

