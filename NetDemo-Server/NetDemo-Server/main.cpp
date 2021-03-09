#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")  //���� ws2_32.dll
#pragma warning(disable:4996)

#define BUF_SIZE 100

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //�����׽���
    SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);

    //���׽���
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
    sockAddr.sin_family = PF_INET;  //ʹ��IPv4��ַ
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //�����IP��ַ
    sockAddr.sin_port = htons(1234);  //�˿�
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //�������״̬
    listen(servSock, 20);

    //���տͻ�������
    sockaddr_in clntAddr;
    int nSize = sizeof(SOCKADDR);
    char buffer[BUF_SIZE] = { 0 };  //������
    while (1) {
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
        char* temp = inet_ntoa(clntAddr.sin_addr);
        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);  //���տͻ��˷���������
        printf("Message form client: %s\n", buffer);
        send(clntSock, buffer, strLen, 0);  //������ԭ������
        closesocket(clntSock);  //�ر��׽���
        memset(buffer, 0, BUF_SIZE);  //���û�����
    }

    //�ر��׽���
    closesocket(servSock);

    //��ֹ DLL ��ʹ��
    WSACleanup();

    return 0;
}