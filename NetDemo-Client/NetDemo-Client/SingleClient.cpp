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
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(IPAddr);
    serverAddr.sin_port = htons(port);

    userID = -1;
    userIDS = "";

    recvMsg = new Msg();
    sendMsg = new Msg();

    ifSendMessage = false;
    ifQuestAllMsg = false;

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

void SingleClient::SendToServer()
{
    if (userID == -1) {
        send(*clientSocket, USER_LOGIN, USER_LOGIN_LEN, 0);
        GetUserID();
    }
    else if(ifSendMessage) {
        send(*clientSocket, SEND_MSG, SEND_MSG_LEN, 0);
        SendMsgToServer();
    }
    else if(ifQuestAllMsg){
        send(*clientSocket, EMPTY_MESSAGE, EMPTY_MESSAGE_LEN, 0);
        GetAllMessage();
    }
}

void SingleClient::CloseSocket()
{
    closesocket(*clientSocket);
}

void SingleClient::UserLogin()
{
    cout << "Input your login name:" << endl;
    getline(cin, userName);
}

// S_R //
void SingleClient::GetUserID()
{
    // send nameLen
    string nameLen = to_string(userName.size());
    while (nameLen.size() < 4)
        nameLen = "0" + nameLen;
    send(*clientSocket, nameLen.c_str(), nameLen.length(), 0);

    // send name
    send(*clientSocket, userName.c_str(), userName.length(), 0);

    // get ID
    while (userIDS.size() < 4)
    {
        char* tempID = new char[100]{ 0 };
        recv(*clientSocket, tempID, BUFFER_MAX_LENG, 0);
        userIDS += tempID;
    }

    // set ID
    userID = stoi(userIDS);
}

// S_S //
void SingleClient::SendMsgToServer()
{
    string MsgLen = to_string(strlen(sendMsg->msg));
    while (MsgLen.size() < 4)
        MsgLen = "0" + MsgLen;
    // send ID
    send(*clientSocket, userIDS.c_str(), 4, 0);
    // send msg len
    send(*clientSocket, MsgLen.c_str(), 4, 0);
    // send message
    send(*clientSocket, sendMsg->msg, sendMsg->msgLen, 0);

    sendMsg->ClearMsg();
    ifSendMessage = false;
    ifQuestAllMsg = true;
}

// S_R_NR //
void SingleClient::GetAllMessage()
{

    // send ID
    send(*clientSocket, userIDS.c_str(), 4, 0);

    // recv Num
    int curHead = 0;
    int curTail = USER_ID_LEN;
    string message = "";
    while (message.size() < curTail)
    {
        char* newNum = new char[100]{ 0 };
        recv(*clientSocket, newNum, BUFFER_MAX_LENG, 0);
        message += newNum;
    }
    int msgNum = stoi(string(message, curHead, USER_ID_LEN));
    if (msgNum > 0) {
        curHead = curTail;
        curTail = curTail + (msgNum * USER_ID_LEN);

        while (message.size() < curTail)
        {
            char* newLen = new char[100]{ 0 };
            recv(*clientSocket, newLen, BUFFER_MAX_LENG, 0);
            message += newLen;
        }
        vector<msgCon> msgList;

        for (int i = curHead; i < curTail; i += 4)
        {
            msgCon newMsgCon;
            newMsgCon.len = stoi(string(message, i, USER_ID_LEN));
            msgList.push_back(newMsgCon);
        }
        int curIndex = 0;
        int nextMsgLen = msgList[curIndex].len;

        curHead = curTail;
        curTail = curTail + nextMsgLen;
        
        while (nextMsgLen != 0)
        {
            while (message.size() < curTail)
            {
                char* newMsgContext = new char[100]{ 0 };
                recv(*clientSocket, newMsgContext, BUFFER_MAX_LENG, 0);
                message += newMsgContext;
            }
            msgList[curIndex].content = string(message, curHead, curTail - 1);
            curIndex++;
            if (curIndex < msgList.size())
            {
                nextMsgLen = msgList[curIndex].len;
            }
            else
            {
                nextMsgLen = 0;
            }
            curHead = curTail;
            curTail = curTail + nextMsgLen;
        }
        for (msgCon tempCon : msgList)
        {
            MsgMachine->ClientToConsole(tempCon.content);
        }
    }
    else
    {
        LogMsg(" 0 msg");
    }
    ifQuestAllMsg = false;
}

