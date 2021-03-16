#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll
#pragma warning(disable:4996)

#include "SingleServer.h"

#define BUF_SIZE 100

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SingleServer* server = new SingleServer(1);

    server->BindServerToPort(1236);

    server->BeginToListen();

    while (1)
    {
        if (server->AcceptClient() == true) {
            server->RecvFromClient();
            server->SendToClient();
            server->CloseClientSocket();
        }
    }
    delete server;

    //终止 DLL 的使用
    WSACleanup();

    return 0;
}

int oldmain() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //绑定套接字
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;  //使用IPv4地址
    sockAddr.sin_addr.s_addr = ADDR_ANY;  //具体的IP地址
    sockAddr.sin_port = htons(1234);  //端口
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //进入监听状态
    listen(servSock, 20);

    //接收客户端请求
    sockaddr_in clntAddr;
    int nSize = sizeof(SOCKADDR);
    char buffer[BUF_SIZE] = { 0 };  //缓冲区
    while (1) {
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
        char* temp = inet_ntoa(clntAddr.sin_addr);
        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);  //接收客户端发来的数据
        printf("Message form client: %s\n", buffer);
        send(clntSock, buffer, strLen, 0);  //将数据原样返回
        closesocket(clntSock);  //关闭套接字
        memset(buffer, 0, BUF_SIZE);  //重置缓冲区
    }

    //关闭套接字
    closesocket(servSock);

    //终止 DLL 的使用
    WSACleanup();

    return 0;
}