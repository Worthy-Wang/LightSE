#include "WebPageLib.h"
#include <cmath>
using std::make_pair;
using std::ofstream;

namespace wd
{
    WebPageLib::WebPageLib(const string &stopWords_dir, const string &ripePage_dir, const string &offset_dir)
        : _stopWords_dir(stopWords_dir), _ripePage_dir(ripePage_dir), _offset_dir(offset_dir)
    {
        readInfoFromFile();
    }

    void WebPageLib::readInfoFromFile()
    {
        //读取停词库
        ifstream stop_words_File(_stopWords_dir);
        if (!stop_words_File.good())
        {
            cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " file open ERROR!" << endl;
            exit(-1);
        }
        string words;
        while (getline(stop_words_File, words))
        {
            _stopWords.insert(words);
        }

        //读取偏移库
        ifstream pageFile(_ripePage_dir);
        ifstream offsetFile(_offset_dir);
        if (!pageFile.good() || !offsetFile.good())
        {
            cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " file open ERROR!" << endl;
            exit(-1);
        }

        string s;
        int docid;
        off_t beg, end;
        while (getline(offsetFile, s))
        {
            stringstream ss(s);
            ss >> docid >> beg >> end;
            //根据偏移读取<doc> ... </doc>
            char buf[65536] = {0};
            pageFile.read(buf, end - beg);
            string doc(buf);
            WebPage page(doc, _splitTool, _stopWords);

            //如果是相同的网页则不必再次加入
            if (std::find(_webPages.begin(), _webPages.end(), page) == _webPages.end())
            {
                _webPages.push_back(std::move(page));
            }
        }

        stop_words_File.close();
        offsetFile.close();
        pageFile.close();
    }

    //去重后重新存入
    void WebPageLib::storeOnDisk(const string &newPageFile, const string &newoffset, const string &invertIndex)
    {
        ofstream pageFile(newPageFile, ofstream::out);
        ofstream offsetFile(newoffset, ofstream::out);
        ofstream invertIndexFile(invertIndex, ofstream::out);
        if (!pageFile.good() || !offsetFile.good() || !invertIndexFile.good())
        {
            cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " file open ERROR!" << endl;
            exit(-1);
        }

        //写入新的 网页库 与 偏移库
        for (size_t i = 0; i < _webPages.size(); i++)
        {
            string doc = _webPages[i].getDoc();
            off_t beg = pageFile.tellp();
            pageFile << doc;
            off_t end = pageFile.tellp();
            offsetFile << i << " " << beg << " " << end << endl;
        }

        //创建TF-IDF算法辅助哈希表
        createDF_hashmap();

        //建立倒排索引表
        createInvertIndexTable();

        //写入倒排索引文件
        for (auto &e : _invertIndexTable)
        {
            invertIndexFile << e.first << " ";
            for (auto &n : e.second)
            {
                invertIndexFile << n.first << " " << n.second << " ";
            }
            invertIndexFile << endl;
        }

        pageFile.close();
        offsetFile.close();
        invertIndexFile.close();
        cout << "新的网页库，偏移库，倒排索引库创建成功" << endl;
    }

    /*
建立倒排索引：
unordered_map<string, vector<pair<int, double>>> _invertIndexTable; //<单词，<所在文档ID, 单词权重>>
通俗的讲，倒排索引的目的就是建立 单词在不同网页中的权重

权重计算方法：TF-IDF算法
TF : 单词在该篇文章中的频率 
DF : 包含该词语的文章总数
IDF : lg (N / DF + 1) 逆文档频率，N代表文章总数，该系数越大则说明 该单词在该文章中辨识度高，权重越大
w = TF * IDF   也就是 w = 单词频率 * 单词在文章中的辨识度
*/

    void WebPageLib::createInvertIndexTable()
    {
        vector<vector<pair<string, double>>> weights(_webPages.size()); //<单词，未归一化权重> temp下标代表文章id

        for (size_t i = 0; i < _webPages.size(); i++)
        {
            map<string, int> &wordsMap = _webPages[i].getWordsMap();
            for (auto &e : wordsMap) // <单词，词频>
            {
                string word = e.first;
                double TF = (double)e.second / _webPages[i].getTotalWords();
                int DF = _DF_hashmap[word];
                int N = _webPages.size();
                double IDF = log10((double)N / (DF + 1));
                double w = TF * IDF;
                weights[i].push_back(make_pair(word, w));
            }
        }

        //将单词的权重进行归一化处理 W = w /sqrt(w1^2 + w2^2 +...+ wn^2)
        for (size_t i = 0; i < weights.size(); i++)
        {
            vector<pair<string, double>> &wordsVec = weights[i]; //i号文档的 <单词，未处理权重>
            double sum = 0;
            for (auto &e : wordsVec)
            {
                sum += e.second * e.second;
            }
            sum = sqrt(sum);
            for (auto &e : wordsVec)
            {
                e.second /= sum;
            }
        }

        //插入结果到倒排索引表
        for (size_t i = 0; i < weights.size(); i++)
        {
            vector<pair<string, double>> &wordsVec = weights[i]; //i号文档的 <单词，归一化权重>
            for (auto &e : wordsVec)
            {
                _invertIndexTable[e.first].push_back(make_pair(i, e.second));
            }
        }
    }

    void WebPageLib::createDF_hashmap()
    {
        for (size_t i = 0; i < _webPages.size(); i++)
        {
            map<string, int> &wordsMap = _webPages[i].getWordsMap();
            for (auto &e : wordsMap)
            {
                _DF_hashmap[e.first]++;
            }
        }

        // 测试函数
        // for (auto& e: _DF_hashmap)
        // {
        //     cout << e.first << ":" << e.second << endl;
        // }
        // cout << _DF_hashmap.size() << endl;
        // exit(0);
    }

} // namespace wd