/***********************************************************
  > File Name: test.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月15日 星期四 22时12分16秒
  > Modified Time:2022年09月15日 星期四 22时12分16秒
 *******************************************************/
#include <iostream>
#include "../../include/WebPage.hh"

using std::cout;
using std::endl;

void test0(){
    string page("<doc><docid>5</docid><url>/tjsj/sjjd/202105/t20210517_1817559.html</url><title>国家统计局新闻发言人就2021年4月份国民经济运行情况答记者问</title><content>国家统计局新闻发言人就2021年4月份国民经济运行情况答记者问</content></doc>");
    WebPage p(std::move(page)); 
    cout << endl;
    cout << p.getDocId() << endl
        << p.getTitle() << endl
        << p.getContent() << endl;

}

int main(){
    test0();
    return 0;
}

