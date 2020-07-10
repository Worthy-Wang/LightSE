#ifndef __WEBPAGELIB_H__
#define __WEBPAGELIB_H__
#include "SplitTool.hpp"
#include "Configuration.h"
#include "WebPage.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <unordered_map>
using std::unordered_map;
using std::cout;
using std::endl;
using std::ifstream;
using std::set;
using std::stringstream;
using std::string;

namespace wd
{   
    class WebPageLib
    {
    public:
        WebPageLib(const string& stopWords_dir, const string& ripePage_dir, const string& offset_dir);
        void storeOnDisk(const string& , const string&, const string&); //将去重后的内容写到新的网页库，偏移库, 倒排索引库中
        

    private:
        void readInfoFromFile(); //从停词库，网页库，偏移库读取数据, 进行去重
        void createInvertIndexTable(); //建立倒排索引表
        void createDF_hashmap(); //TF_IDF算法的辅助哈希表

    private:
        string _stopWords_dir;   
        string _ripePage_dir;
        string _offset_dir;
        vector<WebPage> _webPages; //网页
        unordered_map<string, int> _DF_hashmap; //文档频率哈希表 <单词，文档频率>
        unordered_map<string, vector<pair<int, double>>> _invertIndexTable; //倒排索引 <单词，<所在文档ID, 单词权重>>
        set<string> _stopWords;  //停词库
        SplitTool _splitTool;     //cppjieba分词工具
    };   
}


#endif