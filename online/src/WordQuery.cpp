#include "WordQuery.h"
#include <cmath>
#include <algorithm>
#include <iterator>
#include <json/json.h>
#include <string>
using std::make_pair;
using std::ofstream;

namespace wd
{
    WordQuery::WordQuery(const string &stopWords_dir, const string &ripePage_dir, const string &offset_dir, const string &invertIndex_dir)
        : _stopWords_dir(stopWords_dir), _ripePage_dir(ripePage_dir), _offset_dir(offset_dir), _invertIndex_dir(invertIndex_dir)
    {
        readInfoFromFile();
    }

    void WordQuery::readInfoFromFile()
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

        //读取网页库，偏移库，倒排索引库
        ifstream pageFile(_ripePage_dir);
        ifstream offsetFile(_offset_dir);
        ifstream invertIndexFile(_invertIndex_dir);
        if (!pageFile.good() || !offsetFile.good() || !invertIndexFile.good())
        {
            cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " file open ERROR!" << endl;
            exit(-1);
        }

        //读取网页库与偏移库
        {
            string s;
            int docid;
            off_t beg, end;
            while (getline(offsetFile, s))
            {
                stringstream ss(s);
                ss >> docid >> beg >> end;
                //根据偏移库读取<doc> ... </doc>
                char buf[65536] = {0};
                pageFile.read(buf, end - beg);
                string doc(buf);
                WebPage page(doc, _splitTool, _stopWords);

                _offsetLib[docid] = make_pair(beg, end);
                _webPages.push_back(std::move(page));
            }
        }

        //读取倒排索引库
        {
            string s;
            string word;
            int docid;
            double w;
            while (getline(invertIndexFile, s))
            {
                stringstream ss(s);
                ss >> word;
                while (ss)
                {
                    ss >> docid >> w;
                    _invertIndexTable[word][docid] = w;
                }
            }
        }

