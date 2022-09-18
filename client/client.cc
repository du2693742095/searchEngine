#include "client.h"
#include "Json.hh"
#include <iostream>

using namespace std;

#define BUFFSIZE 128

int create_connect(int port,const char* ip){
    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=inet_addr(ip);
    connect(clientfd,(struct sockaddr*)&addr,sizeof(addr));
    return clientfd;
}

int main(int argc, char* argv[]){
    //检测参数个数
    ARGS_CHECK(argc, 3);
    //与服务器端建立连接
    int port=atoi(argv[2]);
    int clientfd=create_connect(port,argv[1]);
    //buf1用于从键盘读取指令

    while(1){
        int id = 0;
        string sentence;
        cin >> id >> sentence;

        //单词联想是1，网页查询是2
        if(id != 2 && id != 1)
        {
            cout << "指令错误，请重新输入" << endl;
            continue;
        }

        Msg msg;
        msg._ID = id;
        vector<string> temp;
        temp.push_back(sentence);
        msg._msg = temp;
        msg._msgLen = sizeof(sentence);

        json j;
        to_json(j, msg);
        string sendMsg = j.dump();

        //向服务端发送指令
        int ret = send(clientfd, sendMsg.c_str(), sendMsg.size(), 0);
        if(ret == 0)
        {
            puts("connect is broken!");
            exit(1);
        }
        ERROR_CHECK(ret, -1, "send");

        //收信息
        char buff[1024] = {0};
        ret = recv(clientfd, buff, sizeof(buff), 0);
        if(ret == 0)
        {
            puts("connect is broken!");
            exit(1);
        }
        ERROR_CHECK(ret, -1, "send");
        
        from_json(buff, msg);
        for(auto &w: msg._msg)
        {
            cout << w << endl;
        }

      }

    close(clientfd);
    return 0;
}

