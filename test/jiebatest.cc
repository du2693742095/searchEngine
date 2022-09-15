/***********************************************************
  > File Name: a.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月15日 星期四 11时40分18秒
  > Modified Time:2022年09月15日 星期四 11时40分18秒
 *******************************************************/
#include <iostream>
#include "../../include/simhash/cppjieba/Jieba.hpp"

using std::cout;
using std::endl;
using namespace cppjieba;

void test()
{
    cppjieba::Jieba jieba("../../lib/dict/jieba.dict.utf8",
                          "../../lib/dict/hmm_model.utf8",
                          "../../lib/dict/user.dict.utf8");
    vector<string> words;
    string s;
    string result;
    s = "他来到了网易杭研大厦hello kitty";
    cout << "[demo] Cut With HMM" << endl;
    jieba.Cut(s, words, true);
    for(auto &path : words){
        cout << path << endl;
    }
}
int main(){
    test();
    return 0;
}

