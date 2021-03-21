#pragma comment (lib, "ws2_32.lib")
#include "SingleServer.h"
#include "../../Common.h"

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    SingleServer* server = new SingleServer(1);

    server->InitChattingRoom(1235);

    server->OpenChattingRoom();

    delete server;

    WSACleanup();

    return 0;
}