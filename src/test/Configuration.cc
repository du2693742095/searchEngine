/***********************************************************
  > File Name: Configuration.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月13日 星期二 11时15分25秒
  > Modified Time:2022年09月13日 星期二 11时15分25秒
 *******************************************************/
#include "../../include/Configuration.hh"
#include <fstream>
#include <iostream>

Configuration *Configuration::_pInstance = nullptr;

/* 单例模式 */
Configuration * Configuration::getInstance()
{
    if(_pInstance == nullptr){
        std::cerr << "配置文件还未初始化";
    }
    return _pInstance;
}

Configuration * Configuration::getInstance(const std::string & filePath)
{
    if(_pInstance == nullptr){
        _pInstance = new Configuration(filePath);
    }
    return _pInstance;
}

Configuration * Configuration::getInstance(std::string && filePath)
{
    if(_pInstance == nullptr){
        _pInstance = new Configuration(filePath);
    }
    return _pInstance;
}

void Configuration::destroyIns()
{
    if(_pInstance){
        delete _pInstance;
        _pInstance = nullptr;
    }
}

/* 其余函数 */
Configuration::Configuration(const std::string & filePath)
    : _filePath(filePath)
{
    createConf();
}

Configuration::Configuration(std::string && filePath)
    : _filePath(std::move(filePath))
{
    createConf();
}

std::map<std::string, std::string> & Configuration::getConfigMap()
{
    return _configMap;
}

std::set<std::string> & Configuration::getStopWordList_EN()
{
    return _stopWordList_EN;
}

std::set<std::string> & Configuration::getStopWordList_ZH()
{
    return _stopWordList_ZH;
}

//读取配置文件中的key
std::string getKey(const std::string &line)
{
    return std::string(line.begin(), line.begin() + line.find('='));
}

//读取配置文件中的value
std::string getValue(const std::string &line)
{
    return std::string(line.begin() + line.find('=') + 1, line.end());
}

void Configuration::createConf()
{
    /* std::cout << _filePath << std::endl; */
    std::ifstream ifs(_filePath);
    if(!ifs.is_open()){
        perror("open configuration file In webQuery");
    }

    std::string buff;
    buff.reserve(30);
    
    //循环读入一行数据，取出有效数据
    while(std::getline(ifs, buff)){
        //去除注释
        if(buff.size() == 0 || buff[0] == '#')
            continue;

        std::string key = getKey(buff);
        std::string value = getValue(buff);

        _configMap[key] = value;
    }
}

void Configuration::createStopWord_EN()
{
    auto p = _configMap.find("path_stop_words_eng");
    if(p == _configMap.end()){
        std::cerr << "path_stop_words_eng is no exits.";
        return ;
    }
    std::ifstream ifs(p->second);
    
    std::string buff;
    while(std::getline(ifs, buff)){
        _stopWordList_EN.insert(buff);
    }
}

void Configuration::createStopWord_ZH()
{
    auto p = _configMap.find("path_stop_words_zh");
    if(p == _configMap.end()){
        std::cerr << "path_stop_words_eng is no exits.";
        return ;
    }
    std::ifstream ifs(p->second);
    
    std::string buff;
    while(std::getline(ifs, buff)){
        _stopWordList_ZH.insert(buff);
    }
}
