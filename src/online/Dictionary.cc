#include "../../include/Dictionary.hh"
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::make_pair;

//类外初始化
Dictionary* Dictionary::_pDictionary = nullptr;
Dictionary::AutoRelease Dictionary::_ar;

//静态函数
Dictionary* Dictionary::createInstance()
{
    if(_pDictionary == nullptr)
    {
        _pDictionary = new Dictionary();
    }
    return _pDictionary;
}

//通过词典文件路径初始化词典
void Dictionary::init(const string &dict, const string &dictIndex)
{
    //读取词典
    ifstream ifs1(dict.c_str());
    //读取索引表
    ifstream ifs2(dictIndex.c_str());
    //记录词典每行，索引表每行
    string str1, str2;


    //判断文件流正误
    if(!ifs1.good())
    {
        cout << "ifstream open dict file error" << endl;
        return;
    }
    if(!ifs2.good())
    {
        cout << "ifstream open dictIndex file error" << endl;
        return;
    }

    //初始化词典vector
    while(getline(ifs1, str1))
    {
        //去掉中间的空格，分割出单词和频率
        stringstream ss(str1);
        string word;
        string frequency;

        ss >> word;
        ss >> frequency;
        _dict.emplace_back(word, stoi(frequency));
    }

    //初始化索引表map
    while(getline(ifs2, str2))
    {
        //去掉中间的空格，分割出单词和索引值
        stringstream ss(str2);
        //临时索引集合
        set<int> indexSet;
        string word;
        string index;
        
        ss >> word;
        while(ss >> index)
        {
            indexSet.insert(stoi(index));
        }
        _indexTable.insert(make_pair(word, indexSet));
    }

    ifs1.close();
    ifs2.close();
}

//获取词典
vector<pair<string, int>>& Dictionary::getDict()
{
    return _dict;
}

//获取索引表
map<string, set<int>>& Dictionary::getIndexTable()
{
    return _indexTable;
}

//调试打印
void Dictionary::show()
{
    //调试打印(增量编译)已通过
    cout << "dict : " << endl;
    for(auto & data : _dict)
    {
        cout << data.first << " " << data.second << endl;
    }
    cout << "indexTable : " << endl;
    for(auto & data : _indexTable)
    {
        cout << data.first << " ";
        for(auto & set : data.second)
        {
            cout << set << " ";
        }
        cout << endl;
    }
}
