#pragma once

#include <winsock2.h>
#include <deque>
#include <thread>
#include <string>

using namespace std;

class Msg;
class MsgCheckPoint;

class SingleClient
{
#pragma region single mode function
private:
	SingleClient(string IPAddr = nullptr, int port = 0);
	static SingleClient* singleClient;
public:
	static SingleClient* GetClientInstance();
	~SingleClient();

	void Begin();
	void Close();
#pragma endregion

#pragma region client common properties
public:
	Msg* sendMsg;
	string curMsg;

private:
	int roomNum;
	int userID;
	string userName;
	sockaddr_in serverAddr;
	SOCKET* clientSocket;
#pragma endregion

#pragma region set room number
public:
	bool GetRoomNumber();
private:
	bool LinkToServer();
	bool LinkToRoom();
#pragma endregion

#pragma region set user name
public:
	bool GetUserNameID();

private:
	bool SetUserID();

#pragma endregion

#pragma region begin chatting
public:
	void BeginChatToServer();

	void RecvMessageFromServer();
	void SendMessageToServer();
	void CheackHeartBagTime();

	bool ifHeartBag;
	bool ifSendMsg;
	bool ifKeepChatting;

	deque<string> msgList;
private:
	thread* recvThread;
	thread* hearThread;
	thread* sendThread;

	static void RecvThread();
	static void HearThread();
	static void SendThread();

	void SendContentToServer();
	void SendHeartBagToServer();
#pragma endregion

#pragma region tools function
	bool GetMsgWithLen(int Len);
	string CutMsgRegion(int Len);
#pragma endregion
};