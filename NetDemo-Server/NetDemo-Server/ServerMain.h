#pragma once
#include "../../Common.h"
#include <winsock2.h>
#include <vector>
#include <thread>
#include <string>

#define ROOM_CMD_LEN 9
#define START_ROOM "StartRoom"
#define CLOSE_ROOM "CloseRoom"
#define PAUSE_ROOM "PauseRoom"
#define REUSE_ROOM "ReuseRoom"

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

#pragma region begin chack Input
private:
	bool ifKeepListen;
	string cmdMsg;

	void BeingCheckInput();

	void StartRoom();
	void PauseRoom();
	void ReuseRoom();
	void CloseRoom();

#pragma endregion

#pragma region tool function
	bool GetMsgWithLen(int Len);
	string CutMsgRegion(int Len);

	string CutCMDRegion(int Len);

	void CheckEmptyRoom();
#pragma endregion

};