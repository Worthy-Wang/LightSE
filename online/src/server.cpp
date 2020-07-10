#include "Configuration.h"
#include "WordQuery.h"
#include "./net/TcpServer.h"
#include "./threadpool/Threadpool.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <functional>
using std::bind;
using std::cout;
using std::endl;
using std::ifstream;
using std::set;
using std::stringstream;
using namespace wd;
Configuration *Configuration::_pInstance = new Configuration("/home/wwx/mini_search_engine/online/conf/setting.conf"); //饿汉模式，线程安全
WordQuery wordQuery(Configuration::getpInstance()->getConfigMap()["stop_words.utf8"], Configuration::getpInstance()->getConfigMap()["NewRipePage_dir"], Configuration::getpInstance()->getConfigMap()["Newoffset_dir"], Configuration::getpInstance()->getConfigMap()["invertIndex_dir"]);
Threadpool *pthreadpool = NULL; //定义全局变量：线程池

void ConnectionCallBack(const TcpConnectionPtr &ptr)
{
    cout << ">>client has connected " << ptr->Address() << endl;
    // ptr->send("welcome to server");
}

void MessageCallBack(const TcpConnectionPtr &ptr)
{
	string msg(ptr->recv());
	size_t pos = msg.find('\n');
	msg = msg.substr(0, pos);
	cout << "client:" << msg << ",size:" << msg.size() << endl;
    //查询网页的任务交给子线程
    pthreadpool->addTask(std::bind(&WordQuery::doQuery, &wordQuery, msg, ptr));
}

void CloseCallBack(const TcpConnectionPtr &ptr)
{
    cout << ">>client has broken up " << ptr->Address() << endl;
}

int main()
{
    //启动线程池，用于处理任务
    Threadpool threadpool(stoi(Configuration::getpInstance()->getConfigMap()["threadNum"]), stoi(Configuration::getpInstance()->getConfigMap()["TaskQueSize"]));
    pthreadpool = &threadpool;
    pthreadpool->start();

    //启动服务器
    TcpServer server(Configuration::getpInstance()->getConfigMap()["IP"], stoi(Configuration::getpInstance()->getConfigMap()["port"]));
    server.setConnectionCallBack(ConnectionCallBack);
    server.setMessageCallBack(MessageCallBack);
    server.setCloseCallBack(CloseCallBack);
    server.start();
    return 0;
}