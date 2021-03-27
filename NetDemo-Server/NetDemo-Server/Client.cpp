#include "Client.h"

#include "ChattingRoom.h"

Client::Client(int RoomID, int ID, string Name, SOCKET Socket):userRoom(RoomID),userID(ID),userName(Name),userSocket(Socket)
{
	userCurPos = -1;
	{
		setsockopt(userSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&MAX_RECV_TIME, sizeof(int));
		setsockopt(userSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&MAX_SEND_TIME, sizeof(int));
	}

	clientMsg = "";

	ifKeepConnect = false;
}

Client::~Client()
{
}

void Client::Begin()
{
	ifKeepConnect = true;

	string tempIDS = IntToStringID(userID);
	send(userSocket, tempIDS.c_str(), NUM_LEN, 0);

	recvThread = new thread(RecvThread,this);
	recvThread->detach();

	sendThread = new thread(SendThread, this);
	sendThread->detach();
}

void Client::Close()
{
	ifKeepConnect = false;
	closesocket(userSocket);
}

void Client::BeginRecv()
{
	while (ifKeepConnect)
	{
		RecvClientMsg();
	}
}

void Client::BeginSend()
{
	while (ifKeepConnect)
	{
		SendClientMsg();
	}
}

void Client::BeginBeat()
{
	while (ifKeepConnect)
	{
		BeatClientMsg();
	}
}

void Client::RecvThread(Client* client)
{
	client->BeginRecv();
	return;
}
void Client::SendThread(Client* client)
{
	client->BeginSend();
	return;
}
void Client::BeatThread(Client* client)
{
	client->BeginBeat();
	return;
}

void Client::RecvClientMsg()
{
	if (GetMsgWithLen(CMD_LEN))
	{
		string tempCMD = CutMsgRegion(CMD_LEN);
		if (tempCMD == CMD_SEND)
		{
			if (GetMsgWithLen(NUM_LEN))
			{
				int msgLen = stoi(CutMsgRegion(NUM_LEN));
				if (GetMsgWithLen(msgLen))
				{
					string fullMsg = userName + ":" + CutMsgRegion(msgLen);
					roomLink->msgList.push_back(fullMsg);
				}
			}
		}
		else if (tempCMD != CMD_BEAT)
		{
			LogMsg("Message From Client is Worry :");
			LogMsg(tempCMD);
		}
	}
	clientMsg = "";


}
void Client::SendClientMsg()
{
	CheckCurPos();
	if (ifSendMessage)
	{
		SendMsgToClient();
	}
	else if (ifSendBeatBag)
	{
		SendBeatToClient();
	}
}
void Client::BeatClientMsg()
{
	ifSendBeatBag = true;
	Sleep(3000);
}


void Client::CheckCurPos()
{
	if (userCurPos < roomLink->msgList.size())
	{
		ifSendMessage = true;
	}
}
void Client::SendMsgToClient()
{
	string fullMsg = roomLink->msgList[userCurPos];
	send(userSocket, CMD_SEND, CMD_LEN, 0);

	string msgLen = IntToStringID(fullMsg.size());
	send(userSocket, msgLen.c_str(), NUM_LEN, 0);
	send(userSocket, fullMsg.c_str(), fullMsg.size(), 0);

	userCurPos++;
	ifSendMessage = false;
	ifSendBeatBag = false;
}
void Client::SendBeatToClient()
{
	send(userSocket, CMD_BEAT, CMD_LEN, 0);
	ifSendBeatBag = false;
}


bool Client::GetMsgWithLen(int Len)
{
	while (clientMsg.size() < Len)
	{
		char* tempID = new char[100]{ 0 };
		int error = recv(userSocket, tempID, BUFFER_MAX_LENG, 0);
		if (error == 0)
			return false;
		clientMsg += tempID;
	}
	return true;
}

string Client::CutMsgRegion(int Len)
{
	string result = string(clientMsg, 0, Len);
	clientMsg = string(clientMsg, Len, clientMsg.size());
	return result;
}