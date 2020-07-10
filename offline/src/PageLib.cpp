#include "PageLib.h"
#include "Configuration.h"

namespace wd
{
    PageLib::PageLib(DirScanner &dirScanner, RssReader &rssReader)
        : _dirScanner(dirScanner), _rssReader(rssReader)
    {
    }

    void PageLib::store(const string &pagelibs_dir, const string& offsetLib_dir)
    {
        _rssReader.dump(pagelibs_dir, offsetLib_dir, _pagelibs, _offsetLib);
    }
} // namespace wd