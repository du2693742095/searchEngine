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
class PageLib
{
public:
    PageLib(Configuration &conf);
    void create();
    void store();

private:
    void readFile(const string &XMLFile, size_t &totalSize);

private:
    Configuration & _conf;
    DirScanner _dir;//存储了文件名
    vector<string> _files;//存储所有的网页库

    /* 网页偏移库由文章id+文章首地址+文章长度 */
    map<int, pair<int, int>> _offsetLib;//网页偏移库

};

#endif
