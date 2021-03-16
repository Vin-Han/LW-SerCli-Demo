#pragma once
#include "../../Common.h"
#include <winsock2.h>
#include "ConsoleCtr.h"


class SingleClient
{
private:
	SingleClient();
	static SingleClient* singleClient;
public:
	static SingleClient* GetClientInstance();
	~SingleClient();



private:
	sockaddr_in serverAddr;
	ConsoleCtr* conSoleInstance;
	SOCKET* clientSocket;
	Msg* sendMsg;
	Msg* recvMsg;
public:

	static inline void InputOver_Handler(){ ifUserInputOver = true; }
	static bool ifUserInputOver;

	bool ConnectToServer();

	bool SendToServer();
	
	bool RecvFromServer();

	void CloseSocket();

	bool CheckPlayerInput();
};