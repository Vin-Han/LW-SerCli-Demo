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
bind ����ֵ
EADDRINUSE	������ַ�Ѿ�ʹ��
EBADF	sockfd���Ϸ�
EINVAL	sockfd�Ѿ��󶨵�������ַ
ENOTSOCK	sockfd��һ���ļ�������������socket������
EACCES	��ַ���������û���Ȩ�޲���
EADDRNOTAVAIL	�ӿڲ����ڻ��߰󶨵�ַ���Ǳ���	UNIXЭ���壬AF_UNIX
EFAULT	my_addrָ�볬���û��ռ�	UNIXЭ���壬AF_UNIX
EINVAL	��ַ���ȴ��󣬻���socket����AF_UNIX��	UNIXЭ���壬AF_UNIX
ELOOP	����my_addrʱ�������ӹ���	UNIXЭ���壬AF_UNIX
ENAMETOOLONG	my_addr����	UNIXЭ���壬AF_UNIX
ENOENT	�ļ�������	UNIXЭ���壬AF_UNIX
ENOMEN	�ڴ��ں˲���	UNIXЭ���壬AF_UNIX
ENOTDIR	����Ŀ¼	UNIXЭ���壬AF_UNIX
EROFS	socket�ڵ�Ӧ�����ƶ��ļ�ϵͳ��	UNIXЭ���壬AF_UNIX
*/