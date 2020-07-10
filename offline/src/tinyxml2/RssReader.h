#ifndef __RSSREADER_H__
#define __RSSREADER_H__
#include "tinyxml2.h"
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <regex>
#include <fstream>
#include <sstream>
#include <map>
using std::cout;
using std::endl;
using std::map;
using std::ofstream;
using std::regex;
using std::regex_replace;
using std::string;
using std::stringstream;
using std::vector;
using std::pair;
using namespace tinyxml2;


namespace wd
{

    struct RssItem
    {
        string title;
        string link;
        string description;
        string content;
    };

    class RssReader
    {
    public:
        void DFS(XMLElement *);
        bool parseRss(const string &filename);                                                 //解析
        void dump(const string &ripePage_dir, const string& offset_dir, vector<string> &pageLibs, map<int, pair<int,int>> &offsetLib); //输出
    private:
        vector<RssItem> _rss;
    };

} // namespace wd

#endif