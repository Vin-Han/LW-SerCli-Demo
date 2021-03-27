#pragma once
#include "../../Common.h"
#include <winsock2.h>
#include <vector>
#include <thread>

using namespace std;

class ChattingRoom;

class ServerMain
{
#pragma region single mode
public:
	~ServerMain();
	static ServerMain* GetInstance();
private:
	static ServerMain* instance;
	ServerMain();
#pragma endregion

#pragma region begin server socket thread
public:
	void Begin();
	void Close();

	static void ListenThread();

	void BeginListenThread();

private:
	int serverPort;
	thread* listenThread;
	SOCKET serverSocket;
	sockaddr_in serverAddr;

	void SetServerSocket();
	void BindServerToPort();
	void BeginToListen();
#pragma endregion


#pragma region receive new socket

private:
	SOCKET tempSocket;
	string tempMsg;

	void GetUserRoomNum();
	void RegisteUser(int roomIndex);
#pragma endregion


#pragma region chatting rooom
private:
	vector<ChattingRoom*> roomList;
#pragma endregion

#pragma region tool function
	bool GetMsgWithLen(int Len);
	string CutMsgRegion(int Len);
#pragma endregion

};