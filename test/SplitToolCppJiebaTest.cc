/***********************************************************
  > File Name: SplitToolCppJiebaTest.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月15日 星期四 20时44分02秒
  > Modified Time:2022年09月15日 星期四 20时44分02秒
 *******************************************************/
#include <iostream>
#include "../../include/SplitToolCppJieba.hh"
#include "../../include/Configuration.hh"

using std::cout;
using std::endl;

void test0(){
    Configuration *conf = Configuration::getInstance("../../conf/serverConf.conf");
    SplitToolCppJieba jieba(conf);
    std::string a("?, nihao 我是你妈妈");
    auto p = jieba.cut(a);
    for(auto path: p){
        cout << path << endl;
    }
    cout << jieba.isStopWord("?") << endl;
    
}

int main(){
    test0();
    return 0;
}

