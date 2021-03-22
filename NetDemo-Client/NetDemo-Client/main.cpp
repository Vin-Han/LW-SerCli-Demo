#pragma comment (lib, "ws2_32.lib")
#include "SingleClient.h"
#include "ConsoleCtr.h"
#include "MsgCheckPoint.h"
#include "../../Common.h"

int main() {

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    char* IP = new char[] {"127.0.0.1"};


    ConsoleCtr* console = ConsoleCtr::GetInstance();
    SingleClient* client = SingleClient::GetClientInstance(IP, 1234);
    MsgCheckPoint* msgMAchine = MsgCheckPoint::GetInstence();

    msgMAchine->SetClient(client);
    msgMAchine->SetConsole(console);

    client->UserLogin();
    console->BeginInputThread();
    client->BeginChatting();



    WSACleanup();
    return 0;
}
