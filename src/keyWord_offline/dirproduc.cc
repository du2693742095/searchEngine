/***********************************************************
  > File Name: dirproduc.cpp
  > Author: Mou
  > Mail: 2429826959@qq.com
  > Created Time: Thu 15 Sep 2022 10:37:14 AM CST
  > Modified Time:Thu 15 Sep 2022 10:37:14 AM CST
 *******************************************************/

#include "../../include/DictProducer.hh"
#include "../../include/SplitToolCppJieba.hh"

using std::cin;
using std::cout;
using std::endl;

int main()
{
    const string confpath("../conf/serverConf.conf");
 
    Configuration *p = Configuration::getInstance(confpath);
    SplitToolCppJieba *sp = new SplitToolCppJieba(p);
    DirtProducer dicp(p,sp);
    dicp.buildDict_ZH();
    /* dicp.showDirt(); */
    dicp.storeDict();
    cout << "done" << endl;
}


