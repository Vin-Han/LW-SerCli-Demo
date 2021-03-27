#include "ChattingRoom.h"

#include "Client.h"

ChattingRoom::ChattingRoom(int RoomID):roomID(RoomID)
{
	ifOpen = false;
}

ChattingRoom::~ChattingRoom()
{
}

void ChattingRoom::Close()
{
	CheckClientExist();
	for (Client* tempClient : clientList)
	{
		tempClient->Close();
	}
	msgList.clear();
	delete this;
}

void ChattingRoom::RegisteUser(string userName,SOCKET userSocket)
{
	Client* newClient = nullptr;
	CheckClientExist();
	for (Client* tempClient : clientList)
	{
		if (tempClient->userName == userName)
		{
			tempClient->Close();
			tempClient->userSocket = userSocket;
			newClient = tempClient;
		}
	}
	if (newClient == nullptr) {
		newClient = new Client(roomID, clientList.size(), userName, userSocket);
		clientList.push_back(newClient);
	}

	newClient->roomLink = this;
	newClient->Begin();
}

void ChattingRoom::CheckClientExist()
{
	vector<Client*>::iterator curPos = clientList.begin();
	for (; curPos != clientList.end(); curPos++)
	{
		if ((*curPos) == nullptr) {
			curPos = clientList.erase(curPos);
		}
	}
}

