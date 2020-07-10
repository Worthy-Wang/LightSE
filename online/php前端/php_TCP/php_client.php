<?php
/*
*Author:yuandong
Date:2014-4-25
Description:用php写的udp客户端，该客户端只是一个媒介作用，目的是把
浏览器通过post请求传递的关键字通过udp协议传给server端(C语言写的),
因为浏览器不能直接把关键字发给server

*****************************************************/

$buff=$_REQUEST["content"];//采用$_REQUEST超全局数组来接收index.html页面post请求传递过来的数据
//$buff="王道\n";//测试
//tcp  client 
$serverIp="172.21.0.7";//服务端ip地址，如果你的客户端与服务端不在同一台电脑，请修改该ip地址
$serverPort=8888;//通信端口号
//设置超时时间
set_time_limit(0);
//创建套接字
 $sock= socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if(!$sock) {
    echo "creat sock failed";
    exit();//创建套接字失败，结束程序
}
//发送数据到TCP的服务端
$connection = socket_connect($sock, $serverIp, $serverPort );
if($connection< 0) {
	echo "socket_connect failed!";
}
else {
	// echo "connection OK\n";
}

if(!socket_write($sock, $buff)) {
	echo "socket_write() failed: reason:".socket_strerror($sock)."\n";
	exit();
}

{
	// echo "发送成功，内容为:";
	// echo $buff;
}

$slen = socket_read($sock, 1024, PHP_NORMAL_READ);
//echo $slen."====\n";
$len = intval($slen);
//echo "len = ".$len."----\n";

$buff = "";//清空缓冲区
socket_recv($sock, $buff, $len, MSG_WAITALL);

echo trim($buff)."\n";//去掉接受到的字符串的首尾空格，返回给post请求的data
//关闭套接字
socket_close($sock);
?>

