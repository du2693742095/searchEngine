/***********************************************************
  > File Name: WebQueryTest.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月16日 星期五 20时37分15秒
  > Modified Time:2022年09月16日 星期五 20时37分15秒
 *******************************************************/
#include <iostream>
#include "../../include/WebQuery.hh"

using std::cout;
using std::endl;

void test0(){
    Configuration *conf = Configuration::getInstance("../../conf/serverConf.conf");
    SplitToolCppJieba jieba(conf);
    WebQuery w(jieba, conf);
    cout << w.doQuery("缓存") << endl;
}

int main(){
    test0();
    return 0;
}

