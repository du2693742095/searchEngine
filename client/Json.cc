/***********************************************************
  > File Name: Json.cc
  > Author: Huberyee
  > Mail: 923506068@qq.com
  > Created Time: Thu 15 Sep 2022 10:31:43 PM CST
  > Modified Time:Thu 15 Sep 2022 10:31:43 PM CST
 *******************************************************/

#include "Json.hh"


void to_json(json &j, const Msg &msg){
    j= json{ {"msgLen", msg._msgLen},
        {"ID", msg._ID},
        {"msg", msg._msg} };
}

void from_json(const string &recvMsg, Msg &msg){
    json j = json::parse(recvMsg);
    j.at("msgLen").get_to(msg._msgLen);
    j.at("ID").get_to(msg._ID);
    j.at("msg").get_to(msg._msg);
} 
