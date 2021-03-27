#include "ChattingRoom.h"

#include "Client.h"

ChattingRoom::ChattingRoom(int RoomID):roomID(RoomID)
{
}

ChattingRoom::~ChattingRoom()
{
}

void ChattingRoom::RegisteUser(string userName,SOCKET userSocket)
{
	Client* newClient = nullptr;
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

