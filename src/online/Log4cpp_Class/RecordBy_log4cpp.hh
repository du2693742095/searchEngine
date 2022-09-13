/*************************************************************************
 @Author: dsy
 @Created Time : 2022年08月05日 星期五 22时44分57秒
 @File Name: Log4cpp.hh
 @Description: dushiyang68@gmail.com
 ************************************************************************/
#include <iostream>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Category.hh>

namespace Log4cpp{
//Mylogger.cc 使用单例模式
class Mylogger
{
public:
    //只写服务器日志
    void emerg(const std::string & msg);
    void alert(const std::string & msg);
    void crit(const std::string & msg);
    void error(const std::string & msg);
    void warn(const std::string & msg);
    void notice(const std::string & msg);
    void info(const std::string & msg);
    void debug(const std::string & msg);

    //模板实现
    template<class... ARGS>
    void emerg(const std::string & msg, ARGS... args);
    template<class... ARGS>
    void alert(const std::string & msg, ARGS... args);
    template<class... ARGS>
    void crit(const std::string & msg, ARGS... args);
    template<class... ARGS>
    void error(const std::string & msg, ARGS... args);
    template<class... ARGS>
    void warn(const std::string & msg, ARGS... args);
    template<class... ARGS>
    void notice(const std::string & msg, ARGS... args);
    template<class... ARGS>
    void info(const std::string & msg, ARGS... args);
    template<class... ARGS>
    void debug(const std::string & msg, ARGS... args);

    static Mylogger * getInstance(void);
    static Mylogger * getInstance(const char *conf);
    static void destroyInstance(void);
    
private:
	Mylogger()
        : _server(log4cpp::Category::getInstance(std::string("serverInfo")))
        , _client(log4cpp::Category::getInstance(std::string("clientInfo"))){}
	~Mylogger(){};
    Mylogger(const Mylogger &) = delete;
	Mylogger * operator=(const Mylogger &) = delete ;

private:
    static Mylogger *_log;
    log4cpp::Category & _server;
    log4cpp::Category & _client;
};

}//namespace


/* =======================定义函数=======================*/

namespace Log4cpp
{
//初始化static对象
Mylogger * Mylogger::_log = nullptr;

//获取一个单例对象(初始化后写入日志用)
Mylogger * Mylogger::getInstance()
{
    if(nullptr == _log){
        std::cerr << "log4cpp is not init, pls use getInstance(const char * conf) first.";
        exit(1);
    }else{
        return _log;
    }
}

//获取一个单例对象(用配置文件初始化Mylogger时用)
Mylogger * Mylogger::getInstance(const char * conf)
{
    if(nullptr == _log){
        log4cpp::PropertyConfigurator::configure(conf);
        _log = new Mylogger();
    }
    return _log;
}

//销毁log4cpp记录
void Mylogger::destroyInstance(void)
{
    if(_log != nullptr){
        log4cpp::Category::shutdown();
        delete _log;
    }
}

//记录不同等级信号的函数
void Mylogger::emerg(const std::string & msg)
{
    _server.emerg(msg);
}

void Mylogger::alert(const std::string & msg)
{
    _server.alert(msg);
}

void Mylogger::crit(const std::string & msg)
{
    _server.crit(msg);
}

void Mylogger::error(const std::string & msg)
{
    _server.error(msg);
}

void Mylogger::warn(const std::string & msg)
{
    _server.warn(msg);
}

void Mylogger::notice(const std::string & msg)
{
    _server.notice(msg);
}

void Mylogger::info(const std::string & msg)
{
    _server.info(msg);
}

void Mylogger::debug(const std::string & msg)
{
    _server.debug(msg);
}


/*==================模板实现======================*/
//拆分格式化字符串
template<class... ARGS>
std::string spliteString(const std::string & msg, ARGS... args)
{
    constexpr size_t oldlen = 124;
    char buffer[oldlen];  // 默认栈上的缓冲区

    size_t newlen = snprintf(&buffer[0], oldlen, msg.c_str(), args...);
    ++newlen;  // 算上终止符'\0'

    if (newlen > oldlen) {  // 默认缓冲区不够大，从堆上分配
        std::vector<char> newbuffer(newlen);
        snprintf(newbuffer.data(), newlen, msg.c_str(), args...);
        return std::string(newbuffer.data());
    }

    return buffer;
}

//标记函数
template<class... ARGS>
void Mylogger::emerg(const std::string & msg, ARGS... args)
{
    _server.emerg(spliteString(msg, args...));
}

template<class... ARGS>
void Mylogger::alert(const std::string & msg, ARGS... args)
{
    _server.emerg(spliteString(msg, args...));
}

template<class... ARGS>
void Mylogger::crit(const std::string & msg, ARGS... args)
{
    _server.emerg(spliteString(msg, args...));
}

template<class... ARGS>
void Mylogger::error(const std::string & msg, ARGS... args)
{
    _server.emerg(spliteString(msg, args...));
}

template<class... ARGS>
void Mylogger::warn(const std::string & msg, ARGS... args)
{
    _server.emerg(spliteString(msg, args...));
}

template<class... ARGS>
void Mylogger::notice(const std::string & msg, ARGS... args)
{
    _server.emerg(spliteString(msg, args...));
}

template<class... ARGS>
void Mylogger::info(const std::string & msg, ARGS... args)
{
    _server.emerg(spliteString(msg, args...));
}

template<class... ARGS>
void Mylogger::debug(const std::string & msg, ARGS... args)
{
    _server.emerg(spliteString(msg, args...));
}


/*================将函数封装成宏=======================*/
#define logInit(config) Mylogger::getInstance(config);

//在输入日志信息时就把文件名、函数名、行号一起加进去
#define getloginfo(msg) \
    (std::string(__FILE__).append(":").append(__FUNCTION__).append(":").append(std::to_string(__LINE__)).append(":").append(msg))

//##__va_args__红前面加##的含义是保证前面的参数...是0时，能将前面的...忽略掉
#define logserveremerg(msg, ...) Mylogger::getInstance()->emerg(getloginfo(msg), ##__VA_ARGS__);
#define logserveralert(msg, ...) Mylogger::getInstance()->alert(getloginfo(msg), ##__VA_ARGS__);
#define logservererror(msg, ...) Mylogger::getInstance()->error(getloginfo(msg), ##__VA_ARGS__);
#define logserverwarn(msg, ...) Mylogger::getInstance()->warn(getloginfo(msg), ##__VA_ARGS__);
#define logservernotice(msg, ...) Mylogger::getInstance()->notice(getloginfo(msg), ##__VA_ARGS__);
#define logserverinfo(msg, ...) Mylogger::getInstance()->info(getloginfo(msg), ##__VA_ARGS__);
#define logserverdebug(msg, ...) Mylogger::getInstance()->debug(getloginfo(msg), ##__VA_ARGS__);


}//end of Mylogger

