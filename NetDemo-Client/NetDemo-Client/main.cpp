#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //���� ws2_32.dll
#pragma warning(disable:4996)

#include "SingleClient.h"
#include <windows.h>

#define BUF_SIZE 100

int main() {
    //��ʼ��DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SingleClient* client = new SingleClient("client");

    while (1)
    {
        client->ConnectToServer();
        client->SendToServer();
        client->RecvFromServer();
        client->CloseSocket();
    }

    WSACleanup();  //��ֹʹ�� DLL
    return 0;
}

int oldmain() {
    //��ʼ��DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //���������������
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);

    char bufSend[BUF_SIZE] = { 0 };
    char bufRecv[BUF_SIZE] = { 0 };

    while (1) {
        //�����׽���
        SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        int temp = connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
        //��ȡ�û�������ַ��������͸�������
        printf("Input a string: ");
        gets_s(bufSend);
        send(sock, bufSend, strlen(bufSend), 0);
        //���շ��������ص�����
        recv(sock, bufRecv, BUF_SIZE, 0);
        //������յ�������
        printf("Message form server: %s\n", bufRecv);
        memset(bufSend, 0, BUF_SIZE);  //���û�����
        memset(bufRecv, 0, BUF_SIZE);  //���û�����
        closesocket(sock);  //�ر��׽���
    }

    WSACleanup();  //��ֹʹ�� DLL
    return 0;
}