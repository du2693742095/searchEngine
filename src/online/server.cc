/***********************************************************
  > File Name: server.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月17日 星期六 20时57分11秒
  > Modified Time:2022年09月17日 星期六 20时57分11秒
 *******************************************************/
#include "../../include/EchoServer.hh"

int main(){
    EchoServer server("../conf/serverConf.conf");
    server.start();

    return 0;
}

