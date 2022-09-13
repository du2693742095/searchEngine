/***********************************************************
  > File Name: DictProducer.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月13日 星期二 14时48分27秒
  > Modified Time:2022年09月13日 星期二 14时48分27秒
 *******************************************************/
#include "../../include/DictProducer.hh"

DirtProducer::DirtProducer(const string &confPath)
    : _conf(confPath)
    , _dict()
    , _splitTool(nullptr)
    , _index()
{}

DirtProducer::DirtProducer(const string &confPath, SplitTool * splitTool)
    : _conf(confPath)
    , _dict()
    , _splitTool(splitTool)
    , _index()
{}

DirtProducer::~DirtProducer()
{}

void DirtProducer::buildDict_EN()
{
    
}

void DirtProducer::buildDict_ZH()
{

}

void DirtProducer::storeDict(const string & filePath)
{

}

void DirtProducer::showDirt()
{

}

//如果不指明地址，就默认用配置文件里面的
void DirtProducer::storeDict()
{

}

void DirtProducer::getFiles()
{

}

void DirtProducer::pushDict(const string & word)
{

}