        stop_words_File.close();
        offsetFile.close();
        pageFile.close();
        invertIndexFile.close();
        cout << "停词库，网页库，偏移库，倒排索引库读取数据成功!" << endl;
    }

    void WordQuery::doQuery(const string &s, const TcpConnectionPtr &ptr)
    {
        //将进行查询的语句看作一篇文档， 或者说是一篇网页
        WebPage page(s, _splitTool, _stopWords);

        //用TF-IDF算法计算出该篇文章单词权重
        map<string, int> &wordsMap = page.getWordsMap();
        vector<pair<string, double>> vec = getWeightVector(wordsMap);

        //根据解析的单词，找到所有匹配的文档ID
        set<int> pageIds = getAllMatchingPages(vec);
        string ret;
        if (pageIds.empty())
        {
            ret = return404Json(); //没有找到对应文章，返回404json
        }
        else
        {
            //使用余弦相似度算法对文档进行排序
            vector<pair<int, double>> sortedVec; //<文档id，余弦相似度>
            sortedVec = cosSort(vec, pageIds);
            //为每一篇文章生成摘要
            makePageSummary(pageIds, wordsMap);
            //json封装数据返回客户端 发送字符串的格式为 json长度\n格式化json字符串
            ret = JsonPackage(sortedVec);
        }
        int sz = ret.size();
        string message(std::to_string(sz));
        message.append("\n").append(ret);
        ptr->sendInEventLoop(message);

        // //测试函数
        // for (auto &e : sortedVec)
        // {
        //     cout << "文档id: " << e.first << " , 余弦相似度：" << e.second << endl;
        // }

        // // 测试函数
        // cout << "-----------------------------" << endl;
        // cout << "查询词 权重向量:" << endl;
        // for (auto &n : vec)
        // {
        //     cout << n.first << ":" << n.second << endl;
        // }
        // for (auto &e : weightsMap)
        // {
        //     cout << "-----------------------------" << endl;
        //     cout << "文章id:" << e.first << endl;
        //     cout << "对应权重向量:";
        //     for (auto &n : e.second)
        //     {
        //         cout << n << " ";
        //     }
        //     cout << endl;
        //     cout << "文章:" << endl;
        //     cout << _webPages[e.first].getDoc() << endl;
        // }
    }

    void WordQuery::makePageSummary(const set<int> &pageIds, const map<string, int> &wordsMap)
    {
        for (auto& docid: pageIds)
        {
            _webPages[docid].makeSummary(wordsMap);
        }
    }

    string WordQuery::return404Json()
    {
        Json::Value root;
        Json::Value arr;

        Json::Value elem;
        elem["title"] = "404 NOT FOUND";
        elem["summary"] = "很抱歉，没有找到相关的网页。";
        elem["url"] = "NULL";
        arr.append(elem);

        root["files"] = arr;
        Json::StyledWriter writer;
        return writer.write(root);
    }

    double WordQuery::countCos(const vector<double> &X, const vector<double> &Y)
    {
        if (X.size() != Y.size())
        {
            cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " 权重向量长度不相等!" << endl;
            exit(-1);
        }

        //cos0 = (X*Y) / (|X| * |Y|)
        double up = 0;
        for (size_t i = 0; i < X.size(); i++)
            up += X[i] * Y[i];

        double down = getMold(X) * getMold(Y);
        return up / down;
    }

    double WordQuery::getMold(const vector<double> &vec)
    { //求向量的模长
        int n = vec.size();
        double sum = 0.0;
        for (int i = 0; i < n; ++i)
            sum += vec[i] * vec[i];
        return sqrt(sum);
    }

    string WordQuery::JsonPackage(vector<pair<int, double>> &sortedVec)
    {
        Json::Value root;
        Json::Value arr;
        int cnt = 0;
        for (auto &e : sortedVec)
        {
            int docid = e.first;
            WebPage webpage = _webPages[docid];

            Json::Value elem;
            elem["title"] = webpage.getTitle();
            elem["summary"] = webpage.getSummary();
            elem["url"] = webpage.getLink();
            arr.append(elem);
            if (++cnt == 100) //最多记录100条
                break;
        }

        root["files"] = arr;
        Json::StyledWriter writer;
        return writer.write(root);
    }

    vector<pair<int, double>> WordQuery::cosSort(const vector<pair<string, double>> &vec, const set<int> &pageIds)
    {
        vector<double> weightsVec; //单词权重向量
        for (auto &e : vec)
            weightsVec.push_back(e.second);

        //先计算出每一篇文章对应查询单词的权重向量
        map<int, vector<double>> weightsMap; //<文章id, 对应查询词的权重向量>
        for (auto &docid : pageIds)
        {
            for (auto &e : vec)
            {
                string word = e.first;
                double w = _invertIndexTable[word][docid];
                weightsMap[docid].push_back(w);
            }
        }

        //对文章先后进行排序
        vector<pair<int, double>> sortedVec; //<文档id，余弦相似度>
        for (auto &e : weightsMap)
        {
            int docid = e.first;
            vector<double> pageVec = e.second; //不同网页的查询词权重向量
            double cos = countCos(pageVec, weightsVec);
            sortedVec.push_back(make_pair(docid, cos));
        }
        std::sort(sortedVec.begin(), sortedVec.end(), [&](const pair<int, double> &lhs, const pair<int, double> &rhs) {
            return lhs.second > rhs.second;
        });

        return sortedVec;
    }

    set<int> WordQuery::getAllMatchingPages(const vector<pair<string, double>> &vec)
    {
        //通过倒排索引表查找出 包含vec中所有单词的网页id
        vector<set<int>> docid_sets; //存放单词的所在网页id集合
        for (size_t i = 0; i < vec.size(); i++)
        {
            string word = vec[i].first;
            unordered_map<int, double> temp = _invertIndexTable[word];
            set<int> sets;
            for (auto &e : temp)
            {
                sets.insert(e.first);
            }
            docid_sets.push_back(sets);
        }

        //再将docid_sets中 每个单词所在文档集合 求交集
        set<int> ans = docid_sets[0];
        for (size_t i = 1; i < docid_sets.size(); i++)
        {
            set<int> temp;
            std::set_intersection(docid_sets[i].begin(), docid_sets[i].end(), ans.begin(), ans.end(), inserter(temp, temp.begin()));
            swap(ans, temp);
        }

        return ans; //候选文档id
    }

    //获取查询词的权重向量
    vector<pair<string, double>> WordQuery::getWeightVector(const map<string, int> &wordsMap)
    {
        //先求取map中包含的单词总数
        int sum = 0;
        for (auto &e : wordsMap)
        {
            sum += e.second;
        }

        vector<pair<string, double>> vec;
        for (auto &e : wordsMap)
        {
            string word = e.first;
            double TF = (double)e.second / sum;
            int DF = _invertIndexTable[word].size();
            int N = _webPages.size();
            double IDF = log10((double)N / (DF + 1));
            double w = TF * IDF;
            vec.push_back(make_pair(word, w));
        }

        //权重归一化
        double sum2 = 0;
        for (auto &e : vec)
        {
            sum2 += e.second * e.second;
        }
        sum2 = sqrt(sum2);
        for (auto &e : vec)
        {
            e.second /= sum;
        }
        return vec;
    }

} // namespace wd