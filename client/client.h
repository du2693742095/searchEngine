#ifndef __client_H__
#define __client_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<grp.h>
#include<pwd.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<sys/mman.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<sys/uio.h>
#include<time.h>
#include<dirent.h>
#include<pthread.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>
#include<shadow.h>
#include<crypt.h>
#include<features.h>


#define PTHREAD_ERROR_CHECK(a, b)\
    do{\
        if(a != b){\
            fprintf(stderr,"%s:%s\n","ret",strerror(ret)); \
        }\
    }while(0)

#define SIZE(a) (sizeof(a)/sizeof(a[0]))

#define ARGS_CHECK(argc,nums) \
	 do{\
         if(argc!=nums){\
		    fprintf(stderr,"Invalid arguments: expected %d arguments\n",nums);\
	 	    exit(1);\
	    }\
       }while(0)


#define ERROR_CHECK(val, expectedval, msg) \
	do{\
        if(val == expectedval){\
            perror(msg);\
            exit(1);\
        }\
      }while(0)


#define printf_light_blue(fmt, args...) \
            printf("\e[1;36m" fmt "\e[0m", ## args)
#define printf_green(fmt, args...) \
            printf("\e[1;32m" fmt "\e[0m", ## args)
#define printf_purple(fmt, args...) \
            printf("\e[1;35m" fmt "\e[0m", ## args)


#endif
