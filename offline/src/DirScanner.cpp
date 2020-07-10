#include "DirScanner.h"
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
using std::cout;
using std::endl;

namespace wd
{
    DirScanner::DirScanner(const string &dir)
        : _dir(dir)
    {
        traverse();
    }

    vector<string> &DirScanner::get_absolute_files()
    {
        return _files;
    }

    void DirScanner::traverse()
    {
        DIR *dir = opendir(_dir.c_str());
        if (!dir)
        {
            cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " opendir ERROR!" << endl;
            exit(-1);
        }
        struct dirent *dir_info;
        while ((dir_info = readdir(dir)) != nullptr)
        {
            string filename(dir_info->d_name);
            if (("." == filename) || ".." == filename)
                continue;
            string s = _dir + "/" + dir_info->d_name;
            _files.push_back(s);
        }
        closedir(dir);
    }
} // namespace wd