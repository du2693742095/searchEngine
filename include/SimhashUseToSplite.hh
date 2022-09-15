/***********************************************************
  > File Name: SimhashUseToSplite.hh
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月14日 星期三 15时04分31秒
  > Modified Time:2022年09月14日 星期三 15时04分31秒
 *******************************************************/
#ifndef __SIMHASHUSETOSPLITE_HH__
#define __SIMHASHUSETOSPLITE_HH__

#include "simhash/Simhasher.hpp"
#include "Configuration.hh"

using namespace simhash;

//名字弄长一点，免得到时候重复
class SimHashUseToSplite
{
public:
    SimHashUseToSplite(Configuration *conf)
        : _conf(conf)
        , _path(_conf->getConfigMap())
        , _simhasher(_path.find("path_jieba")->second,
                   _path.find("path_hmm_model")->second,
                   _path.find("path_idf")->second,
                   _path.find("path_stop_words")->second)
    {}

    ~SimHashUseToSplite(){}

    bool isRepeat(string &page)
    {
        size_t topN = 5;//提取的索引个数
        uint64_t u64 = 0;//返回的simhash值

        //查重
        _simhasher.make(page, topN, u64);
        for(auto &id: _simhash){
            if(Simhasher::isEqual(id, u64)){
                return true;
            }
        }
        _simhash.insert(u64);
        return false;
    }

    //直接将传入的字符串数组去重
    void deduplication(vector<string> &oldPage)
    {
        size_t topN = 50;//提取的索引个数
        uint64_t u64 = 0;//返回的simhash值

        //查重
        for(auto &page: oldPage){
            _simhasher.make(page, topN, u64);
            for(auto &id: _simhash){
                if(Simhasher::isEqual(id, u64)){
                    //如果重复了，就把它的内容擦去
                    page.clear();
                    break;
                }
            }
        }
    }
    
    //将传入的字符串数组去重后通过newPage传出
    void deduplication(vector<string> &oldPage, vector<string> & newPage)
    {
        size_t topN = 50;//提取的索引个数
        uint64_t u64 = 0;//返回的simhash值

        //查重
        bool isRepeat = true;
        for(auto &page: oldPage){
            _simhasher.make(page, topN, u64);
            for(auto &id: _simhash){
                if(Simhasher::isEqual(id, u64)){
                    isRepeat = false;
                    break;
                }
            }
            //文章不重复
            if(isRepeat){
                _simhash.insert(u64);
                newPage.push_back(page);
            }
        }
    }
    
private:
#if 0
    //检测两篇文章是否重复
    bool isEqual(uint64_t lhs, uint64_t rhs, unsigned short distance = 3)
    {
        unsigned short cnt = 0;
        lhs ^= rhs;
        while(lhs && cnt <= distance){
            lhs &= lhs - 1;
            ++cnt;
        }
        if(cnt <= distance){
            return true;
        }
        else{
            return false;
        }
    }
#endif

private:
    Configuration *_conf;
    map<string, string> &_path;
    Simhasher _simhasher;
    set<uint64_t> _simhash;
};

#endif
