#pragma comment (lib, "ws2_32.lib")
#include "SingleClient.h"
#include "ConsoleCtr.h"
#include "MsgCheckPoint.h"
#include <winsock2.h>

int main() {

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    char* IP = new char[] {"127.0.0.1"};


    ConsoleCtr* console = ConsoleCtr::GetInstance();
    SingleClient* client = SingleClient::GetClientInstance();
    MsgCheckPoint* msgMAchine = MsgCheckPoint::GetInstence(console,client);

    client->Begin();
    console->Begin();

    client->Close();
    console->Close();

    WSACleanup();
    return 0;
}
