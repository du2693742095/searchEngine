#ifndef __LRUCACHE_HH__
#define __LRUCACHE_HH__

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

class LRUCache
{
struct LNode
    {
        //存储的键值对
        //key代表要查的单词
        //value代表查找到的最接近的单词
        string key;
        string value;
        //前后节点指针
        LNode* front;
        LNode* next;
        //初始化
        LNode(string _key, string _value)
        :key(_key)
        ,value(_value)
        ,front(nullptr)
        ,next(nullptr){}
    };

public:
    //构造函数
    LRUCache(int num = 100);
    //拷贝构造
    LRUCache(const LRUCache &cache);
    //往双向链表中插入新的节点
    void insert(LNode* p);
    //从双向链表中删除一个节点
    void remove(LNode* p);
    //往缓存中添加或删除数据
    void addElement(const string &key, const string &value);
    //从文件中读取缓存信息
    void readFromFile(const string &filename);
    //将缓存信息写入到文件中
    void writeToFile(const string &filename);
    //更新缓存信息
    void update(const LRUCache &rhs);
    //获取待更新的节点LNode
    LNode* getPendingUpdateList();

private:
    //采用hashTable查找
    unordered_map<string, LNode*> _hashMap;
    //缓存节点的容量
    int _capacity;
    //头结点
    struct LNode* _Head;
    //尾节点
    struct LNode* _Rear;
};

#endif
