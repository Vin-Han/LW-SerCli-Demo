#include "ServerMain.h"

int main() {

    ServerMain* serverMain = ServerMain::GetInstance();

    serverMain->Begin();

    return 0;
}