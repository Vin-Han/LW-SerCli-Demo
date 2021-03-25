#pragma once
#include "../../Common.h"
#include <winsock2.h>

enum CMDList;

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

#pragma region server socket
public:
	void BeginListen(int port);
	static SOCKET serverSocket;
private:
	sockaddr_in serverAddr;
	void SetServerSocket(int port);
	void BindServerToPort();
	void BeginToListen();
#pragma endregion

#pragma region command List
public:
	CMDList curCMD;

#pragma endregion



};

enum CMDList
{
	None = 0,
	OpenRoom = 1,
	CloseRoom = 2,
	PauseRoom = 3,
};