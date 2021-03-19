#include "SingleServer.h"


SingleServer::SingleServer(int ServerID):serverID(ServerID)
{
    serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&serverAddr, 0, sizeof(serverAddr));  
    serverAddr.sin_family = PF_INET; 
    serverAddr.sin_addr.s_addr = ADDR_ANY;

    userCount = 1000;
    msgList.clear();
}

SingleServer::~SingleServer()
{
    closesocket(serverSock);
}

bool SingleServer::BindServerToPort(int port)
{
    serverAddr.sin_port = htons(port);  //¶Ë¿Ú
    int error = bind(serverSock, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR));
    if (error == 0)
        return true;
    else {
        LogMsg("Server Bind Error");
        return false;
    }
}

bool SingleServer::BeginToListen()
{
    int error = listen(serverSock, LISTEN_MAX_LIST);
    if (error != -1)
        return true;
    else {
        LogMsg("Server Listen Error");
        return false;
    }
}

bool SingleServer::AcceptClient()
{
    sockaddr_in clientAddr;
    clientSocket = accept(serverSock, (SOCKADDR*)&clientAddr, &SOCKET_ADDR_LENGTH);
    if (clientSocket != -1)
        return true;
    else {
        LogMsg("Server Accept Error");
        return false;
    }
}

bool SingleServer::RecvFromClient()
{
    tempMsg.msgLen = recv(clientSocket, tempMsg.msg, BUFFER_MAX_LENG, 0);
    if (tempMsg.msgLen >= 0) {
        if (RecvMsgCheck()) {
            msgList.push_back(tempMsg.msg);
        }
        return true;
    }
    else {
        LogMsg("Server Receive Msg Error");
        return false;
    }
}

bool SingleServer::SendToClient()
{
    send(clientSocket, "0",1 , 0);

    return true;

}



void SingleServer::CloseClientSocket()
{
    tempMsg.ClearMsg();
    closesocket(clientSocket); 
}
 
bool SingleServer::RecvMsgCheck()
{
    if (tempMsg.msgLen == EMPTY_MESSAGE_LEN && 
        StringBeginWith(EMPTY_MESSAGE, tempMsg.msg)) {
        return false;
    }
    else if (tempMsg.msgLen == USER_LOGIN_LEN &&
        StringBeginWith(USER_LOGIN, tempMsg.msg))
    {
        AddNewUser();
    }
    return true;
}

void SingleServer::AddNewUser()
{
    Msg newUser;
    newUser.msgLen = recv(clientSocket, newUser.msg, BUFFER_MAX_LENG, 0);
    if (userList.count(newUser.msg) != 0) {
        map<string, int>::const_iterator userInfor = userList.find(newUser.msg);
        char userID[5];
        _itoa_s(userInfor->second, userID, 10);
        send(clientSocket, userID, 4, 0);
    }
    else
    {
        userCount += 1;
        char userID[5];
        _itoa_s(userCount, userID,10);
        userList[(newUser.msg)] = userCount;
        send(clientSocket, userID, 4, 0);
    }
}

