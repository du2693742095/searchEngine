/*************************************************************************
 @Author: dsy
 @Created Time : 2022年08月06日 星期六 20时42分57秒
 @File Name: main.cc
 @Description: dushiyang68@gmail.com
 ************************************************************************/
#include "RecordBy_log4cpp.hh"

using namespace Log4cpp;

void test(void)
{
    Mylogger *log = Mylogger::getInstance("log4cppConfig.conf");
    char a[] = "wuhan";
    int b = 10;
    char c[] = "chendu";
    Mylogger::logserveremerg("hello %s %d %s", a, b, c);
}

int main(void)
{
    test();
    
    return 0;
}
