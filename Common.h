#pragma once

#include <iostream>
#include <winsock2.h>

#define LISTEN_MAX_LIST 10 
#define BUFFER_MAX_LENG 100

#define EMPTY_MESSAGE "bytedance_work"
#define EMPTY_MESSAGE_LEN 14

#define CMD_HEAD "cmd:"
#define CMD_HEAD_LEN 4

#define USER_LOGIN "new_user_login"
#define USER_LOGIN_LEN 14

using namespace std;

static int SOCKET_ADDR_LENGTH = sizeof(SOCKADDR);

static void LogMsg(string Msg) {
	cout << Msg << endl;
}

class Msg {
public:
	~Msg() {
		
	}
public:
	char  msg[BUFFER_MAX_LENG] = { 0 };
	int   msgLen = 0;
	void ClearMsg() {
		memset(msg, 0, BUFFER_MAX_LENG);
		msgLen = 0;
	}
};

#pragma region string enpand function

#pragma endregion

static bool StringBeginWith(const std::string& origin, const std::string& head) {
	return (!origin.compare(0, head.length(), head));
}

static bool StringEndWith(const std::string& origin, const std::string& tail) {
	if (origin.length() >= tail.length()) {
		return (0 == origin.compare(origin.length() - tail.length(), tail.length(), tail));
	}
	return false;
}

static void GetConsoleXY(int& X, int& Y)
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfor;
	HANDLE cursorInfor = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(cursorInfor, &bufferInfor);
	X = bufferInfor.dwCursorPosition.X;
	Y = bufferInfor.dwCursorPosition.Y;
}

static void SetConSoleXY(int X, int Y)
{
	COORD cursorPos = { X,Y };
	HANDLE cursorInfor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(cursorInfor, cursorPos);
}



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