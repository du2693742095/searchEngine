/***********************************************************
  > File Name: PageLib.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月13日 星期二 15时05分52秒
  > Modified Time:2022年09月13日 星期二 15时05分52秒
 *******************************************************/
#ifndef __PAGELIB_HH__
#define __PAGELIB_HH__

#include "Configuration.hh"
#include "DirScanner.hh"
#include <string>
#include <map>
#include <vector>
#include <fstream>

using std::string;
using std::map;
using std::vector;
using std::pair;

/* 用来生成网页库和位置偏移库 */
/* 如果没有标题，则抽取第一行内容作为标题 
 * 将标题也放入内容中，一起变成一篇文章 */
class PageLib
{
public:
    PageLib(Configuration *conf);
    void create();
    void store();
    vector<string> & getFiles();

private:
    void readFile(const string &XMLFile, size_t &totalSize);

private:
    Configuration * _conf;
    DirScanner _dir;//存储了文件名
    vector<string> _files;//存储所有的网页库

    /* 网页偏移库由文章id+文章首地址+文章长度 */
    map<int, pair<int, int>> _offsetLib;//网页偏移库

};

#endif
