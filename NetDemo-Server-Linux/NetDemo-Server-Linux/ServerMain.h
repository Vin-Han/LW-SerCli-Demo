#pragma once
#include "Common.h"
#include <sys/socket.h>
#include <vector>
#include <thread>
#include <string>

#define ROOM_CMD_LEN 9
#define START_ROOM "StartRoom"
#define CLOSE_ROOM "CloseRoom"
#define PAUSE_ROOM "PauseRoom"
#define REUSE_ROOM "ReuseRoom"
#define EXITSERVER "OutServer"

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
	int serverSocket;
	sockaddr_in serverAddr;

	void SetServerSocket();
	void BindServerToPort();
	void BeginToListen();
#pragma endregion

#pragma region receive new socket

private:
	int tempSocket;
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
	void OutServer();
#pragma endregion

#pragma region tool function
	bool GetMsgWithLen(int Len);
	string CutMsgRegion(int Len);

	string CutCMDRegion(int Len);
	bool CheckListNumForCMD();

	void CheckEmptyRoom();

#pragma endregion

};