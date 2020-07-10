#ifndef __WORDQUERY_H__
#define __WORDQUERY_H__
#include "SplitTool.hpp"
#include "Configuration.h"
#include "./net/TcpConnection.h"
#include "WebPage.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <unordered_map>
using std::cout;
using std::endl;
using std::ifstream;
using std::set;
using std::string;
using std::stringstream;
using std::unordered_map;

namespace wd
{
    class WordQuery
    {
    public:
        WordQuery(const string &stopWords_dir, const string &ripePage_dir, const string &offset_dir, const string &invertIndex_dir);
        void doQuery(const string &str, const TcpConnectionPtr&); //执行查询，用于绑定线程池的回调函数

    private:
        void readInfoFromFile(); //从停词库，网页库，偏移库，倒排索引库读取数据
        vector<pair<string, double>> getWeightVector(const map<string, int> &);
        set<int> getAllMatchingPages(const vector<pair<string, double>> &);
        vector<pair<int, double>> cosSort(const vector<pair<string, double>> &, const set<int>&); //余弦相似度算法
        string JsonPackage(vector<pair<int,double>>& ); //将网页封装成json格式
        string return404Json(); //返回404网页json格式
        double countCos(const vector<double> &, const vector<double> &); //计算余弦相似度
        double getMold(const vector<double> &);                          //求向量模长
        void makePageSummary(const set<int>& pageIds, const map<string,int>& wordsMap); //为匹配到的文章生成摘要

    private:
        string _stopWords_dir;
        string _ripePage_dir;
        string _offset_dir;
        string _invertIndex_dir;

        vector<WebPage> _webPages;                                           //网页库
        unordered_map<int, pair<int, int>> _offsetLib;                       //偏移库
        unordered_map<string, unordered_map<int, double>> _invertIndexTable; //倒排索引库 <单词，<所在文档ID, 单词权重>>
        set<string> _stopWords;                                              //停词库
        SplitTool _splitTool;                                                //cppjieba分词工具
    };
} // namespace wd

#endif