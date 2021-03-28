#include "SingleClient.h"
#include "../../Common.h"

#include <iostream>
#include <conio.h>
#include <algorithm>
#include <winsock2.h>
#include <cstdlib>

SingleClient* SingleClient::singleClient = nullptr;

//--------------------------------------------------------------------------------------------//
SingleClient* SingleClient::GetClientInstance()
{
    if (singleClient == nullptr) {
        string IPAddr;
        GetInputString(IPAddr, "Put Server IP Address :");

        int portNum;
        GetInputInt(&portNum, "Put Server Listen Port :");

        singleClient = new SingleClient(IPAddr, portNum);
    }
    return singleClient;
}

SingleClient::SingleClient(string IPAddr, int port)
{
    clientSocket = new SOCKET(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP));
    {
        setsockopt(*clientSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&MAX_RECV_TIME, sizeof(int));
        setsockopt(*clientSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&MAX_SEND_TIME, sizeof(int));
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    {
        const char* ipAddr = IPAddr.c_str();
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(ipAddr);
        serverAddr.sin_port = htons(port);
    }

    curMsg = "";

    ifKeepChatting = false;
    ifSendMsg = false;
    ifHeartBag = false;

    curPos = 0;
    msgList.clear();
    sendMsg = new Msg();
}

SingleClient::~SingleClient()
{
}

void SingleClient::Begin()
{
    if (GetRoomNumber() == false) Close();
    if (GetUserNameID() == false) Close();
    BeginChatToServer();
}

void SingleClient::Close()
{
    ifKeepChatting = false;
    closesocket(*clientSocket);
    delete clientSocket;
    exit(0);
}

//-----------------------------------------------------//
bool SingleClient::GetRoomNumber()
{
    GetInputInt(&roomNum, "Which Room You Want To Enjoy:");
    return LinkToServer();
}

bool SingleClient::LinkToServer()
{
    int error = connect(*clientSocket, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR));
    if (error != 0) {
        LogMsg("Client Connect Error, Check IPAddr or Port");
        return false;
    }
    else
        return LinkToRoom();
}
bool SingleClient::LinkToRoom()
{
    // send cmd head
    send(*clientSocket, CMD_ROOM, CMD_LEN, 0);
    // send room number
    string roomNumS = IntToStringID(roomNum);
    send(*clientSocket, roomNumS.c_str(), NUM_LEN, 0);

    // get confirm
    if (GetMsgWithLen(CMD_LEN))
    {
        string result = CutMsgRegion(CMD_LEN);
        if (result == CMD_OPEN)
            return true;
        else
        {
            cout << "Room is not exist or not open new" << endl;
            return false;
        }
    }
    else
    {
        return false;
    }

}

//-----------------------------------------------------//
bool SingleClient::GetUserNameID()
{
    GetInputString(userName, "Input Your User Name:");
    return SetUserID();
}

bool SingleClient::SetUserID()
{
    // send cmd head
    send(*clientSocket, CMD_REGI, CMD_LEN, 0);
    // send name length
    string nameLen = IntToStringID(userName.size());
    send(*clientSocket, nameLen.c_str(), NUM_LEN, 0);
    // send name
    send(*clientSocket, userName.c_str(), userName.size(), 0);

    if (GetMsgWithLen(NUM_LEN))
    {
        userID = stoi(CutMsgRegion(NUM_LEN));
        return true;
    }
    else
        return false;
}

//-----------------------------------------------------//
void SingleClient::BeginChatToServer()
{
    ifKeepChatting = true;

    recvThread = new thread(RecvThread);
    recvThread->detach();

    hearThread = new thread(HearThread);
    hearThread->detach();

    sendThread = new thread(SendThread);
    sendThread->detach();
}

void SingleClient::RecvThread()
{
    SingleClient::GetClientInstance()->RecvMessageFromServer();
    return;
}
void SingleClient::SendThread()
{
    SingleClient::GetClientInstance()->SendMessageToServer();
    return;
}
void SingleClient::HearThread()
{
    SingleClient::GetClientInstance()->CheackHeartBagTime();
    return;
}

void SingleClient::RecvMessageFromServer()
{
    while (ifKeepChatting)
    {
        if (GetMsgWithLen(CMD_LEN))
        {
            string cmdNews = CutMsgRegion(CMD_LEN);
            if (cmdNews == CMD_SEND)
            {
                if (GetMsgWithLen(NUM_LEN))
                {
                    int msgLen = stoi(CutMsgRegion(NUM_LEN));
                    if (GetMsgWithLen(msgLen))
                    {
                        msgList.push_back(CutMsgRegion(msgLen));
                    }
                }
            }
            else if (cmdNews != CMD_BEAT)
            {
                LogMsg("Message From Server is Worry :");
                LogMsg(cmdNews);
            }
        }
        //curMsg = "";
    }
}
void SingleClient::SendMessageToServer()
{
    while (ifKeepChatting)
    {
        if (ifSendMsg)
        {
            SendContentToServer();
        }
        else if (ifHeartBag)
        {
            SendHeartBagToServer();
        }
    }
}
void SingleClient::CheackHeartBagTime()
{
    while (SingleClient::GetClientInstance()->ifKeepChatting)
    {
        SingleClient::GetClientInstance()->ifHeartBag = true;
        Sleep(3000);
    }
}

void SingleClient::SendContentToServer()
{
    send(*clientSocket, CMD_SEND, CMD_LEN, 0);

    string msgLen = IntToStringID(sendMsg->msgLen);
    send(*clientSocket, msgLen.c_str(), msgLen.size(), 0);
    send(*clientSocket, sendMsg->msg, sendMsg->msgLen, 0);

    sendMsg->ClearMsg();
    ifHeartBag = false;
    ifSendMsg = false;
}
void SingleClient::SendHeartBagToServer()
{
    send(*clientSocket, CMD_BEAT, CMD_LEN, 0);
    ifHeartBag = false;
}

//-----------------------------------------------------//
bool SingleClient::GetMsgWithLen(int Len)
{
    while (curMsg.size() < Len)
    {
        char* tempID = new char[100]{ 0 };
        int error = recv(*clientSocket, tempID, BUFFER_MAX_LENG, 0);
        if (error == 0)
            return false;
        curMsg += tempID;
    }
    return true;
}

string SingleClient::CutMsgRegion(int Len)
{
    string result = string(curMsg, 0, Len);
    curMsg = string(curMsg, Len, curMsg.size());
    return result;
}

//--------------------------------------------------------------------------------------------//