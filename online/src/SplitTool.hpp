#ifndef _MY_WORD_SEGMENTATION_H_
#define _MY_WORD_SEGMENTATION_H_

#include "cppjieba/include/cppjieba/Jieba.hpp"

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

const char *const DICT_PATH = "cppjieba/dict/jieba.dict.utf8";      //最大概率法(MPSegment: Max Probability)分词所使用的词典路径
const char *const HMM_PATH = "cppjieba/dict/hmm_model.utf8";        //隐式马尔科夫模型(HMMSegment: Hidden Markov Model)分词所使用的词典路径
const char *const USER_DICT_PATH = "cppjieba/dict/user.dict.utf8";  //用户自定义词典路径
const char *const IDF_PATH = "cppjieba/dict/idf.utf8";              //IDF路径
const char *const STOP_WORD_PATH = "cppjieba/dict/stop_words.utf8"; //停用词路径

namespace wd
{
    class SplitTool //使用结巴分词库进行分词
    {
    public:
        SplitTool()
            : _jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH) //初始化Jieba类对象
        {
            cout << "cppjieba init!" << endl;
        }

        vector<string> cut(const string str) //返回str的分词结果
        {
            vector<string> words;
            _jieba.CutForSearch(str, words); //FullSegment
            return words;
        }

    private:
        cppjieba::Jieba _jieba;
    };

} // namespace wd

#endif