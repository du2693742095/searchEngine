/***********************************************************
  > File Name: simhustest.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月14日 星期三 19时19分27秒
  > Modified Time:2022年09月14日 星期三 19时19分27秒
 *******************************************************/
#include <iostream>
#include "../../include/Configuration.hh"
#include "../../include/SimhashUseToSplite.hh"

using std::cout;
using std::endl;

void test0(){
    SimHashUseToSplite s(Configuration::getInstance("../conf/serverConf.conf"));
    
}

int main(){
    test0();
    return 0;
}

