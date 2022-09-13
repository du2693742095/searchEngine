/***********************************************************
  > File Name: pageLibTest.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月13日 星期二 22时08分44秒
  > Modified Time:2022年09月13日 星期二 22时08分44秒
 *******************************************************/
#include <iostream>
#include "../../include/PageLib.hh"
#include "../../include/Configuration.hh"

using std::cout;
using std::endl;

void test0(){
    Configuration conf("../conf/serverConf.conf");
    PageLib p(conf);
    p.create();
    p.store();
}

int main(){
    test0();
    return 0;
}

