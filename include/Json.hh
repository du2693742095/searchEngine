/***********************************************************
  > File Name: Json.hh
  > Author: Huberyee
  > Mail: 923506068@qq.com
  > Created Time: Thu 15 Sep 2022 09:47:37 PM CST
  > Modified Time:Thu 15 Sep 2022 09:47:37 PM CST
 *******************************************************/

#ifndef __JSON_HH__
#define __JSON_HH__

#include <string>
#include <vector>

#include <nlohmann/json.hpp>


using std::string;
using std::vector;


using json = nlohmann::json;

struct Msg
{
    int _msgLen;
    int _ID;
    vector<string> _msg;
};

void to_json(json &j, const Msg &msg);
void from_json(json &j, Msg &msg);



#endif

