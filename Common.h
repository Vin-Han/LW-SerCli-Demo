#pragma once

#include <iostream>
#include <winsock2.h>
#include <string>

using namespace std;

#define LISTEN_MAX_LIST 10
#define BUFFER_MAX_LENG 100

#define MSG_HEAD_LEN 14
#define USER_ID_LEN 4
#define USER_MSG_LEN 4
#define COMMON_PORT 1240

// ����Э���ͷ //
#define CMD_LEN 8
// ������Ϣ���� //
#define NUM_LEN 4

// ����������
static const char* CMD_ROOM = "intoroom";
// ������
static const char* CMD_BEAT = "heartbag";
// ע���û�
static const char* CMD_REGI = "new_user";

// ������Ϣ
static const char* CMD_SEND = "send_msg";

// ����ر�
static const char* CMD_OPEN = "roomopen";
// ���俪��
static const char* CMD_CLOS = "roomclos";


// �����ȡʱ��
static const int MAX_RECV_TIME = 10 * 1000;
// �����ʱ��
static const int MAX_SEND_TIME = 10 * 1000;



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

static void GetInputInt(int* target,const string& inforMsg) {
	cout << inforMsg << endl;
	cin >> *target;
	while (cin.fail() == true)
	{
		cout << inforMsg << endl;
		cin.clear();
		cin.get();
		cin.sync();
		cin >> *target;
	}
}

static void GetInputString(string& target, const string& inforMsg) {
	cout << inforMsg << endl;
	cin >> target;
	while (cin.fail() == true)
	{
		cout << inforMsg << endl;
		cin.clear();
		cin.get();
		cin.sync();
		cin >> target;
	}
}

static string IntToStringID(int ID, int len = 4)
{
	string target = to_string(ID);
	while (target.size() < len)
	{
		target = "0" + target;
	}
	return target;
}