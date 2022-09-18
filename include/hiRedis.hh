#ifndef __HIREDIS_H_
#define __HIREDIS_H_

#include "Configuration.hh"

#include <hiredis/hiredis.h>
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

class HiRedis
{
public:
    HiRedis(const string &, const int &);
    HiRedis(Configuration *);
    ~HiRedis();
    bool connect();
    bool set(const string &, const string &);
    string get(const string &);
private:
    void ReplyJuge(int);
private:
   redisContext * _pConnect;
   redisReply *   _pReply;
   Configuration* _conf;
   const string _ip;
   const int _port;
};

#endif
