#ifndef __TEXTQUERY_H__
#define __TEXTQUERY_H__

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <set>

using line_No = std::vector<std::string>::size_type;

//存储查询的结果
class QueryResult
{
public:
    QueryResult(std::string w, std::shared_ptr<std::set<line_No>> line,
                std::shared_ptr<std::vector<std::string>> file)
        : _queryWord(w), _line(line), _file(file) {}

    //返回文件
    std::shared_ptr<std::vector<std::string>> getFile()
    {   return _file;   }

    //定义迭代器，返回查找到的行号信息
    std::set<line_No>::iterator begin()
    {    return _line->begin();    }
    std::set<line_No>::iterator end()
    {    return _line->end();    }

    friend std::ostream& operator<<(std::ostream& os, const QueryResult & t);

private:
    std::string _queryWord; //待查询的单词
    std::shared_ptr<std::set<line_No>> _line;//所有的行号信息
    std::shared_ptr<std::vector<std::string>> _file;//保存文件
};

//构造单词表
class TextQuery
{    
public:
    //将vector<string>的最小单位重命名一下
    //将文本读入内存，构造函数
    TextQuery(std::ifstream & is);
    QueryResult query(const std::string &) const;

//测试
    void printText(void);

private:
    //存储文件的智能指针
    std::shared_ptr<std::vector<std::string>> _file;
    //存储每隔单词到行号的映射
    std::map<std::string, std::shared_ptr<std::set<line_No>>> _wordMap;
};

/*====================基类类======================*/
//原始基类，用来给下面的所有的派生类继承，定义接口
class Query_base
{
    friend class Query;
protected:
    virtual ~Query_base() = default;
private:
    //eval表示执行查询，并存储到QueryResult里面去
    virtual QueryResult eval(const TextQuery &) const = 0;
    //返回查询结果的string形式，
    virtual std::string rep() const = 0;
};

//用于封装的大框架，所有的接口都隐藏在这里面
class WordQuery;
class Query
{
    //定义三个运算符，即三种查询方式
    //声明为友元的是为了之后能访问shared_ptr保存的Query_base基类指针
    friend Query operator~(const Query &);
    friend Query operator|(const Query &, const Query &);
    friend Query operator&(const Query &, const Query &);

    friend std::ostream & operator<<(std::ostream &os, const Query &query);

public:
    Query(const std::string & s);

    QueryResult eval(const TextQuery & t) const
    {   return  _q->eval(t);   }
    
    std::string rep() const 
    {   return _q->rep();     }

private:
    Query(std::shared_ptr<Query_base> query) : _q(query){}
    std::shared_ptr<Query_base> _q;
};


/*=====================单运算符的派生类=======================*/
class WordQuery: public Query_base
{
    friend Query;
private:
    WordQuery(const std::string &s): _queryWord(s){}

    QueryResult eval(const TextQuery & t) const
    {   return t.query(_queryWord);     }

    std::string rep() const
    {   return _queryWord;   }

private:
    std::string _queryWord;
};

//取非
class NotQuery: public Query_base
{
    //用Query引用的原因是之后都是用Qurey指针来访问
    friend Query operator~(const Query &);

private:
    NotQuery(const Query &q): _query(q){}
    QueryResult eval(const TextQuery & t) const;
    std::string rep() const
    {   return "(~" + _query.rep() + ")";   };

private:
    Query _query;
};

/*=====================基类=======================*/
//用于定义两个操作数运算符的基类
class BinaryQuery: public Query_base
{
protected:
    BinaryQuery(const Query &l, const Query &r, std::string s)
        : _lhs(l), _rhs(r), _opSym(s){}
    virtual QueryResult eval(const TextQuery &) const = 0;
    std::string rep() const
    {
        return "(" + _lhs.rep() + " " + _opSym + " " + _rhs.rep() + ")"; 
    }

    Query _lhs, _rhs;   //左右操作数
    std::string _opSym; //运算符的名字
};


/*=====================双运算符的派生类=======================*/
class AndQuery: public BinaryQuery
{
    friend Query operator&(const Query &lhs, const Query &rhs);
private:
    AndQuery(const Query &lhs, const Query &rhs)
        : BinaryQuery(lhs, rhs, "&"){}
    QueryResult eval(const TextQuery & t) const;
};

class OrQuery: public BinaryQuery
{
    friend Query operator|(const Query &lhs, const Query &rhs);
private:
    OrQuery(const Query &lhs, const Query &rhs)
        : BinaryQuery(lhs, rhs, "|"){}
    QueryResult eval(const TextQuery & t) const;
};


/*=====================Query重载的运算符===========================*/
inline Query operator~(const Query &rhs)
{
    return std::shared_ptr<Query_base>(new NotQuery(rhs));
}

inline Query operator&(const Query &lhs, const Query &rhs)
{
    return std::shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}

inline Query operator|(const Query &lhs, const Query &rhs)
{
    return std::shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}


#endif
