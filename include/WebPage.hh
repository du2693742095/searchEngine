/***********************************************************
  > File Name: WebPage.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月14日 星期三 19时34分03秒
  > Modified Time:2022年09月14日 星期三 19时34分03秒
 *******************************************************/
#ifndef __WEBPAGE_HH__
#define __WEBPAGE_HH__

#include "Configuration.hh"
#include <string>
#include <map>
#include <vector>
#include <fstream>

using std::string;
using std::map;
using std::vector;
using std::pair;

//
class WebPage
{
public:
    WebPage(const string & doc);
    WebPage(const string && doc);
    size_t getDocId();
    string & getTitle();
    string & getContent();
    
    //对文档的id进行比较
    friend bool operator < (const WebPage &lhs, const WebPage &rhs);

private:
    size_t _docId;
    string _doc;//存储整篇文章，包括xml在内
    string _docTitle;
    string _docUrl;
    string _docContent;
    /* vector<string> _topWords; */
    /* map<string, int> _wordsMap; //保存所有词语和词频，不包括停用词 */

};

#endif
