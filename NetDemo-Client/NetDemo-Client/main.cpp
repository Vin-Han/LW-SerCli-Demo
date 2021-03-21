#pragma comment (lib, "ws2_32.lib")
#include "SingleClient.h"
#include "ConsoleCtr.h"
#include "MsgCheckPoint.h"
#include "../../Common.h"


int main() {
    //��ʼ��DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //�����׽���
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //���������������
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //���շ��������ص�����
    char szBuffer[MAXBYTE] = { 0 };
    recv(sock, szBuffer, MAXBYTE, NULL);

    //������յ�������
    printf("Message form server: %s\n", szBuffer);

    //�ر��׽���
    closesocket(sock);

    //��ֹʹ�� DLL
    WSACleanup();

    system("pause");
    return 0;
}

int maina() {

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    char* IP = new char[] {"127.0.0.1"};


    ConsoleCtr* console = ConsoleCtr::GetInstance();
    SingleClient* client = SingleClient::GetClientInstance(IP, 1260);
    MsgCheckPoint* msgMAchine = MsgCheckPoint::GetInstence();

    msgMAchine->SetClient(client);
    msgMAchine->SetConsole(console);

    client->UserLogin();
    client->BeginChatting();
    console->BeginInputThread();


    WSACleanup();
    return 0;
}
