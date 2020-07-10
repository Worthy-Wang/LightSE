#ifndef __PAGELIB_H__
#define __PAGELIB_H__
#include "DirScanner.h"
#include "Configuration.h"
#include "tinyxml2/RssReader.h"
#include <vector>
#include <string>
#include <map>
using std::vector;
using std::string;
using std::map;
using std::pair;

namespace wd
{
    class PageLib
    {
    public:
        PageLib(DirScanner&, RssReader&);
        void store(const string&, const string&); //存储网页库与偏移库

    private:
        DirScanner &_dirScanner;
        RssReader &_rssReader;
        vector<string> _pagelibs; //存放rss格式化后的string
        map<int, pair<int,int>> _offsetLib; //<网页编号, <起始偏移，结束偏移>>
    };

}


#endif