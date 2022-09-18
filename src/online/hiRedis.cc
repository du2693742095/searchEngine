#include "../../include/hiRedis.hh"

//测试用
HiRedis::HiRedis(const string & ip, const int & port)
:_ip(ip)
,_port(port)
{
    connect();
}

HiRedis::HiRedis(Configuration * conf)
:_conf(conf)
,_ip(_conf->getConfigMap().find("ip_redis")->second)
,_port(stoi(_conf->getConfigMap().find("port_redis")->second))
{
    connect();
}

HiRedis::~HiRedis()
{
    if(_pReply){
        freeReplyObject(_pReply);
    }
    if(_pConnect){
        redisFree(_pConnect);
    }
    _pReply = nullptr;
    _pConnect = nullptr;
}

bool HiRedis::connect()
{
    _pConnect = redisConnect(_ip.c_str(), _port);
    if(_pConnect == nullptr || _pConnect->err)
    {
        if(_pConnect)
        {
            cerr << "error :" << _pConnect->errstr;
            redisFree(_pConnect);
        }
        else
        {
            cout << "can not allocate redis context";
        }
        return false;
    }
    return true;
}

//string -> string 后期按需重载 
bool HiRedis::set(const string & key, const string & value)
{
    _pReply = (redisReply *)redisCommand(_pConnect, "set %s %s", key.c_str(),  value.c_str());
    //redis操作失败出错
    if(_pReply == nullptr)
    {
        redisFree(_pConnect);
        connect();
        return false;
    }

    freeReplyObject(_pReply);
    _pReply = nullptr;
    return true;
}

string HiRedis::get(const string & key)
{
    _pReply = (redisReply *)redisCommand(_pConnect, "get %s", key.c_str());
    if(_pReply == nullptr)
    {
        redisFree(_pConnect);
        connect();
        return string();
    }

    //没查找到
    if(_pReply->type == REDIS_REPLY_NIL)
    {
        freeReplyObject(_pReply);
        _pReply = nullptr;
        return string();
    }

    string ret = _pReply->str;
    
    freeReplyObject(_pReply);
    _pReply = nullptr;
    
    return ret;   //看情况修改
}

//测试用
void HiRedis::ReplyJuge(int type){
    bool res = false;
    switch(type)
    {
        case REDIS_REPLY_STRING :
            cout << "get value(string) :" << "\n" << _pReply->str << endl;
            break;
        case REDIS_REPLY_ARRAY :
            cout << "get value(array) :" << endl;
            for(size_t i = 0; i < _pReply->elements; ++i)
            {cout << _pReply->element[i]; }
            cout << endl;
            break;
        case REDIS_REPLY_INTEGER :
            cout << "get value(int) :" << _pReply->integer << endl;
            break;
        case REDIS_REPLY_NIL :
            cout << "Nil Result" << endl;
            break;
        case REDIS_REPLY_STATUS :
            cout << "Reply Status:" << _pReply->str << endl;
            break;
        case REDIS_REPLY_ERROR :
            cout << "Error :" << _pReply->str << endl;
            res = false;
            break;
    }

    if(res)
        exit(-1);
}

