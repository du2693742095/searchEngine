/***********************************************************
  > File Name: WebQuery_offline.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月16日 星期五 09时20分24秒
  > Modified Time:2022年09月16日 星期五 09时20分24秒
 *******************************************************/
#include <iostream>
#include "../../include/Configuration.hh"
#include "../../include/PageLib.hh"
#include "../../include/PageLibPreprocessor.hh"

using std::cout;
using std::endl;

int main(){
    Configuration::getInstance("../conf/serverConf.conf");
    //创建网页库和网页偏移库
    PageLib page(Configuration::getInstance());
    page.create();
    page.store();

    //创建倒排索引库
    PageLibPreProcessor pageLib(Configuration::getInstance());
    pageLib.doProcess();
    pageLib.store();

    return 0;
}

