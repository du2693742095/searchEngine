/***********************************************************
  > File Name: DirSacnner.cc
  > Author: dsy
  > Mail: dushiyang68@gmail.com
  > Created Time: 2022年09月13日 星期二 15时24分00秒
  > Modified Time:2022年09月13日 星期二 15时24分00秒
 *******************************************************/
#include "../../include/DirScanner.hh"
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>

DirScanner::DirScanner()
    : _files()
{}

DirScanner::~DirScanner()
{}

vector<string> DirScanner::operator()(const string &dirName)
{
    traverse(dirName);
    return getFiles();
}

void DirScanner::print()
{
    for(auto &p : _files){
        std::cout << p << std::endl;
    }
}

vector<string> DirScanner::getFiles()
{
    return _files;
}

void DirScanner::traverse(const string &dirName)
{
    /* std::cout << "path" << dirName << std::endl; */
    DIR *dp = opendir(dirName.c_str());
    if(dp == nullptr){
        perror("opendir");
        return ;
    }
    
    chdir(dirName.c_str());
    struct dirent *dir = nullptr;
    struct stat mstat;
    while((dir = readdir(dp)) != nullptr)
    {
        char *name = dir->d_name;
        ::stat(name, &mstat);
        //判断是否为文件夹，需要进行递归
        
        if((mstat.st_mode & S_IFMT) == S_IFDIR){
            if(name[0] == '.'){
                continue;
            }
            else{
                std::cout << "递归去了" << std::endl;
                traverse(name);
            }
        }
        else{
            string fileName;
            fileName = fileName + getcwd(NULL, 0) + "/" + name;
            _files.push_back(std::move(fileName));
        }
    }
    chdir("..");
    closedir(dp);
}
