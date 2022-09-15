#include "../../include/SplitToolCppJieba.hh"
#include <algorithm>

using namespace std;

SplitToolCppJieba::SplitToolCppJieba(Configuration * conf)
:_conf(conf)
,_jieba(_conf->getConfigMap().find("path_jieba")->second,
        _conf->getConfigMap().find("path_hmm_model")->second,
        "")
{
    ifstream ifs_stop(_conf->getConfigMap().find("path_stop_words")->second);
    if(!ifs_stop.good())
    {
        cout << "open file error" << endl;
        return;
    }
    string word;
    while(ifs_stop >> word)
    {
        _stopword.insert(word);
    }
}

SplitToolCppJieba::~SplitToolCppJieba()
{
    _conf = nullptr;
}

vector<string> SplitToolCppJieba::cut(const string & sentence)
{
    vector<string> words;
    _jieba.Cut(sentence, words, true);
    return words;
}  

bool SplitToolCppJieba::isStopWord(const string & word){
    return _stopword.find(word) == _stopword.end();
}

bool SplitToolCppJieba::isEnglish(const string & words)
{
    const char start = *words.begin();
    if(start & (1 << 7)){
        return false;
    }
    else{
        return true;
    }
}

string SplitToolCppJieba::transferToLower(string & words)
{
    std::transform(words.begin(), words.end(), words.begin(), ::tolower);
    return words;
}
