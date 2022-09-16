/***********************************************************
  > File Name: candidateresult.h
  > Author: Huberyee
  > Mail: 923506068@qq.com
  > Created Time: Wed 14 Sep 2022 08:57:31 PM CST
  > Modified Time:Wed 14 Sep 2022 08:57:31 PM CST
 *******************************************************/

#ifndef __CANDIDATERESULT_HH__
#define __CANDIDATERESULT_HH__

#include <string>

using std::string;


class CandidateResult
{
public:
    CandidateResult(string word, int freq, int dist)
    :_word(word)
    ,_freq(freq)
    ,_dist(dist)
    {}
    string _word;
    int _freq;
    int _dist;
};

#endif
