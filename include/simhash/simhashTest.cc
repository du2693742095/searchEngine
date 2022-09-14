/***********************************************************
  > File Name: a.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月14日 星期三 14时23分14秒
  > Modified Time:2022年09月14日 星期三 14时23分14秒
 *******************************************************/
#include "Simhasher.hpp"
#include <iostream>
#include <string>

using namespace std;
using namespace simhash;

void test()
{
    Simhasher simhasher("../../lib/dict/jieba.dict.utf8",
        "../../lib/dict/hmm_model.utf8",
        "../../lib/dict/idf.utf8",
        "../../lib/dict/stop_words.utf8");
    string s("我是蓝翔技工拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上总经理，出任CEO，走上人生巅峰。");
    string s2("我是蓝翔技工拖拉机学院suohiiduihiduih手扶业的。不用多久，我会升职加薪，当上总经理，出任CEO，走上人生巅峰。");
    string s3("我们私藏会说你的情况，你在加密领域亨利和埃的任务，我就会和你hi去，当上总经理，我是你妈的，走上况状放大来。");
    size_t topN = 5;
    uint64_t u64 = 0;
    uint64_t u64_2 = 0;
    uint64_t u64_3 = 0;

    vector<pair<string ,double> > res;
    /* simhasher.extract(s, res, topN); //提取关键词与权重 */

    simhasher.make(s, topN, u64); //
    simhasher.make(s2, topN, u64_2); //
    simhasher.make(s3, topN, u64_3); //
    
    cout<< "文本：\"" << s << "\"" << endl;
    cout<< "文本：\"" << s2 << "\"" << endl;
    cout<< "文本：\"" << s3 << "\"" << endl;
    /* cout << "关键词序列是: " << res << endl; */
    cout<< "simhash值是: " << u64 <<endl;
    cout<< "simhash值是: " << u64_2 <<endl;
    cout<< "simhash值是: " << u64_3 <<endl;

    /* const char * bin1 = "100010110110"; */
    /* const char * bin2 = "110001110011"; */
    uint64_t u1, u2;
    /* u1 = Simhasher::binaryStringToUint64(bin1); */
    /* u2 = Simhasher::binaryStringToUint64(bin2); */

    u1 = Simhasher::binaryStringToUint64(s.c_str());
    u2 = Simhasher::binaryStringToUint64(s3.c_str());

    cout << u1 << "和" << u2 << " simhash值的相等判断如下："<<endl;
    cout << "海明距离阈值默认设置为3，则isEqual结果为："
    << (Simhasher::isEqual(u1, u2)) << endl;
    cout << "海明距离阈值默认设置为5，则isEqual结果为："
    << (Simhasher::isEqual(u1, u2, 5)) << endl;
}

int main(){
    test();
    return 0;
}

