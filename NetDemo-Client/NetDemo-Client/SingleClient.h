#pragma once

#include <winsock2.h>
#include "ConsoleCtr.h"
#include <vector>

class Msg;
class MsgCheckPoint;

class SingleClient
{
#pragma region single mode function
private:
	SingleClient(char* IPAddr = nullptr, int port = 0);
	static SingleClient* singleClient;
public:
	static SingleClient* GetClientInstance(char* IPAddr = nullptr, int port = 0);
	~SingleClient();
#pragma endregion

#pragma region normal connect
private:
	sockaddr_in serverAddr;
	SOCKET* clientSocket;
	Msg* recvMsg;
	MsgCheckPoint* MsgMachine;
public:
	Msg* sendMsg;

	void BeginChatting();

	bool ConnectToServer();

	bool SendToServer();

	bool RecvFromServer();

	void CloseSocket();
#pragma endregion

#pragma region set Name
public:
	void UserLogin();

private:
	int userID;
	string userName;
	void SetUserName();
	int GetUserID();
#pragma endregion


};