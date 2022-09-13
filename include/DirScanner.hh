/***********************************************************
  > File Name: DirScanner.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月13日 星期二 15时07分38秒
  > Modified Time:2022年09月13日 星期二 15时07分38秒
 *******************************************************/
#ifndef __DIRSCANNER_HH__
#define __DIRSCANNER_HH__

#include <string>
#include <map>
#include <set>
#include <vector>

using std::string;
using std::map;
using std::set;
using std::vector;


class DirScanner
{
public:
    DirScanner();
    ~DirScanner();
    vector<string> operator()(const string & dirName);//用来调用tranvers函数
    void print();
    vector<string> getFiles();

private:
    void traverse(const string & dirName);//获取地址下的所有文件

private:
    vector<string> _files;//文件的绝对路径名（用来返回）
};

#endif
