#pragma once

#include "../../Common.h"

#include <winsock2.h>
#include <string>
#include <thread>

using namespace std;

class ChattingRoom;

class Client
{
public:
	Client(int RoomID, int ID, string Name, SOCKET Socket);
	~Client();

	void Begin();
	void Close();

	ChattingRoom* roomLink;

	string userName;
	int    userID;
	SOCKET userSocket;
	int    userRoom;
	int    userCurPos;




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

	void RecvClientMsg();
	void SendClientMsg();
	void BeatClientMsg();

	void CheckCurPos();
	void SendMsgToClient();
	void SendBeatToClient();
#pragma endregion

#pragma region tools function
private:
	bool GetMsgWithLen(int Len);
	string CutMsgRegion(int Len);
#pragma endregion


};