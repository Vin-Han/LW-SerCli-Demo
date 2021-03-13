#include "SingleServer.h"


SingleServer::SingleServer(int ServerID):serverID(ServerID)
{
    serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //���׽���
    memset(&serverAddr, 0, sizeof(serverAddr));  //ÿ���ֽڶ���0���
    serverAddr.sin_family = PF_INET;  //ʹ��IPv4��ַ
    serverAddr.sin_addr.s_addr = ADDR_ANY;  //�����IP��ַ

}

SingleServer::~SingleServer()
{
    closesocket(serverSock);
}

bool SingleServer::BindServerToPort(int port)
{
    serverAddr.sin_port = htons(port);  //�˿�
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

bool SingleServer::SendToClient()
{
    int error = send(clientSocket, tempMsg.msg, tempMsg.msgLen, 0);
    if (error != -1) {
        tempMsg.ClearMsg();
        return true;
    }
    else {
        LogMsg("Server Send Msg Error");
        return false;
    }
}

bool SingleServer::RecvFromClient()
{
    tempMsg.msgLen = recv(clientSocket, tempMsg.msg, BUFFER_MAX_LENG, 0);
    if (tempMsg.msgLen > 0) {
        LogMsg(tempMsg.msg);
        return true;
    }

    else{
        LogMsg("Server Receive Msg Error");
        return false;
    }
}

void SingleServer::CloseClientSocket()
{
    closesocket(clientSocket); 
}

