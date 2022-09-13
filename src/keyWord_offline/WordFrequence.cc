/*************************************************************************
  @Author: dsy
  @Created Time : 2022年08月04日 星期四 22时43分10秒
  @File Name: tocken.cc
  @Description: dushiyang68@gmail.com
 ************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>  //用来拆分单词
#include <map>

using std::cout; using std::cin; using std::endl;
using std::string;
using std::vector;
using std::map;

#if 0
//用vector需要的结构体
struct Record
{
    Record(string w, int f): _word(w), _frequency(f){}
    string _word;
    int _frequency;
};
#endif

class Dictionary
{
public:
    Dictionary(){}
    void read(const string &filename);
    string dealWord(const string &word);
    void insertMap(const string &word);
    void print();
    void store(const string &filename);
private:
    //vector<Record> _dict;
    map<string, int> _dict;
};

void Dictionary::print()
{
    for(auto & i : _dict){
        cout << i.first << ": " << i.second <<
            ((i.second > 1) ? " times" : " time") << endl;
    }
}

void Dictionary::read(const string &filename)
{
    std::ifstream ifs(filename);
    if(!ifs.good()){
        cout << "open file error" << endl;
        return ;
    }

    string temp;
    temp.reserve(100);

    //循环读取一行
    while(getline(ifs, temp)){
        //拆分单词
        std::istringstream is(temp);
        string word;
        while (is >> word){
            string newWord = dealWord(word);
            insertMap(newWord);
        }       
    }
}

string Dictionary::dealWord(const string &word)
{
    for(size_t i = 0; i < word.size(); ++i){
        if(!isalpha(word[i])){
            return string();
        }
    }
    return word;
}

void Dictionary::insertMap(const string &word)
{
    if(word.size() > 1){
        ++_dict[word];
    }
}

void Dictionary::store(const string &filename)
{
    std::ofstream ofs(filename);
    if(!ofs.good()){
        cout << "open file error" << endl;
        return ;
    }
    for(auto &p: _dict){
        ofs << p.first << ": " << p.second <<
            ((p.second > 1) ? " times" : " time") << endl;
    }
}

int main(){
    Dictionary d;
    d.read("hello.txt");
    d.store("store.dat");
    return 0;
}
