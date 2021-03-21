#pragma once
#include "../../Common.h"
#include <winsock2.h>
#include <vector>
#include <map>

class SingleServer
{
public:
	SingleServer(int ServerID);
	~SingleServer();

#pragma region normal chatting room
public:
	void InitChattingRoom(int port);
	void OpenChattingRoom();
	void CloseChattingRoom();

private:
	SOCKET* serverSocket;
	SOCKET clientSocket;
	sockaddr_in serverAddr;
	bool ifOpenChatting;
	Msg tempMsg;
	vector<string> msgList;
	int serverID;

	void SetServerSocket();
	bool BindServerToPort(int port);
	bool BeginToListen();
	bool AcceptClient();
	void RecvFromClient();
	void CloseClientSocket();
#pragma endregion

#pragma region msg check
private:
	void RecvMsgCheck();
#pragma endregion

#pragma region user login
private:
	map<string, int> userList;
	vector<string> userNameList;
	int userCount;
	void AddNewUser();
#pragma endregion

#pragma region get all msg
private:
	vector<int> userCurPos;
	void GetLastMsg();
#pragma endregion

#pragma region get user new Message
	void AddNewMessage();
#pragma endregion

};

