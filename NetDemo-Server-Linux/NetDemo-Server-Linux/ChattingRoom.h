#pragma once

#include <vector>
#include <string>
#include <sys/socket.h>
using namespace std;

class Client;

class ChattingRoom
{
public:
	ChattingRoom(int RoomID);
	~ChattingRoom();

	void Close();

	void RegisteUser(string userName,int userSocket);

	bool ifOpen;
	int roomID;

	vector<Client*> clientList;
	vector<string>  msgList;

private:
	void CheckClientExist();
};
