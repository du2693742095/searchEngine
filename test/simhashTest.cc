/***********************************************************
  > File Name: a.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月15日 星期四 10时14分57秒
  > Modified Time:2022年09月15日 星期四 10时14分57秒
 *******************************************************/
#include <iostream>
/* #include "../../include/simhash/Simhasher.hpp" */
#include "../../include/SimhashUseToSplite.hh"
#include "../../include/Configuration.hh"
#include <vector>

using namespace std;
using namespace simhash;

void test0(){
    auto _conf = Configuration::getInstance("../../conf/serverConf.conf");
    /* auto _path = _conf->getConfigMap(); */
    /* cout << _path.find("path_jieba")->second << endl; */
    /* cout << _path.find("path_hmm_model")->second << endl; */
    /* cout << _path.find("path_idf")->second << endl; */
    /* cout << _path.find("path_stop_words")->second << endl; */

    /* Simhasher _simhasher(_path.find("path_jieba")->second, */
    /*                _path.find("path_hmm_model")->second, */
    /*                _path.find("path_idf")->second, */
    /*                _path.find("path_stop_words")->second); */
    /* Simhasher *sim; */

    /* Simhasher _simhasher("../../lib/dict/jieba.dict.utf8", */
    /*                "../../lib/dict/hmm_model.utf8", */
    /*                "../../lib/dict/idf.utf8", */
    /*                "../../lib/dict/stop_words.utf8"); */

    SimHashUseToSplite sim(_conf);    

    cout << "初始化" << endl;

    vector<string> _pageLib;
    _pageLib.push_back("nihao,woshinimama，我不知道你在说啥子哦");
    _pageLib.push_back("nihao,woshinimama，我不知道你在说啥子哦");
    _pageLib.push_back("wodiaonitamade, nisuosazi o，我们和平共处好不好？");

    for(auto &page: _pageLib){
        cout << page << endl;
    }
    cout << endl;

    for(auto page = _pageLib.begin(); page != _pageLib.end(); ++page){
        if(sim.isRepeat(*page)){
            //如果重复了，就置空
            *page = string();
        }
    }
    for(auto &page: _pageLib){
        cout << page << endl;
    }

}

int main(){
    test0();
    return 0;
}

