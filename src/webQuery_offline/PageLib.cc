/***********************************************************
  > File Name: PageLib.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月13日 星期二 17时18分01秒
  > Modified Time:2022年09月13日 星期二 17时18分01秒
 *******************************************************/
#include "../../include/PageLib.hh"
#include "../../include/tinyxml2.h"
#include <iostream>
#include <regex>

using namespace tinyxml2;

//错误检查
inline 
void errorCheck(int flag, int really, const char * msg)
{
    if(flag != really)
    {
        perror(msg);
        return ;
    }
}

PageLib::PageLib(Configuration *conf)
    : _conf(conf)
{
    auto p = _conf->getConfigMap().find("path_XMLLib");
    _dir(p->second);
    _files.reserve(1024);
}

void PageLib::create()
{
    std::cout << "开始建立网页库和网页偏移库" << std::endl;
    vector<string> dir = _dir.getFiles();
    size_t totalSize = 0;
    
    for(auto &file: dir){
        /* std::cout << "一篇" << std::endl; */
        readFile(file, totalSize);
    }
}

void PageLib::store()
{
    std::cout << "存储网页库和倒排索引库！" << std::endl;
    string ripePagePath = _conf->getConfigMap().find("path_newRipePage")->second;
    string offsetPagePath = _conf->getConfigMap().find("path_newOffset")->second;

    std::ofstream ripePage(ripePagePath);
    errorCheck(ripePage.is_open(), true, "open ripePage in PageLib");
    std::ofstream offsetPage(offsetPagePath);
    errorCheck(offsetPage.is_open(), true, "open offsetPage in PageLib");

    for(auto &p: _files){
        ripePage << p << "\n";
    }
    for(auto &p: _offsetLib){
        offsetPage << p.first << " " 
            << p.second.first << " "
            << p.second.second << "\n";
    }
}

//读一个XML文件
void PageLib::readFile(const string & xmlFile, size_t & totalSize)
{
    /* std::cout << xmlFile << std::endl; */
    //打开文件
    XMLDocument xmlDocument;
    XMLError error = xmlDocument.LoadFile(xmlFile.c_str());
    if(error != XML_SUCCESS) {
        std::cerr << "读取 xml 失败：" << xmlDocument.ErrorStr() << std::endl;
        return;
    }
    
    //找到item结点
    XMLElement* itemNode = xmlDocument.FirstChildElement("rss")
        ->FirstChildElement("channel")->FirstChildElement("item");
    if(!itemNode){
        std::cerr << "获取结点失败" << std::endl;
        return;
    }

    //存储一篇文章
    std::stringstream formateText;
    //循环遍历结点，获取结点信息
    while(itemNode){
        //重置stringstream的状态和缓冲区
        formateText.clear();
        formateText.str("");

        const XMLElement* elTitle = itemNode->FirstChildElement("title");
        const XMLElement* elLink = itemNode->FirstChildElement("link");
        const XMLElement* elContent = itemNode->FirstChildElement("content");
        const XMLElement* elDiscreption = itemNode->FirstChildElement("description");

        //做空判断！！！
        const char *pTitle = nullptr;
        const char *pLink = nullptr;
        const char *pContent = nullptr;
        if(elLink) pLink = elLink->GetText();
        //文章内容可能是content或者description的其中一种
        if(elContent) pContent = elContent->GetText();
        if(elDiscreption) pContent = elDiscreption->GetText();
        //如果没有标题，那就抽取第一行当作标题
        char titleTemp[256] = {0};
        if(elTitle){ 
            pTitle = elTitle->GetText();
        }
        else{
            //抽取第一行
            int idx = 0;
            while(pContent && pContent[idx++] != '\n');
            strncpy(titleTemp, pContent, idx);
            pTitle = titleTemp;
        }

        //做判空处理
        string title, link, description, content;
        if(pTitle)  title = pTitle;
        if(pLink)  link = pLink;
        if(pContent)  content = pContent;
        //去除其中的html标签
        std::regex reg("<[^>]+>");
        description = regex_replace(description, reg, "");
        content = regex_replace(content, reg, "");
        
        int id = _files.size() + 1;
        formateText << "<doc><docid>" << id
            << "</docid><url>" << link
            << "</url><title>" << title
            << "</title><content>" << title << content
            << "</content></doc>";

        //一篇文章输入到网页库
        _files.push_back(formateText.str());
        
        //输入到网页偏移库
        size_t pageSize = _files.back().size();
        pair<int, int> p1(totalSize, pageSize);
        totalSize += pageSize + 1;
        _offsetLib.insert(pair<int, pair<int, int>>(id, p1));

        itemNode = itemNode->NextSiblingElement("item");
    }
}

vector<string> & PageLib::getFiles()
{
    return _files;
}
