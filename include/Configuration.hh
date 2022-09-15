/***********************************************************
  > File Name: Configuration.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月13日 星期二 11时11分12秒
  > Modified Time:2022年09月13日 星期二 11时11分12秒
 *******************************************************/
#ifndef __CONFIGURATION_HH__
#define __CONFIGURATION_HH__

#include <string>
#include <map>
#include <set>

class Configuration
{
public:
    static Configuration * getInstance();
    static Configuration * getInstance(const std::string & filePath);
    static Configuration * getInstance(const std::string && filePath);
    static void destroyIns();

    std::map<std::string, std::string> & getConfigMap();
    std::set<std::string> & getStopWordList_EN();
    std::set<std::string> & getStopWordList_ZH();
    void createStopWord_EN(); //只在离线版词频中用
    void createStopWord_ZH(); //只在离线版词频中用

private:
    Configuration(const std::string & filePath);
    Configuration(const std::string && filePath);
    ~Configuration(){}

    Configuration(const Configuration &) = delete ;
    Configuration & operator=(const Configuration &) = delete ;

    void createConf(); //参数的个数就是map的大小

private:
    static Configuration *_pInstance;
    std::string _filePath;
    std::map<std::string, std::string> _configMap;
    std::set<std::string> _stopWordList_EN;
    std::set<std::string> _stopWordList_ZH;
};


#endif
