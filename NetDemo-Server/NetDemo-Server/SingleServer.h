#pragma once
#include "../../Common.h"
#include <winsock2.h>

class SingleServer
{
public:
	SingleServer(int ServerID);
	~SingleServer();

private:
	int serverID;
	SOCKET serverSock;
	sockaddr_in serverAddr;

	SOCKET clientSocket;
	Msg tempMsg;

public:
	bool BindServerToPort(int port);

	bool BeginToListen();

	bool AcceptClient();

	bool SendToClient();

	bool RecvFromClient();

	void CloseClientSocket();

private:
	bool RecvMsgCheck();
};

