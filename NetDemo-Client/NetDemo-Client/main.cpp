#pragma comment (lib, "ws2_32.lib")
#include "SingleClient.h"
#include "ConsoleCtr.h"
#include "MsgCheckPoint.h"
#include "../../Common.h"


int main() {
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //接收服务器传回的数据
    char szBuffer[MAXBYTE] = { 0 };
    recv(sock, szBuffer, MAXBYTE, NULL);

    //输出接收到的数据
    printf("Message form server: %s\n", szBuffer);

    //关闭套接字
    closesocket(sock);

    //终止使用 DLL
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
