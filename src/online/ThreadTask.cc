/***********************************************************
  > File Name: ThreadTask.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月17日 星期六 10时56分59秒
  > Modified Time:2022年09月17日 星期六 10时56分59秒
 *******************************************************/
#include "../../include/ThreadTask.hh"
#include "../../include/Json.hh"
#include "../../include/WebPageSearcher.hh"
#include "../../include/KeywordsSearcher.hh"

ThreadTask::ThreadTask(const std::string &msg, const TcpConnectionPtr &conn,
                       HiRedis * redis, WebQuery & w, Dictionary * dict)
        : _msg(msg)
        , _conn(conn)
        , _redis(redis)
        , _webQuery(w)
        , _dict(dict)
{}

void ThreadTask::process()
{
    /* cerr << "线程池执行任务" << "\n"; */
    
    //解析发过来的JSON数据包
    Msg command;
    from_json(_msg, command);

    int msgID = command._ID;
    string queryWords = command._msg.front();

    //不用判断是否为空，空字符交给查询函数，返回空的json
    /* 去redis里面查找，底层用的是是trig类型，所以查找时候把查找的key用指令做区分
     * 单词查询的后面加_1，网页查询的加_2，用来查询 */
    switch(msgID)
    {
        case 1:{
            KeywordsSearcher searcher(queryWords, _conn, _dict, _redis);
            searcher.doQuery();
            break;
        }

        case 2:{
            //查询网页
            WebPageSearcher searcher(queryWords, _conn, _webQuery, _redis);
            searcher.doQuery();
            break;
        }

        default :
            _conn->sendToEpollLoop("指令出错");
    }
}
