/***********************************************************
  > File Name: jsonTest.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月16日 星期五 16时37分46秒
  > Modified Time:2022年09月16日 星期五 16时37分46秒
 *******************************************************/
#include <nlohmann/json.hpp>
/* #include "../include/nlohmann/json.hpp" */
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using json = nlohmann::json;

struct Player
{
    string name;
    int credits;
    int ranking;
};

#if 1
void to_json(json &j, const Player &p)
{
    j= json{ {"name", p.name},
    {"credits", p.credits},
    {"ranking", p.ranking} };
}
#endif

#if 1
void from_json(const json &j, Player &p)
{
    j.at("name").get_to(p.name);
    j.at("credits").get_to(p.credits);
    j.at("ranking").get_to(p.ranking);
}
#endif

void test()
{
    auto j = R"([
        {"name": "Judd Trump","credits": 1754500,"ranking": 1},
        {"name": "Neil Robertson","credits": 1040500,"ranking": 2},
        {"name": "Ronnie O'Sullivan","credits": 954500,"ranking": 3}
        ])"_json;
    vector<Player> players = j.get<vector<Player>>();
    /* vector<Player> players; */
    /* j.get_to(players); */
    cout<< "name:" << players[2].name << endl;
    cout<< "credits:" << players[2].credits << endl;
    cout<< "ranking:" << players[2].ranking << endl;
}

void test3()
{
    auto j3 = json::parse(R"({"happy": true, "pi": 3.141})");
    cout << "j3 = " << j3 << endl;
    json j_string = "this is a string";
    auto cpp_string = j_string.get<std::string>();
    cout << "cpp_string = " << cpp_string << endl;
}

void test4()
{
    json j_string = "this is a string";
    auto cpp_string = j_string.get<std::string>();
    std::string cpp_string2;
    j_string.get_to(cpp_string2);
    std::string serialized_string = j_string.dump();
    std::cout << cpp_string
    << " == " << cpp_string2
    << " == " << j_string.get<std::string>() << '\n';
    std::cout << j_string
    << " == " << serialized_string << std::endl;
}
int main()
{
    test();
    return 0;
}
