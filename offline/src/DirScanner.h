#ifndef __DIRSCANNER_H__
#define __DIRSCANNER_H__
#include <vector>
#include <string>
using std::string;
using std::vector;

namespace wd
{
    class DirScanner
    {
    public:
        DirScanner(const string &dir);
        vector<string>& get_absolute_files();

    private:
        void traverse(); //获取语料库目录下的所有语料文件

    private:
        string _dir;           //语料库目录
        vector<string> _files; //语料库文件的绝对路径
    };
} // namespace wd

#endif