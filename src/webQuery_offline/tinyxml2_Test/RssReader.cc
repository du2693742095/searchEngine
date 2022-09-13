/***********************************************************
  > File Name: RssParse.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年08月23日 星期二 14时13分31秒
  > Modified Time:2022年08月23日 星期二 14时13分31秒
 *******************************************************/
#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include "tinyxml2.h"

#define DEFUALT_XML_SIZE 1000

using std::cout; 
using std::endl;
using std::string;
using std::vector;
using namespace tinyxml2;

struct RssItem
{
    string title;
    string link;
    string description;
    string content;
    RssItem(const string &t, const string &l, const string &d, const string &c)
        : title(t), link(l), description(d), content(c){}
};

class RssReader
{
public:
    RssReader(const string & xmlFile);
    void parseRss(const string & xmlFile);//解析
    void display(void) const;
    void dump(const string & filename);//输出
private:
    vector<RssItem> _rss;
 }; 

RssReader::RssReader(const string & xmlFile)
{
    _rss.reserve(DEFUALT_XML_SIZE);
    parseRss(xmlFile);
}

void RssReader::parseRss(const string & xmlFile)
{
    //打开文件
    XMLDocument xmlDocument;
    XMLError error = xmlDocument.LoadFile(xmlFile.c_str());
    if(error != XML_SUCCESS) {
        std::cerr << "读取 xml 失败：" << xmlDocument.ErrorStr() << endl;
        return;
    }
    
    //找到item结点
    XMLElement* itemNode = xmlDocument.FirstChildElement("rss")
        ->FirstChildElement("channel")->FirstChildElement("item");
    if(!itemNode){
        std::cerr << "获取结点失败" << endl;
        return;
    }

    //循环遍历结点，获取结点信息
    while(itemNode){
        const char *pTitle = itemNode->FirstChildElement("title")->GetText();
        const char *pLink = itemNode->FirstChildElement("link")->GetText();
        const char *pDescription = itemNode->FirstChildElement("description")->GetText();
        const char *pContent = itemNode->FirstChildElement("content:encoded")->GetText();
        
        //做判空处理
        string title, link, description, content;
        if(pTitle)  title = pTitle;
        if(pLink)  link = pLink;
        if(pDescription)  description = pDescription;
        if(pContent)  content = pContent;
        //去除其中的html标签
        std::regex reg("<[^>]+>");
        description = regex_replace(description, reg, "");
        content = regex_replace(content, reg, "");
        
        _rss.push_back(RssItem(title, link, description, content));
        
        itemNode = itemNode->NextSiblingElement("item");
    }
}

void RssReader::dump(const string & filename)
{
    std::ofstream ofs(filename);
    if(!ofs){
        std::cerr << "open " << filename << " failed!" << endl;
        return;
    }
    for(size_t i = 0; i < _rss.size(); ++i){
        ofs << "<doc><docid>" << i + 1
            << "</docid><title>" << _rss[i].title 
            << "</title><link>" << _rss[i].link 
            << "</link><description>" << _rss[i].description
            << "</description><content>" << _rss[i].content 
            << "</content></doc>" << '\n';
    }
}


void RssReader::display(void) const
{
    for(auto &p: _rss){
        cout << "title: " << p.title << endl
             << "link: " << p.link << endl
             << "description: " << p.description << endl
             << "content: " << p.content << endl;
        cout << endl;
    }
}

void test0(){
    RssReader r("coolshell.xml");
    r.dump("XML.dat");
}

int main(){
    test0();
    return 0;
}

