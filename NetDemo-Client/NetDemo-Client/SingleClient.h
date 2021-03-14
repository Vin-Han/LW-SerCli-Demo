#pragma once
#include "../../Common.h"
#include <winsock2.h>



class SingleClient
{
public:
	SingleClient(string name);
	~SingleClient();

private:
	string clientName;
	sockaddr_in serverAddr;
	
	SOCKET* clientSocket;
	Msg* sendMsg;
	Msg* recvMsg;

public:
	bool ConnectToServer();

	bool SendToServer();
	
	bool RecvFromServer();

	void CloseSocket();
};