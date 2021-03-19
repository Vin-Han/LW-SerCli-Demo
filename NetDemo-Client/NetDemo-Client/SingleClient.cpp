#include "SingleClient.h"
#include "../../Common.h"
#include "MsgCheckPoint.h"

#include <iostream>
#include <conio.h>
#include <algorithm>
#include <string>

using namespace std;

SingleClient* SingleClient::singleClient = nullptr;


SingleClient::SingleClient(char* IPAddr, int port)
{
    memset(&serverAddr, 0, sizeof(serverAddr));  //每个字节都用0填充
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(IPAddr);
    serverAddr.sin_port = htons(port);

    recvMsg = new Msg();
    sendMsg = new Msg();

    MsgMachine = MsgCheckPoint::GetInstence();
}

SingleClient* SingleClient::GetClientInstance(char* IPAddr, int port)
{
    if (singleClient == nullptr) {
        singleClient = new SingleClient(IPAddr,port);
    }
    return singleClient;
}

SingleClient::~SingleClient()
{
}

//--------------------------------------------------------------------------------------------//
void SingleClient::BeginChatting()
{
    while (1)
    {
        ConnectToServer();
        SendToServer();
        RecvFromServer();
        CloseSocket();
    }
}

bool SingleClient::ConnectToServer()
{
    clientSocket = new SOCKET(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP));
    int error = connect(*clientSocket, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR));
    if (error == 0)
        return true;
    else {
        LogMsg("Client Connect Error");
        return false;
    }
}

bool SingleClient::SendToServer()
{
    int error;
    if (sendMsg->msgLen > 0) {
        error = send(*clientSocket, sendMsg->msg, sendMsg->msgLen, 0);
        sendMsg->msgLen = 0;
    }
    else{
        error = send(*clientSocket, EMPTY_MESSAGE, EMPTY_MESSAGE_LEN, 0);
    }
    if (error != -1) {
        return true;
    }
    else {
        LogMsg("Client Send Msg Error");
        return false;
    }
}

bool SingleClient::RecvFromServer()
{
    recvMsg->msgLen = recv(*clientSocket, recvMsg->msg, BUFFER_MAX_LENG, 0);
    if (recvMsg->msgLen == 0) {
        LogMsg("Client Receive Msg Error");
        return false;
    }

    int recvTimes = MsgMachine->ClientToClient(recvMsg->msg);
    for (int i = 0; i < recvTimes; i++)
    {
        recvMsg->msgLen = recv(*clientSocket, recvMsg->msg, BUFFER_MAX_LENG, 0);
        if (recvMsg->msgLen == 0) {
            LogMsg("Client Receive Msg Error");
            return false;
        }
        MsgMachine->ClientToConsole(recvMsg->msg);
    }
    return true;
}

void SingleClient::CloseSocket()
{
    memset(sendMsg->msg, 0, BUFFER_MAX_LENG);
    memset(recvMsg->msg, 0, BUFFER_MAX_LENG);
    closesocket(*clientSocket);
}

//--------------------------------------------------------------------------------------------//
void SingleClient::UserLogin()
{
    userID = -1;
    SetUserName();
    userID = GetUserID();
}

void SingleClient::SetUserName()
{
    cout << "Input your login name:" << endl;
    getline(cin,userName);
}

int SingleClient::GetUserID()
{
    ConnectToServer();
    Msg userInfor;
    send(*clientSocket, USER_LOGIN, USER_LOGIN_LEN, 0);

    send(*clientSocket, userName.c_str(), userName.length(), 0);
    userInfor.msgLen = recv(*clientSocket, userInfor.msg, BUFFER_MAX_LENG, 0);
    CloseSocket();
    return MsgMachine->ClientToClient(userInfor.msg);
}
