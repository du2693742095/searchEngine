#include "../include/func.h"

int clientTcpInit(char* IP, int port){                                          

    int clientfd=socket(AF_INET, SOCK_STREAM, 0);                               
    ERROR_CHECK(clientfd, -1, "socket");                                        

    struct sockaddr_in serverAddr;                                              
    memset(&serverAddr, 0, sizeof(serverAddr));                                 
    serverAddr.sin_family = AF_INET;                                            
    serverAddr.sin_port = htons(port);                                          
    serverAddr.sin_addr.s_addr = inet_addr(IP);                                 

    int ret;                                                                    
    ret=connect(clientfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));   
    ERROR_CHECK(ret, -1, "connect");                                            

    return clientfd;                                                            
}                                                                               

//分析指令，打包成协议如果返回NULL，则说明出错
cmd_t *analyzeCmd(char *cmd, size_t cmdSize)
{
    char instruction[10] = {0};     //暂存指令
    cmd_t *cmdBuff = (cmd_t *)calloc(1, sizeof(cmd_t));

    //获取指令
    size_t position = 0;
    for( ; cmd[position] != ' ' && position <  cmdSize && cmd[position] != '\n'; ++position){
        instruction[position] = cmd[position];
    }
    if(!strcmp(instruction, "exit")) cmdBuff->cmd = CMD_TYPE_EXIT;
    else if(!strcmp(instruction, "cd")) cmdBuff->cmd =CMD_TYPE_CD;
    else if(!strcmp(instruction, "mv")) cmdBuff->cmd =CMD_TYPE_MV;
    else if(!strcmp(instruction, "cp")) cmdBuff->cmd =CMD_TYPE_CP;
    else if(!strcmp(instruction, "ls")) cmdBuff->cmd =CMD_TYPE_LS;
    else if(!strcmp(instruction, "ll")) cmdBuff->cmd =CMD_TYPE_LL;
    else if(!strcmp(instruction, "pwd")) cmdBuff->cmd =CMD_TYPE_PWD;
    else if(!strcmp(instruction, "puts")) cmdBuff->cmd =CMD_TYPE_PUTS;
    else if(!strcmp(instruction, "gets")) cmdBuff->cmd =CMD_TYPE_GETS;
    else if(!strcmp(instruction, "rm")) cmdBuff->cmd =CMD_TYPE_RM;
    else if(!strcmp(instruction, "mkdir")) cmdBuff->cmd = CMD_TYPE_MKDIR;
    //输入错误，返回空
    else { printf("Error arguments.\n"); free(cmdBuff); return NULL; }

    //获取参数
    //忽略空格和结尾
    while(cmd[position] == ' ' && position < cmdSize){
        position++;
    }
    //查看是否有参数
    if(position >= cmdSize){
        cmdBuff->argSize = 0;
        return cmdBuff;
    }else{
        char argv_1[124] = {0};
        char argv_2[124] = {0};
        sscanf(cmd + position, "%s %s", argv_1, argv_2);
        //查看拆分后的数据里面是否有效，统计参数个数
        if(strlen(argv_1) > 0){
            cmdBuff->argSize++;
            cmdBuff->args[0].argLen = strlen(argv_1);
            strcpy(cmdBuff->args[0].arg, argv_1);
        }
        if(strlen(argv_2) > 0){
            cmdBuff->argSize++;
            cmdBuff->args[1].argLen = strlen(argv_2);
            strcpy(cmdBuff->args[1].arg, argv_2);
        }
    }

    return cmdBuff;
}


int recvCriclefile(int fd, char *buff, int length){
    //lenght为想接收的长度 
    int index = length;
    char *pbuff = buff;
    int ret = 0;
    while(index > 0){
        ret = recv(fd, pbuff, index, MSG_WAITALL);
        if(ret == 0){
            printf("connect broken\n");
            close(fd);
            break;
        }else if(ret == -1 && errno == EINTR){
            continue;
        }else if(ret == -1){
            perror("recv");
            break;
        }else{
            //index表示剩余未接收的长度
            index -= ret; 
            pbuff += ret;
        }
    }
    return ret;
}
void handleOrder(int clientfd){
    int size = 0;
    int ret = 0;
    ret = recv(clientfd, &size, 4, MSG_WAITALL);
    if(ret == 0)
    {
        puts("connect is broken!");
        exit(1);
    }
    char recvbuff[4096] = {0};
    recvCriclefile(clientfd, recvbuff, size);//train body
    printf("%s\n",recvbuff);
}

