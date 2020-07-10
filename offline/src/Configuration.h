#ifndef _MY_CONFIGURATION_H_
#define _MY_CONFIGURATION_H_
#include <string>
#include <map>
using std::map;
using std::string;

namespace wd
{
    class Configuration
    {
    public:
        static Configuration* getpInstance();
        static void destroy();
        map<string,string>& getConfigMap();
        
    private:
        Configuration(const string& filepath); //传入配置文件的相对路径
        ~Configuration();
        void readConf();
        
    private:
        string _filepath;
        map<string,string> _configMap;// <配置名称，实际参数>
        static Configuration* _pInstance;
    };
} // namespace wd

#endif