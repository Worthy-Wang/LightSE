


### 项目简介

该项目是一个用于资料查询的`轻量级搜索引擎`，旨在为公司的产品资料和档案提供便利的查询功能。
* 项目架构

服务器端基于`Linux` 平台、`C/C++`语言；客户端采用`PHP` 实现；爬虫采用`Python` 实现。

* 项目描述

该项目是一个用于资料查询的轻量级搜索引擎。实现了从`爬取网页`，`处理网页`，`显示结果`等整个搜索引擎的流程。线下部分使用`tinyxml2`解析爬取的网页，建立`网页库`，网页`偏移库`，使用`cppjieba`分词，`TopK算法`结合`指纹信息算法`判断网页重复，去重后通过`TF-IDF算法`建立`倒排索引库`。线上部分则使用`MirrorLib框架`搭建服务器，客户端发送查询词后，计算`文本特征`并建立`VSM模型`，并使用`余弦相似度算法`匹配网页，使用`jsoncpp`封装后发送。后续优化则使用`mysql`作为网页存储的数据库，`redis`作为`Cache缓存`，`log4cpp`添加`日志系统`。
项目展示成果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200713090204641.gif)

### 项目架构图

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
（前两步框架可直接使用博主的开源项目 [MirrorLib](https://github.com/Worthy-Wang/MirrorLib)）
3. 搭建搜索引擎框架
技术亮点：`文件流字符串流操作`，`TF-IDF算法`，`开源库log4cpp记录日志`，`余弦相似度算法`，`开源库jsoncpp`


#### 优化功能
1. 使用mysql作为网页存储的数据库
2. 使用redis作为Cache缓存

<br>

#### 算法详解
如果对于上述算法感觉不是很理解，可以看看这一篇文章：
[基于 K-means 算法实现的文本聚类（干货）](https://blog.csdn.net/qq_29110265/article/details/90769363)
