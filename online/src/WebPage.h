#ifndef __WEBPAGE_H__
#define __WEBPAGE_H__
#include "SplitTool.hpp"
#include <string>
#include <vector>
#include <map>
#include <set>
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

namespace wd
{
    const static size_t TOPK_NUMBER = 20;
    //存储一篇文章, 也就是<doc>..</doc>
    class WebPage
    {
    public:
        WebPage(const string &doc, SplitTool &splitTool, set<string>& stopWords);
        map<string, int> &getWordsMap();
        void makeSummary(const map<string,int>& wordsMap); //根据查询词生成摘要 
        string getDoc() const;
        string getTitle() const { return _title;}
        string getLink() const { return _link; }
        string getSummary() const { return _summary;}

        //判断两篇文章是否相等
        friend bool operator==(const WebPage &lhs, const WebPage &rhs);

    private:
        void processDoc();                   //使用cppjieba，对格式化文档进行处理
        void storeWordsMap(const string &s); //进行词频统计
        void calcTopK(size_t k);             //topK算法计算频率最高的k个单词

    private:
        string _doc; //整个<doc> .. </doc>， 也就是整篇文章xml
        string _title;
        string _link; //url
        string _description;
        string _content;
        string _summary; //摘要，自行生成
        vector<string> _topWords;   //该篇文章频率最高的前k个词
        map<string, int> _wordsMap; //该篇文章  <单词，频率>
        SplitTool &_splitTool;      //cppjieba中文分词库
        set<string> &_stopWords;    //停词库
    };

} // namespace wd

#endif