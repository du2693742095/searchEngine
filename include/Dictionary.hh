#ifndef __DICTIONARY_HH__
#define __DICTIONARY_HH__

#include <set>
#include <map>
#include <string>
#include <vector>
#include <unordered_map>

using std::set;
using std::map;
using std::pair;
using std::vector;
using std::string;
using std::unordered_map;

//单例对象自动释放：嵌套类
class Dictionary
{
    //AutoRelease转为单例Dictionary服务
    class AutoRelease
    {
    public:
        AutoRelease(){}
        ~AutoRelease()
        {
            //嵌套类内部可直接访问外部类静态数据成员
            if(_pDictionary)
            {
                delete _pDictionary;
                _pDictionary = nullptr;
            }
        }
    };

public:
    //静态函数
    static Dictionary* createInstance();
    //通过词典文件路径初始化词典
    void init(const string &dictpath);
    //获取词典
    vector<pair<string, int>> &getDict();
    //获取索引表
    map<string, set<int>> &getIndexTable();
    
private:
    Dictionary(){}
    ~Dictionary(){}
    Dictionary(const Dictionary &) = delete;
    Dictionary & operator = (const Dictionary &) = delete;

private:
    //单例对象
    static Dictionary* _pDictionary; 
    //自动释放对象
    static AutoRelease _ar;
    //词典
    vector<pair<string, int>> _dict;
    //索引表
    map<string, set<int>> _indexTable;
};

#endif
