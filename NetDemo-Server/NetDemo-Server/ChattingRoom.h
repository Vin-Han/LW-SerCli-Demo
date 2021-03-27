#pragma once

#include <vector>
#include <string>
#include <winsock2.h>
using namespace std;

class Client;

class ChattingRoom
{
public:
	ChattingRoom(int RoomID);
	~ChattingRoom();

	void RegisteUser(string userName,SOCKET userSocket);

	bool ifOpen;

	int roomID;

	vector<Client*> clientList;
	vector<string>  msgList;

};
