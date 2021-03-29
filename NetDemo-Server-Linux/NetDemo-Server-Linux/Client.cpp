#include "Client.h"

#include "ChattingRoom.h"

Client::Client(int RoomID, int ID, string Name, int Socket, int curPos):userRoom(RoomID),userID(ID),userName(Name),userSocket(Socket), userCurPos(curPos)
{
	{
		setsockopt(userSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&MAX_RECV_TIME, sizeof(int));
		setsockopt(userSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&MAX_SEND_TIME, sizeof(int));
	}

	clientMsg = "";

	ifKeepConnect = false;
	ifSendMessage = false;
	ifSendBeatBag = false;
	ifFirstClose = true;
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

	beatThread = new thread(BeatThread, this);
	beatThread->detach();
}
void Client::Close()
{
	if (ifFirstClose)
	{
		ifFirstClose = false;
	}
	ifKeepConnect = false;
	close(userSocket);
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

void Client::BeginRecv()
{
	while (ifKeepConnect)
	{
		if (RecvClientMsg() == false)ifKeepConnect = false;
	}
}
void Client::BeginSend()
{
	while (ifKeepConnect)
	{
		if (SendClientMsg() == false)ifKeepConnect = false;
	}
}
void Client::BeginBeat()
{
	while (ifKeepConnect)
	{
		BeatClientMsg();
	}
}

bool Client::RecvClientMsg()
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
	else
	{
		Close();
		return false;
	}
	clientMsg = "";
	return true;
}
bool Client::SendClientMsg()
{
	CheckCurPos();
	if (ifSendMessage)
	{
		if (SendMsgToClient() == false) {
			Close();
			return false;
		}

	}
	else if (ifSendBeatBag)
	{
		if (SendBeatToClient() == false) {
			Close();
			return false;
		}
	}
	return true;
}
void Client::BeatClientMsg()
{
	ifSendBeatBag = true;
	sleep(3000);
}

void Client::CheckCurPos()
{
	if (userCurPos < roomLink->msgList.size())
	{
		ifSendMessage = true;
	}
}
bool Client::SendMsgToClient()
{
	string fullMsg = roomLink->msgList[userCurPos];
	if (send(userSocket, CMD_SEND, CMD_LEN, 0) == -1)return false;

	string msgLen = IntToStringID(fullMsg.size());
	if (send(userSocket, msgLen.c_str(), NUM_LEN, 0) == -1)return false;
	if (send(userSocket, fullMsg.c_str(), fullMsg.size(), 0) == -1)return false;

	userCurPos++;
	ifSendMessage = false;
	ifSendBeatBag = false;
	return true;
}
bool Client::SendBeatToClient()
{
	if (send(userSocket, CMD_BEAT, CMD_LEN, 0) == -1)return false;
	ifSendBeatBag = false;
	return true;
}

bool Client::GetMsgWithLen(int Len)
{
	while (clientMsg.size() < Len)
	{
		char* tempID = new char[100]{ 0 };
		int error = recv(userSocket, tempID, BUFFER_MAX_LENG, 0);
		if (error == -1 )
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