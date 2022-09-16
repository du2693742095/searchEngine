/***********************************************************
  > File Name: SocketIO.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月12日 星期一 09时37分40秒
  > Modified Time:2022年09月12日 星期一 09时37分40秒
 *******************************************************/
#include "../../include/SocketIO.hh"

SocketIO::SocketIO(int fd)
    : _fd(fd)
{}

int SocketIO::readn(char *info, int len)
{
    int left = len;
    char * pChar = info;

    while(left > 0){
        int ret = read(_fd, pChar, left);
        if(ret < 0 && errno == EINTR){
            continue;
        }else if(ret < 0){
            perror("read In SocketIO::readn");
            return -1;
        }else if(ret == 0){
            break; //连接断开的时候可以继续接收，但发送的时候就应该返回-1
        }else{
            left -= ret;
            pChar += ret;
        }
    }
    return len - left;
}

int SocketIO::readLine(char *info, int len)
{
    int left = len;
    char * pChar = info;
    int total = 0;
    while(left > 0){
        //一次性看一个MSG_LEN长度的数据，看里面有没有换行
        int ret = recv(_fd, pChar, left, MSG_PEEK);
        if(-1 == ret && errno == EINTR){
            continue;
        }else if(-1 == ret){
            perror("read In SocketIO::readn");
            return -1;
        }else if(0 == ret){
            break;
        }else{
            //遍历读取到的字符串，看有没有\n
            for(int i = 0; i < ret; ++i){
                if(pChar[i] == '\n'){
                    int idx = i + 1;
                    readn(pChar, idx);
                    pChar[idx] = '\0';
                    pChar += idx;
                    return total + idx;
                }
            }
            readn(pChar, ret);
            total += ret;
            pChar += ret;
            left -= ret;
        }
    }
    *pChar = '\0';
    return total - left;
}

int SocketIO::writen(const char *info, int len)
{
    int left = len;
    const char * pChar = info;

    while(left > 0){
        int ret = write(_fd, pChar, left);
        if(ret < 0 && errno == EINTR){
            continue;
        }else if(ret < 0){
            perror("read In SocketIO::readn");
            return -1;
        }else if(ret == 0){
            break; //连接断开的时候可以继续接收，但发送的时候就应该返回-1
        }else{
            left -= ret;
            pChar += ret;
        }
    }
    return len - left;
}

