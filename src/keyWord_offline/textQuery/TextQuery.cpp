/*************************************************************************
 @Author: dsy
 @Created Time : 2022年08月15日 星期一 11时39分29秒
 @File Name: TextQuery.cpp
 @Description: dushiyang68@gmail.com
 ************************************************************************/
#include "TextQuery.hh"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <algorithm>

using std::cout;using std::endl;

using std::vector; 
using std::string;
using std::map;
using std::set;
using std::shared_ptr;
using std::ifstream;

TextQuery::TextQuery(ifstream & is)
    : _file(new vector<string>())
{
    string text;
    while(std::getline(is, text)){
        _file->push_back(text);
        int n = _file->size() - 1;//获取行号
        //拆分单词
        std::istringstream curLine(text);
        string word;
        while(curLine >> word){
            //查看单词是否在查询的map中
            auto &lines =  _wordMap[word];//返回set<line_No>的指针
            //如果单词不再map中，就分配一个新的set给他
            if(!lines){
                lines.reset(new set<line_No>);
            }
            lines->insert(n);
        }
    }
}

QueryResult TextQuery::query(const string & rhs) const
{
    shared_ptr<set<line_No>> noNode(new set<line_No>);
    auto result = _wordMap.find(rhs);
    if(result == _wordMap.end()){
        return QueryResult(rhs, noNode, _file);
    }else{
        //std::cout << *(_file->begin() + result->second->size()) << std::endl;
        return QueryResult(rhs, result->second, _file);
    }
}

//测试
void TextQuery::printText()
{
    for(auto & line : *_file){
        cout << line << endl;
    }
}

std::ostream& operator<<(std::ostream& os, const QueryResult& t)
{
    os << t._queryWord << ": " << t._line->size() << " times" << std::endl;
    for(auto num : *t._line){
        os << "\t(line " << num + 1 << ")"
                  << *(t._file->begin() + num) << std::endl;
    }
    return os;
}

/*====================接口类的定义============*/
//Query类
std::ostream & operator<<(std::ostream &os, const Query &query)
{
    return os << query.rep();
}

Query::Query(const std::string & s)
    : _q(new WordQuery(s))
    {}


/*===================派生类==================*/
//NotQuery
QueryResult NotQuery::eval(const TextQuery & t) const
{
    QueryResult result = _query.eval(t);
    //存储结果的行号
    shared_ptr<set<line_No>> ret = std::make_shared<set<line_No>>();
    //将查询到的结果取出来
    auto begin = result.begin(), end = result.end();
    size_t totalLine = result.getFile()->size();
    for(size_t i = 0; i < totalLine; ++i){
        if(begin == end && *begin != i){
            ret->insert(i);
        }else if(begin != end){
            ++begin;
        }
    }
    return QueryResult(rep(), ret, result.getFile());
}

//AndQuery
QueryResult AndQuery::eval(const TextQuery & t) const
{
    auto left = _lhs.eval(t), right = _rhs.eval(t);
    shared_ptr<set<line_No>> ret = std::make_shared<set<line_No>>();
    //用书上的函数，直接合并
    set_intersection(left.begin(), left.end(),
                     right.begin(), right.end(),
                     inserter(*ret, ret->begin()));
    return QueryResult(rep(), ret, left.getFile());
}

//OrQuery
QueryResult OrQuery::eval(const TextQuery & t) const
{
    auto left = _lhs.eval(t), right = _rhs.eval(t);
    shared_ptr<set<line_No>> ret = std::make_shared<set<line_No>>();
    //直接把两个的结果都插入进去，set的性质可以保证不重复
    ret->insert(left.begin(), left.end());
    ret->insert(right.begin(), right.end());
    return QueryResult(rep(), ret, left.getFile());
}
