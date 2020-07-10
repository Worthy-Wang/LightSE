#include "WebPage.h"
#include <sstream>
#include <regex>
#include <queue>
#include <vector>
#include <algorithm>
#include <iterator>
using std::back_inserter;
using std::priority_queue;
using std::regex;
using std::regex_replace;
using std::stringstream;
using std::vector;

namespace wd
{
    WebPage::WebPage(const string &doc, SplitTool &splitTool, set<string>& stopWords)
        : _doc(doc), _splitTool(splitTool), _stopWords(stopWords)
    {
        processDoc();
    }

    void WebPage::processDoc()
    {
        stringstream ss(_doc);
        string word;
        while (ss >> word) // 空白已经全部被替换掉
        {
            if ("<title>" == word)
            {
                ss >> _title;
                //统计词频
                storeWordsMap(_title);
            }
            else if ("<link>" == word)
            {
                ss >> _link;
            }
            else if ("<description>" == word)
            {
                ss >> _description;
                //统计词频
                storeWordsMap(_description);
            }
            else if ("<content>" == word)
            {
                ss >> _content;
                //统计词频
                storeWordsMap(_content);
            }
        }

        //如果没有上述标签，则直接将整个文章存入
        if (_wordsMap.empty())
        {
            storeWordsMap(_doc);
        }

        //计算出前K频率的词
        calcTopK(TOPK_NUMBER);

        // //测试
        // for (auto &e : _wordsMap)
        // {
        //     cout << e.first << ":" << e.second << endl;
        // }
    }


    void WebPage::makeSummary(const map<string,int>& wordsMap)
    {
        for(auto& e: wordsMap)
        {
            size_t pos = _doc.find(e.first);
            if (pos != _doc.npos)
            {
                _summary += _doc.substr(pos, 100); 
            }
        }
    }

    void WebPage::storeWordsMap(const string &s)
    {
        //注意此处不能采用正则表达式，因为一些中英文的停止词也不能够算进去
        vector<string> vec = _splitTool.cut(s);
        for (auto &e : vec)
        {
            if(_stopWords.find(e) != _stopWords.end())
                continue;
            _wordsMap[e]++;
        }
    }

    struct Comp
    {
        bool operator()(const pair<string, int> &lhs, const pair<string, int> &rhs)
        {
            return lhs.second > rhs.second; //降序，形成小顶堆
        }
    };

    void WebPage::calcTopK(size_t k)
    {
        priority_queue<pair<string, int>, vector<pair<string, int>>, Comp> priQue;
        for (auto &e : _wordsMap)
        {
            priQue.push(e);
            if (priQue.size() > k)
            {
                priQue.pop();
            }
        }

        //赋值给_topWords
        while (!priQue.empty())
        {
            _topWords.push_back(priQue.top().first);
            priQue.pop();
        }

        std::sort(_topWords.begin(), _topWords.end());

        // //测试_topWords
        // cout << "------------------------------------" << endl;
        // cout << "docid:" << _docid << endl;
        // for (auto& e: _topWords)
        // {
        //     cout << e  << ":" << _wordsMap[e] << endl;
        // }
    }


    string WebPage::getDoc() const
    {
        return _doc;
    }

    map<string, int> &WebPage::getWordsMap()
    {
        return _wordsMap;
    }


    bool operator==(const WebPage &lhs, const WebPage &rhs)
    {
        vector<string> vec1 = lhs._topWords;
        vector<string> vec2 = rhs._topWords;
        vector<string> intersection;
        std::set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), back_inserter(intersection));
        if (intersection.size() == TOPK_NUMBER) //前K个高频词都相同 
            return true;
        else 
            return false;
    }

} // namespace wd