#pragma once

#include <iostream>
#include <winsock2.h>

#define LISTEN_MAX_LIST 10 
#define BUFFER_MAX_LENG 100

#define EMPTY_MESSAGE "bytedance_work"
#define EMPTY_MESSAGE_LEN 14

using namespace std;

static int SOCKET_ADDR_LENGTH = sizeof(SOCKADDR);

static void LogMsg(string Msg) {
	cout << Msg << endl;
}

class Msg {
public:
	~Msg() {
		delete[] msg;
	}
public:
	char  msg[BUFFER_MAX_LENG] = { 0 };
	int   msgLen = 0;
	void ClearMsg() {
		msg[BUFFER_MAX_LENG] = { 0 };
		msgLen = 0;
	}
};


/*
bind 返回值
EADDRINUSE	给定地址已经使用
EBADF	sockfd不合法
EINVAL	sockfd已经绑定到其他地址
ENOTSOCK	sockfd是一个文件描述符，不是socket描述符
EACCES	地址被保护，用户的权限不足
EADDRNOTAVAIL	接口不存在或者绑定地址不是本地	UNIX协议族，AF_UNIX
EFAULT	my_addr指针超出用户空间	UNIX协议族，AF_UNIX
EINVAL	地址长度错误，或者socket不是AF_UNIX族	UNIX协议族，AF_UNIX
ELOOP	解析my_addr时符号链接过多	UNIX协议族，AF_UNIX
ENAMETOOLONG	my_addr过长	UNIX协议族，AF_UNIX
ENOENT	文件不存在	UNIX协议族，AF_UNIX
ENOMEN	内存内核不足	UNIX协议族，AF_UNIX
ENOTDIR	不是目录	UNIX协议族，AF_UNIX
EROFS	socket节点应该在制度文件系统上	UNIX协议族，AF_UNIX
*/