/***********************************************************
  > File Name: a.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月15日 星期四 16时29分15秒
  > Modified Time:2022年09月15日 星期四 16时29分15秒
 *******************************************************/
#include <iostream>
#include "../../include/PageLibPreprocessor.hh"
#include "../../include/Configuration.hh"

void test0(){
    Configuration *conf = Configuration::getInstance("../conf/serverConf.conf");
    PageLibPreProcessor page(conf);
    page.doProcess();
    /* page.store(); */
}

int main(){
    test0();
    return 0;
}

