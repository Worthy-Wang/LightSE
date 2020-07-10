#include "RssReader.h"

namespace wd
{
    void RssReader::DFS(XMLElement *Root)
    {
        while (Root)
        {
            if (!strcmp(Root->Name(), "item"))
            {
                RssItem it;
                XMLElement *Title = Root->FirstChildElement("title");
                XMLElement *Link = Root->FirstChildElement("link");
                XMLElement *Description = Root->FirstChildElement("description");
                XMLElement *Content_encoded = Root->FirstChildElement("content:encoded");
                if (Title && Title->GetText())
                {
                    stringstream ss;
                    string s = Title->GetText();
                    s = regex_replace(s, regex("(<)[^>]*(>)"), ""); //<XXX> 这样的形式全部转换为 "" 
                    ss << regex_replace(s, regex("\\s|\u3000"), ","); // 所有空白全部转换为","
                    it.title = ss.str();
                }
                if (Link && Link->GetText())
                {
                    stringstream ss;
                    string s = Link->GetText();
                    s = regex_replace(s, regex("(<)[^>]*(>)"), ""); //<XXX> 这样的形式全部转换为 "" 
                    ss << regex_replace(s, regex("\\s|\u3000"), ","); // 所有空白全部转换为","
                    it.link = ss.str();
                }
                if (Description && Description->GetText())
                {
                    stringstream ss;
                    string s = Description->GetText();
                    s = regex_replace(s, regex("(<)[^>]*(>)"), ""); //<XXX> 这样的形式全部转换为 "" 
                    ss << regex_replace(s, regex("\\s|\u3000"), ","); // 所有空白全部转换为","
                    it.description = ss.str();
                }
                if (Content_encoded && Content_encoded->GetText())
                {
                    stringstream ss;
                    string s = Content_encoded->GetText();
                    s = regex_replace(s, regex("(<)[^>]*(>)"), ""); //<XXX> 这样的形式全部转换为 "" 
                    ss << regex_replace(s, regex("\\s|\u3000"), ","); // 所有空白全部转换为","
                    it.content = ss.str();
                }
                _rss.push_back(it);
            }

            DFS(Root->FirstChildElement());
            Root = Root->NextSiblingElement();
        }
    }

    bool RssReader::parseRss(const string &filename)
    {
        XMLDocument doc;
        if (doc.LoadFile(filename.c_str()) != XML_SUCCESS)
        {
            cout << "LoadFile ERROR!" << endl;
            return false;
        }
        //get Root Element
        XMLElement *Root = doc.RootElement();
        DFS(Root);

        return true;
    }

    void RssReader::dump(const string &ripePage_dir, const string &offset_dir, vector<string> &pageLibs, map<int, pair<int, int>> &offsetLib)
    {
        ofstream ofs1(ripePage_dir, ofstream::out);
        if (!ofs1)
        {
            cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " ripePage_dir open ERROR!" << endl;
            exit(-1);
        }

        for (size_t i = 0; i < _rss.size(); i++)
        {
            stringstream ss;
            ss << "<doc>" << endl;
            ss << "\t <title> " << _rss[i].title << " </title>" << endl;
            ss << "\t <link> " << _rss[i].link << " </link>" << endl;
            ss << "\t <description> " << _rss[i].description << " </description>" << endl;
            ss << "\t <content> " << _rss[i].content << " </content>" << endl;
            ss << "</doc>" << endl;
            ss << endl
               << endl
               << endl;

            offsetLib[i].first = ofs1.tellp();
            ofs1 << ss.str();
            offsetLib[i].second = ofs1.tellp();

            pageLibs.push_back(ss.str());
        }
        ofs1.close();

        ofstream ofs2(offset_dir, ofstream::out);
        if (!ofs2)
        {
            cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " offset_dir open ERROR!" << endl;
            exit(-1);
        }
        for (auto &e : offsetLib)
        {
            ofs2 << e.first << " " << e.second.first << " " << e.second.second << endl;
        }
        ofs2.close();
    }

} // namespace wd
