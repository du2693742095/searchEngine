/***********************************************************
  > File Name: WebPage.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月14日 星期三 19时56分47秒
  > Modified Time:2022年09月14日 星期三 19时56分47秒
 *******************************************************/
#include "../../include/WebPage.hh"

WebPage::WebPage(const string & doc)
    : _doc(doc)
{
    string id(_doc.find("<docid>"),_doc.find("</docid>"));
    _docId = atoi(id.c_str());

    string url(_doc.find("<url>"), _doc.find("</url"));
    _docUrl = std::move(url);

    string title(_doc.find("<title>"), _doc.find("</title>"));
    _docTitle = std::move(title);

    string content(_doc.find("<content>"), _doc.find("<content>"));
    _docContent = std::move(content);
}

WebPage::WebPage(const string && doc)
    : _doc(std::move(doc))
{
    string id(_doc.find("<docid>"),_doc.find("</docid>"));
    _docId = atoi(id.c_str());

    string url(_doc.find("<url>"), _doc.find("</url"));
    _docUrl = std::move(url);

    string title(_doc.find("<title>"), _doc.find("</title>"));
    _docTitle = std::move(title);

    string content(_doc.find("<content>"), _doc.find("<content>"));
    _docContent = std::move(content);
}

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

