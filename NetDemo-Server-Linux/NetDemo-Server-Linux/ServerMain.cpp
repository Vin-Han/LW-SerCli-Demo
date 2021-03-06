#include "ServerMain.h"

#include "ChattingRoom.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

ServerMain* ServerMain::instance = nullptr;

//--------------------------------------------------------------------------------------------//
ServerMain::ServerMain()
{
	ifKeepListen = false;
}
ServerMain::~ServerMain(){}
ServerMain* ServerMain::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ServerMain();
	}
	return instance;
}

//-----------------------------------------------------//
void ServerMain::Begin()
{
	ifKeepListen = true;

	GetInputInt(&serverPort, "Use Witch Port For Server:");

	listenThread = new thread(ListenThread);
	listenThread->detach();

	BeingCheckInput();
}
void ServerMain::Close()
{
	close(serverSocket);
}

void ServerMain::ListenThread()
{
	ServerMain::GetInstance()->BeginListenThread();
	return;
}
void ServerMain::BeginListenThread()
{
	SetServerSocket();
	BindServerToPort();
	BeginToListen();
	while (ifKeepListen)
	{
		GetUserRoomNum();
	}
}

void ServerMain::SetServerSocket()
{
	serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(serverPort);
}
void ServerMain::BindServerToPort()
{
	int error = bind(serverSocket, (sockaddr*)&serverAddr, sizeof(sockaddr));
	if (error != 0)
		LogMsg("Server Bind Error");
}
void ServerMain::BeginToListen()
{
	int error = listen(serverSocket, LISTEN_MAX_LIST);
	if (error == -1)
		LogMsg("Server Listen Error");
}

void ServerMain::GetUserRoomNum()
{
	tempSocket = accept(serverSocket, (sockaddr*)&serverAddr, &SOCKET_ADDR_LENGTH);
	if (GetMsgWithLen(CMD_LEN)) 
	{
		if (CutMsgRegion(CMD_LEN) == CMD_ROOM) 
		{
			if (GetMsgWithLen(NUM_LEN))
			{
				int roomNum = stoi(CutMsgRegion(NUM_LEN));
				CheckEmptyRoom();
				for (int i = 0; i < roomList.size(); i++)
				{
					if (roomList[i]->roomID == roomNum)
					{
						if (roomList[i]->ifOpen == false)
						{
							send(tempSocket, CMD_CLOS, CMD_LEN, 0);
						}
						else
						{
							send(tempSocket, CMD_OPEN, CMD_LEN, 0);
							RegisteUser(i);
						}
						return;;
					}
				}
				send(tempSocket, CMD_CLOS, CMD_LEN, 0);
			}
		}
	}
	tempMsg = "";
}
void ServerMain::RegisteUser(int roomIndex)
{
	if (GetMsgWithLen(CMD_LEN)) 
	{
		string tempCMD = CutMsgRegion(CMD_LEN);
		if (tempCMD == CMD_REGI)
		{
			if (GetMsgWithLen(NUM_LEN))
			{
				int nameLen = stoi(CutMsgRegion(NUM_LEN));
				if (GetMsgWithLen(nameLen))
				{
					string userName = CutMsgRegion(nameLen);
					roomList[roomIndex]->RegisteUser(userName, tempSocket);
				}
			}
		}
	}
}

void ServerMain::BeingCheckInput()
{
	while (ifKeepListen)
	{
		GetInputString(cmdMsg, "Give Your CMD Command : ");
		string cmdType = CutCMDRegion(ROOM_CMD_LEN);
		if (cmdType == START_ROOM)
		{
			StartRoom();
		}
		else if (cmdType == PAUSE_ROOM)
		{
			PauseRoom();
		}
		else if (cmdType == REUSE_ROOM)
		{
			ReuseRoom();
		}
		else if (cmdType == CLOSE_ROOM)
		{
			CloseRoom();
		}
		else if (cmdType == EXITSERVER)
		{
			OutServer();
		}
		else
		{
			cout << "Can't Tecogine This CMD" << endl;
		}
	}
}

void ServerMain::StartRoom()
{
	if (CheckListNumForCMD() == false)
	{
		cout << "Room Number is worry" << endl;
		return;
	}
	int roomNum = stoi(CutCMDRegion(NUM_LEN));
	CheckEmptyRoom();
	for (ChattingRoom* tempRoom : roomList)
	{
		if (tempRoom->roomID == roomNum)
		{
			cout << "Room is exist now" << endl;
			return;
		}
	}
	ChattingRoom* newRoom = new ChattingRoom(roomNum);
	newRoom->ifOpen = true;
	roomList.push_back(newRoom);
}
void ServerMain::CloseRoom()
{
	if (CheckListNumForCMD() == false)
	{
		cout << "Room Number is worry" << endl;
		return;
	}
	int roomNum = stoi(CutCMDRegion(NUM_LEN));
	CheckEmptyRoom();

	for (ChattingRoom* tempRoom : roomList)
	{
		if (tempRoom->roomID == roomNum)
		{
			tempRoom->Close();
			tempRoom = nullptr;
			return;
		}
	}
	cout << "Room is not exist now" << endl;
	return;
}
void ServerMain::PauseRoom()
{
	if (CheckListNumForCMD() == false)
	{
		cout << "Room Number is worry" << endl;
		return;
	}
	int roomNum = stoi(CutCMDRegion(NUM_LEN));
	CheckEmptyRoom();
	for (ChattingRoom* tempRoom : roomList)
	{
		if (tempRoom->roomID == roomNum)
		{
			tempRoom->ifOpen = false;
			return;
		}
	}
}
void ServerMain::ReuseRoom()
{
	if (CheckListNumForCMD() == false)
	{
		cout << "Room Number is worry" << endl;
		return;
	}
	int roomNum = stoi(CutCMDRegion(NUM_LEN));
	CheckEmptyRoom();
	for (ChattingRoom* tempRoom : roomList)
	{
		if (tempRoom->roomID == roomNum)
		{
			tempRoom->ifOpen = true;
			return;
		}
	}
}
void ServerMain::OutServer()
{
	exit(0);
}

//-----------------------------------------------------//
bool ServerMain::GetMsgWithLen(int Len)
{
	while (tempMsg.size() < Len)
	{
		char* tempID = new char[100]{ 0 };
		int error = recv(tempSocket, tempID, BUFFER_MAX_LENG, 0);
		if (error == 0)
			return false;
		tempMsg += tempID;
	}
	return true;
}
string ServerMain::CutMsgRegion(int Len)
{
	string result = string(tempMsg, 0, Len);
	tempMsg = string(tempMsg, Len, tempMsg.size());
	return result;
}

string ServerMain::CutCMDRegion(int Len)
{
	string result = string(cmdMsg, 0, Len);
	cmdMsg = string(cmdMsg, Len, cmdMsg.size());
	return result;
}
bool ServerMain::CheckListNumForCMD()
{
	if (cmdMsg.size() < 4)
	{
		return false;
	}
	return true;
}

void ServerMain::CheckEmptyRoom()
{
	vector<ChattingRoom*>::iterator curPos = roomList.begin();
	for (; curPos != roomList.end(); curPos++)
	{
		if ((*curPos) == nullptr)
		{
			curPos = roomList.erase(curPos);
		}
	}
}

