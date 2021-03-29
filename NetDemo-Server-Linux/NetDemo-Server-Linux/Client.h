#pragma once

#include "Common.h"

#include <string>
#include <thread>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

class ChattingRoom;

class Client
{
#pragma region normal functions
public:
	Client(int RoomID, int ID, string Name, int Socket, int curPos = 0);
	~Client();

	void Begin();
	void Close();
#pragma endregion

#pragma region properties
public:
	ChattingRoom* roomLink;
	int userSocket;
	string userName;
	int    userID;
	int    userCurPos;
private:
	int    userRoom;
	bool   ifFirstClose;
#pragma endregion

#pragma region thread function
public:
	string  clientMsg;

	void BeginRecv();
	void BeginSend();
	void BeginBeat();
private:
	bool ifKeepConnect;
	bool ifSendMessage;
	bool ifSendBeatBag;

	thread* recvThread;
	thread* sendThread;
	thread* beatThread;

	static void RecvThread(Client* client);
	static void SendThread(Client* client);
	static void BeatThread(Client* client);

	bool RecvClientMsg();
	bool SendClientMsg();
	void BeatClientMsg();

	void CheckCurPos();
	bool SendMsgToClient();
	bool SendBeatToClient();
#pragma endregion

#pragma region tools function
private:
	bool GetMsgWithLen(int Len);
	string CutMsgRegion(int Len);
#pragma endregion

};