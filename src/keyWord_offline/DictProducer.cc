#include "../../include/DictProducer.hh"
#include "../../include/SplitToolCppJieba.hh"
#include "../../include/DirScanner.hh"

DirtProducer::DirtProducer(Configuration * conf)//默认处理英文
:_conf(conf)
,_splitTool(nullptr)
,_files(DirScanner()(_conf->getConfigMap().find("path_TXT_EN")->second))
{
    _dict.reserve(10000);
}
    
DirtProducer::DirtProducer(Configuration * conf, SplitToolCppJieba * splitTool)//处理中文
:_conf(conf)
,_splitTool(splitTool)
,_files(DirScanner()(_conf->getConfigMap().find("path_TXT_ZH")->second))
{
    _dict.reserve(10000);
}

DirtProducer::~DirtProducer()
{}

void DirtProducer::buildDict_EN()
{
    unordered_map<string, int> tmpdict;
    for(auto & elem : _files)
    {
        ifstream ifs(elem);
        string strline, word;
        while(getline(ifs,strline))
        {
            stringstream iss(strline);
            while(iss >> word)
            {
                if(!_splitTool->isStopWord(word))
                    ++tmpdict[_splitTool->transferToLower(word)];
            }
        }
    }

    for(auto & e : tmpdict)
    {
        _dict.emplace_back(std::move(e));
    }


    int line_no = 1; //行号
    string word;
    for(auto & e : _dict)
    {
        for(size_t i = 0; i < e.first.size();)
        {
            if(_splitTool->isEnglish(e.first)){
                word = e.first[i];
                ++i;
            }else
            {   
                word = e.first.substr(0,3);
                i+=3;
            }
            if(e.first.find(word) != string::npos)
            {
                _index[word].insert(line_no);
            }
        }
        ++line_no;
    }
}

void DirtProducer::buildDict_ZH()
{
    unordered_map<string, int> tmpdict;
    for(auto & elem : _files)
    {
        ifstream ifs(elem);
        string strline;
        while(getline(ifs,strline))
        {
            vector<string> tmp = _splitTool->cut(strline);
            for(auto & word :tmp)
            {
               if(!_splitTool->isStopWord(word) && !(word.find_first_not_of("0123456789.") == string::npos))
                    ++tmpdict[_splitTool->transferToLower(word)];
            }
        }
    }

    for(auto & e : tmpdict)
    {
        _dict.emplace_back(std::move(e));
    }

    
    int line_no = 1; //行号
    string word;
    for(auto & e : _dict)
    {
        for(size_t i = 0; i < e.first.size();)
        {
            if(_splitTool->isEnglish(e.first)){
                word = e.first[i];
                ++i;
            }else
            {   
                word = e.first.substr(0,3);
                i+=3;
            }
            if(e.first.find(word) != string::npos)
            {
                _index[word].insert(line_no);
            }
        }
        ++line_no;
    }
}

void DirtProducer::storeDict(const string & filePath)
{
    string path_dictIndex = filePath + "/data/dictIndex.dat";
    string path_dict =  filePath + "/data/dict.dat";
    ofstream ofs_dictIndex(path_dictIndex);
    ofstream ofs_dict(path_dict);
    if(!ofs_dict.good())
    {
        cout << "open file error" << endl;
        return;
    }
    if(!ofs_dictIndex.good())
    {
        cout << "open file error" << endl;
        return;
    }
    for(auto & elem : _dict)
    {
       ofs_dict << elem.first << " " << elem.second << "\n";
    }
    
    for(auto & elem : _index)
    {
        ofs_dictIndex << elem.first << " ";
        for(auto & pa : elem.second)
        {
            ofs_dictIndex << pa  << " ";
        }
         ofs_dictIndex << "\n";
    }
    ofs_dict.close();
    ofs_dictIndex.close();
}

void DirtProducer::showDirt()
{
    cout << "-----------------------Dict-----------------------" << endl;
    for(auto & elem : _dict)
    {
        cout << "word :" << elem.first << " frequence:" << elem.second << endl;
    }

    cout << "-----------------------Index-----------------------" << endl;
    for(auto & elem : _index)
    {
        cout << "word:" << elem.first << " index:" << elem.second << endl;
    }
}

void DirtProducer::storeDict()//如果不指明地址，就默认用配置文件里面的
{
    ofstream ofs_dict(_conf->getConfigMap().find("path_dict")->second);
    if(!ofs_dict.good())
    {
        cout << "open file error" << endl;
        return;
    }
    ofstream ofs_dictIndex(_conf->getConfigMap().find("path_dictIndex")->second);
    if(!ofs_dictIndex.good())
    {
        cout << "open file error" << endl;
        return;
    }
    for(auto & elem : _dict)
    {
       
        ofs_dict << elem.first << " " << elem.second << endl;
       
    }

    for(auto & elem : _index)
    {
        ofs_dictIndex << elem.first << " ";
        for(auto & pa: elem.second)
        {
            ofs_dictIndex << pa  << " ";
        }
        ofs_dictIndex << "\n";
    }
    ofs_dict.close();
    ofs_dictIndex.close();
}
