/*************************************************************************
 @Author: dsy
 @Created Time : 2022年08月15日 星期一 18时24分50秒
 @File Name: main.cpp
 @Description: dushiyang68@gmail.com
 ************************************************************************/
#include "TextQuery.hh"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;

void test()
{
    std::ifstream is("test.txt");
    TextQuery bash(is);

    Query q = Query("you") & Query("are");
    cout << q.eval(bash) << endl;
    
    q = ~Query("i");
    cout << q.eval(bash) << endl;
    
    q = Query("love") | Query("you");
    cout << q.eval(bash) << endl;
}


int main()
{
    test();
    return 0;
}

