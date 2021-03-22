#include "SingleServer.h"
#include <sstream>

SingleServer::SingleServer(int ServerID)
{
    serverID = ServerID;
    msgList.clear();
    userCurPos.clear();
    userCount = 0;
    userList["Manager"] = userCount;
    userNameList.push_back("Manager");
    userCurPos.push_back(0);
}

SingleServer::~SingleServer()
{
    closesocket(serverSocket);
}

void SingleServer::InitChattingRoom(int port)
{
    SetServerSocket(port);
    BindServerToPort();
    BeginToListen();
}

void SingleServer::OpenChattingRoom()
{
    ifOpenChatting = true;
    while (1)
    {
        if (AcceptClient() == true) {
            RecvFromClient();
            CloseClientSocket();
        }
    }
}

void SingleServer::SetServerSocket(int port)
{
    serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = ADDR_ANY;
    serverAddr.sin_port = htons(port);  //¶Ë¿Ú
}

bool SingleServer::BindServerToPort()
{
    tempMsg.ClearMsg();

    int error = bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR));
    if (error == 0)
        return true;
    else {
        LogMsg("Server Bind Error");
        return false;
    }
}

bool SingleServer::BeginToListen()
{
    int error = listen(serverSocket, LISTEN_MAX_LIST);
    if (error != -1)
        return true;
    else {
        LogMsg("Server Listen Error");
        return false;
    }
}

void SingleServer::CloseChattingRoom()
{
    ifOpenChatting = false;
}

bool SingleServer::AcceptClient()
{    sockaddr_in clientAddr;
    clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &SOCKET_ADDR_LENGTH);
    if (clientSocket != -1)
        return true;
    else {
        LogMsg("Server Accept Error");
        return false;
    }
}

void SingleServer::RecvFromClient()
{
    tempMsg.msgLen = recv(clientSocket, tempMsg.msg, BUFFER_MAX_LENG, 0);
    if (tempMsg.msgLen >= MSG_HEAD_LEN)
        RecvMsgCheck();
    else
        LogMsg("Server Receive Msg Error");
}

void SingleServer::CloseClientSocket()
{
    tempMsg.ClearMsg();
    closesocket(clientSocket); 
}
 
void SingleServer::RecvMsgCheck()
{
    if (StringBeginWith(EMPTY_MESSAGE, tempMsg.msg))
        GetLastMsg();
    else if (StringBeginWith(USER_LOGIN, tempMsg.msg))
        AddNewUser();
    else if(StringBeginWith(SEND_MSG, tempMsg.msg))
        AddNewMessage();
}

// R_S //
void SingleServer::AddNewUser()
{
    int curHead = MSG_HEAD_LEN;
    int curTail = MSG_HEAD_LEN + USER_ID_LEN;
    string message = tempMsg.msg;

    // get name length
    while (message.size() < curTail)
    {
        char* newUser = new char[100]{ 0 };
        recv(clientSocket, newUser, BUFFER_MAX_LENG, 0);
        message += newUser;
    }
    int nameLen = stoi(message.substr(curHead, USER_ID_LEN));
    curHead = curTail;
    curTail += nameLen;
    // get name

    while (message.size() < curTail)
    {
        char* newUser = new char[100]{ 0 };
        recv(clientSocket, newUser, BUFFER_MAX_LENG, 0);
        message += newUser;
    }
    string userName = string(message.substr(curHead, nameLen));

    // return user ID
    if (userList.count(userName) != 0) {
        // if exist, send ID
        map<string, int>::const_iterator userInfor = userList.find(userName);

        string userIDS = to_string(userInfor->second);
        while (userIDS.size() < 4)
            userIDS = "0" + userIDS;

        send(clientSocket, userIDS.c_str(), 4, 0);
    }
    else
    {
        // if new, build new ID
        userCount += 1;
        string userIDS = to_string(userCount);
        while (userIDS.size() < 4)
            userIDS = "0" + userIDS;

        userList[(userName)] = userCount;
        userNameList.push_back(userName);
        userCurPos.push_back(0);

        send(clientSocket, userIDS.c_str(), 4, 0);
    }
}

// R_R //
void SingleServer::AddNewMessage()
{
    int curHead = MSG_HEAD_LEN;
    int curTail = MSG_HEAD_LEN + USER_ID_LEN;
    string message = tempMsg.msg;

    // get ID
    while (message.size() < curTail)
    {
        char* newUser = new char[100]{ 0 };
        recv(clientSocket, newUser, BUFFER_MAX_LENG, 0);
        message += newUser;
    }
    int userID = stoi(message.substr(curHead, USER_ID_LEN));
    curHead = curTail;
    curTail += USER_MSG_LEN;

    // get length
    while (message.size() < curTail)
    {
        char* newUser = new char[100]{ 0 };
        recv(clientSocket, newUser, BUFFER_MAX_LENG, 0);
        message += newUser;
    }
    int msgLen = stoi(message.substr(curHead, USER_ID_LEN));
    curHead = curTail;
    curTail += msgLen;

    // get name
    while (message.size() < curTail)
    {
        char* newUser = new char[100]{ 0 };
        recv(clientSocket, newUser, BUFFER_MAX_LENG, 0);
        message += newUser;
    }
    string userMsg = string(message.substr(curHead, msgLen));

    // store message
    string storeMsg = userNameList[userID] + ":" + userMsg;
    msgList.push_back(storeMsg);
}

// R_S_NS //
void SingleServer::GetLastMsg()
{
    int curHead = MSG_HEAD_LEN;
    int curTail = MSG_HEAD_LEN + USER_ID_LEN;
    string message = tempMsg.msg;

    // get ID
    while (message.size() < curTail)
    {
        char* newUser = new char[100]{ 0 };
        recv(clientSocket, newUser, BUFFER_MAX_LENG, 0);
        message += newUser;
    }
    int userID = stoi(string(message, curHead, (curTail - 1)));

    if (userID > userCurPos.size())
        LogMsg("Server Client is not exist");
    else
    {
        // send number
        int msgNum = msgList.size() - userCurPos[userID];
        string msgNumS = to_string(msgNum);
        while (msgNumS.size() < 4)
            msgNumS = "0" + msgNumS;
        send(clientSocket, msgNumS.c_str(), 4, 0);

        if (msgNum > 0) {
            // send len
            string msgLen = "";
            for (int i = userCurPos[userID]; i < msgList.size(); i++)
            {
                string singleLen = to_string(msgList[i].size());
                while (singleLen.size() < 4)
                    singleLen = "0" + singleLen;
                msgLen += singleLen;
            }
            send(clientSocket, msgLen.c_str(), msgLen.size(), 0);

            // send msg
            for (int i = userCurPos[userID]; i < msgList.size(); i++)
            {
                send(clientSocket, msgList[i].c_str(), msgList[i].size(), 0);
            }
            userCurPos[userID] = msgList.size();
        }
        else
        {
            LogMsg(" 0 msg");
        }
    }
}







