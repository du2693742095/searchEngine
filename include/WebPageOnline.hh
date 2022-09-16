/***********************************************************
  > File Name: WebPageOnline.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月16日 星期五 11时08分53秒
  > Modified Time:2022年09月16日 星期五 11时08分53秒
 *******************************************************/
#ifndef __WEBPAGEONLInE_HH__
#define __WEBPAGEONLInE_HH__

#include <string>
#include <map>
#include <vector>
#include <fstream>

using std::string;
using std::map;
using std::vector;
using std::pair;

class WebPageOnlie
{
public:
    WebPageOnlie();
    WebPageOnlie(const string & doc);
    size_t getDocId();
    string & getTitle();
    string & getUrl();
    string & getSummery(const vector<string> &queryWord);
    
    //对文档的id进行比较
    friend bool operator < (const WebPageOnlie &lhs, const WebPageOnlie &rhs);

private:
    void cut(const string &_doc);

private:
    size_t _docId;
    string _docTitle;
    string _docUrl;
    string _docContent;
    string _docSummery;
    /* vector<string> _topWords;//词频最高的20个词（用topK） */
    /* map<string, int> _wordsMap; //保存所有词语和词频，不包括停用词 */

};

#endif