void handlePuts(int clientfd, char* filename){
    //打开要发送文件
    int fd = open(filename, O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    //发送文件名
    train_t train;
    memset(&train, 0, sizeof(train));
    train.length = strlen(filename);
    strcpy(train.buff, filename);
    int ret = send(clientfd, &train, 4+train.length, MSG_WAITALL);
    ERROR_CHECK(ret, -1, "send");
    //发送文件内容
    struct stat st;
    fstat(fd, &st);
    ret = send(clientfd, &st.st_size, 8, MSG_WAITALL);
    ERROR_CHECK(ret, -1, "send");
    size_t sendSize = 0;
    while(sendSize < st.st_size){
        memset(&train, 0, sizeof(train));
        int readlen = read(fd, train.buff, sizeof(train.buff));
        train.length = readlen;
        ret = send(clientfd, &train, 4+train.length, MSG_WAITALL);
        ERROR_CHECK(ret, -1, "send");
        sendSize += readlen;
    }
    printf("file:%s has sent successfully\n", filename);
}

void handleGets(int clientfd){
    //接收文件名
    char filename[32] = {0};
    int filenamelen = 0;
    recv(clientfd, &filenamelen, 4, MSG_WAITALL);
    printf("filenamelen=%d\n", filenamelen);
    recv(clientfd, &filename, filenamelen, MSG_WAITALL);
    printf("filename:%s\n", filename);

    int fd = open(filename, O_RDWR|O_CREAT, 0664);
    //接收文件内容
    size_t filelen = 0;
    recv(clientfd, &filelen, 8, MSG_WAITALL);
    printf("filecontent length=%ld\n", filelen);

    size_t recvSize = 0;
    while(recvSize < filelen){
        char buff[1000] = {0};
        int buflen = 0;
        recv(clientfd, &buflen, 4, MSG_WAITALL);
        recv(clientfd, buff, buflen, MSG_WAITALL);
        write(fd, buff, buflen);
        recvSize += buflen;
    }
}

int cd_client(int clientfd)
{
    int backret=99;                                                      
    int ret=recv(clientfd,&backret,4,0);
    if(ret == 0)
    {
        puts("connect is broken!");
        exit(1);
    }
    if(backret==0)
    {
        printf("cd succeed!\n");
        return 0;
    }
    else 
    {
        printf("cd failed!\n");
        return -1;
    }
}

int mv_client(int clientfd)
{
    int length;
    int ret = recv(clientfd,&length,4,MSG_WAITALL);
    if(ret == 0)
    {
        puts("connect is broken!");
        exit(1);
    }
    if(length==66)
    {
        printf("mv failed!\n");
        return -1;
    }
    if(length==99)
    {
        printf("mv succeed!\n");
        return 0;
    }

}

int cp_client(int clientfd)
{
    int length;
    int ret = recv(clientfd,&length,4,MSG_WAITALL);
    if(ret == 0)
    {
        puts("connect is broken!");
        exit(1);
    }
    if(length==-1)
    {
        printf("cp failed!\n");
        return -1;
    }
    printf("cp succeed!\n");
    return 0;
}

int rm_client(int clientfd)
{
    int length;
    int ret = recv(clientfd,&length,4,MSG_WAITALL);
    if(ret == 0)
    {
        puts("connect is broken!");
        exit(1);
    }
    if(length==-1)
    {
        printf("rm failed!\n");
        return -1;
    }
    printf("rm succeed!\n");
    return 0;
}

int pwd_client(int clientfd){
    int size = 0;
    int ret = 0;
    ret = recv(clientfd, &size, 4, MSG_WAITALL);
    if(ret == 0)
    {
        puts("connect is broken!");
        exit(1);
    }
    char recvbuff[4096] = {0};
    recv(clientfd, recvbuff, size, MSG_WAITALL);
    printf("%s\n",recvbuff);
}

int getsfileplus_client(int peerfd,const char* pthname)
{
    int fds[2];
    size_t ret;
    pipe(fds);
    size_t hasrecv=0;
    size_t percentOne =0;
    size_t lastSize = 0;
    size_t filelength = 0;
    int fd = open(pthname, O_WRONLY); 
    if(fd==-1){
        ret=send(peerfd,&hasrecv,8,MSG_WAITALL);
        if(ret == 0)
        {
            puts("connect is broken!");
            exit(1);
        }
        if(ret!=8) return ret;
        fd = open(pthname, O_WRONLY|O_CREAT, 0666); 
        if(fd==-1) return -1;
        ret=recv(peerfd,&filelength, 8,MSG_WAITALL);
        if(ret == 0)
        {
            puts("connect is broken!");
            exit(1);
        }
        if(ret!=8) return ret;
        printf("%s need download length: %ld\n",pthname,filelength);
    }else{
        struct stat statbuf;
        ret=stat(pthname,&statbuf);
        hasrecv=statbuf.st_size;
        ret=send(peerfd,&hasrecv,8,MSG_WAITALL);
        if(ret == 0)
        {
            puts("connect is broken!");
            exit(1);
        }
        if(ret!=8) return ret;
        lseek(fd,hasrecv,SEEK_SET);
        ret=recv(peerfd,&filelength, 8,MSG_WAITALL);
        if(ret == 0)
        {
            puts("connect is broken!");
            exit(1);
        }
        if(ret!=8) return ret;
        printf("recv/ret=%ld\n",ret);
        printf("%s need download length: %ld\n",pthname,filelength);
    }
    size_t recvSize=0;
    percentOne = filelength / 100;
    printf("fd=%d\n",fd);
    while(recvSize < filelength) {
        size_t lastRecv=filelength-recvSize;
        if(lastRecv<4096){
            ret=splice(peerfd,NULL,fds[1],NULL,lastRecv,SPLICE_F_MORE);
            ret=splice(fds[0],NULL,fd,NULL,ret,SPLICE_F_MORE);        
            if(ret<=0) return ret;
        }else{
            ret=splice(peerfd,NULL,fds[1],NULL,4096,SPLICE_F_MORE);
            ret=splice(fds[0],NULL,fd,NULL,ret,SPLICE_F_MORE);        
            if (ret<=0) return ret;
        }
        recvSize += ret;
        if(recvSize - lastSize > percentOne) {
            printf("has download %5.2f%%\r", (double)recvSize / filelength * 100);
            fflush(stdout);
            lastSize = recvSize;
        }
    }
    printf("%s has complete 100.00%%\n",pthname);
}
