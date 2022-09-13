/***********************************************************
  > File Name: dirtest.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月13日 星期二 16时35分15秒
  > Modified Time:2022年09月13日 星期二 16时35分15秒
 *******************************************************/
#include <iostream>
#include "../../include/DirScanner.hh"

using std::cout;
using std::endl;

void test0(){
    DirScanner dir;
    dir("../../include");
    dir.print();
}

int main(){
    test0();
    return 0;
}

