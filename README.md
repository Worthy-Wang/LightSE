`github源码`:[https://github.com/Worthy-Wang/mini_search_engine](https://github.com/Worthy-Wang/mini_search_engine)

-------------


### 项目简介
这是一款基于RSS的搜索引擎，项目展示成果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713090204641.gif)

### 项目架构
1.首先搭建好线程池，启动线程时都在等待任务队列出任务
2.启动封装好的服务器程序，当客户端发来请求时，将此请求封装成一个任务，加入任务队列，让子线程去完成该任务
3.搜索引擎类的封装，对于查询词的计算
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713090409283.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1dvcnRoeV9XYW5n,size_16,color_FFFFFF,t_70)
<br>

### 项目流程
#### 离线版本
1. 建立网页库与网页偏移库
技术亮点：`开源库tinyxml2(DFS遍历节点)`, `正则表达式`，`文件流字符串流操作`, `Unix目录操作`，`单例模式(饿汉)`
2. 网页去重
技术亮点: `开源库cppjieba分词`，`set停词集合`，`TopK算法(判断网页重复)`
3. 建立倒排索引
技术亮点：`unordered_map倒排哈希表`，`TF-IDF算法(计算权重)`



#### 在线部分
1. 搭建线程池框架
技术亮点：`基于对象线程池设计`，`Posix线程类，互斥锁，条件变量`，`任务队列`，`自动加解锁类`，`子线程安全退出机制`，`回调函数`，` 智能指针unique_ptr`
2. 搭建服务器框架
技术亮点：`基于对象服务器设计`，`回调函数`，`socket网络编程`，`IO多路复用epoll`，`eventfd(IO线程与计算线程分离)`，`客户端安全退出机制` ，`智能指针shared_ptr`
3. 搭建搜索引擎框架
技术亮点：`文件流字符串流操作`，`TF-IDF算法`，`余弦相似度算法`，`开源库jsoncpp`


#### 优化功能
1. 使用mysql作为网页存储的数据库
2. 使用redis作为Cache缓存
