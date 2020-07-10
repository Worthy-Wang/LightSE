#include "DirScanner.h"
#include "PageLib.h"
#include "Configuration.h"
#include "WebPage.h"
#include "SplitTool.hpp"
#include "WebPageLib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
using std::cout;
using std::endl;
using std::ifstream;
using std::set;
using std::stringstream;
using namespace wd;
Configuration *Configuration::_pInstance = new Configuration("/home/wwx/mini_search_engine/offline/conf/setting.conf"); //饿汉模式，线程安全

void test1()
{
    DirScanner rssScanner(Configuration::getpInstance()->getConfigMap()["rss_dir"]);
    vector<string> rssFiles = rssScanner.get_absolute_files();
    for (auto &e : rssFiles)
    {
        ifstream ifs(e, ifstream::in);
        if (!ifs.good())
        {
            cout << "can't open" << endl;
        }
    }
}

//从rss语料库解析后导入 网页库 与 偏移库
void test2()
{
    DirScanner rssScanner(Configuration::getpInstance()->getConfigMap()["rss_dir"]);
    RssReader rssReader;
    vector<string> rssFiles = rssScanner.get_absolute_files();
    for (auto &e : rssFiles)
    {
        rssReader.parseRss(e);
    }
    PageLib pageLib(rssScanner, rssReader);
    pageLib.store(Configuration::getpInstance()->getConfigMap()["ripePage_dir"], Configuration::getpInstance()->getConfigMap()["offset_dir"]);
}


//从已经建立好的网页库 和 偏移库 中读取到
void test3()
{
    WebPageLib pageLibs(Configuration::getpInstance()->getConfigMap()["stop_words.utf8"],Configuration::getpInstance()->getConfigMap()["ripePage_dir"],Configuration::getpInstance()->getConfigMap()["offset_dir"]);
    pageLibs.storeOnDisk(Configuration::getpInstance()->getConfigMap()["NewRipePage_dir"],Configuration::getpInstance()->getConfigMap()["Newoffset_dir"], Configuration::getpInstance()->getConfigMap()["invertIndex_dir"]);
    
}

int main()
{
    test3();
    return 0;
}