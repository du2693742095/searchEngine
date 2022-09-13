/*************************************************************************
 @Author: dsy
 @Created Time : 2022年08月14日 星期日 18时27分04秒
 @File Name: RssItem.cpp
 @Description: dushiyang68@gmail.com
 ************************************************************************/
#include <iostream>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


int main() {
    
    XMLDocument xmlDocument;
    XMLError error = xmlDocument.LoadFile("coolshell.xml");
    if(error != XML_SUCCESS) {
        std::cout << "读取 xml 失败：" << xmlDocument.ErrorStr() << endl;
        return EXIT_FAILURE;
    }
    
    XMLElement* XMLFristNode = xmlDocument.FirstChildElement("rss")->
        FirstChildElement("channel")->FirstChildElement("item");
    XMLElement* XMLNode = XMLFristNode->FirstChildElement("title");
    if(XMLNode){
        cout << XMLNode->GetText() << endl;
    }else{
        cout << "获取结点失败" << endl;
    }

    XMLNode = XMLFristNode->FirstChildElement("link");
    if(XMLNode){
        cout << XMLNode->GetText() << endl;
    }else{
        cout << "获取结点失败" << endl;
    }

#if 0
    XMLNode = XMLFristNode->FirstChildElement("description");
    if(XMLNode){
        cout << XMLNode->GetText() << endl;
    }else{
        cout << "获取结点失败" << endl;
    }
#endif

    XMLFristNode = XMLFristNode->NextSiblingElement("item");
    cout << endl;
    XMLNode = XMLFristNode->FirstChildElement("title");
    if(XMLNode){
        cout << XMLNode->GetText() << endl;
    }else{
        cout << "获取结点失败" << endl;
    }

    return 0;
}
