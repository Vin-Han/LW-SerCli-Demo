#include "ServerMain.h"

ServerMain* ServerMain::instance = nullptr;
SOCKET ServerMain::serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

ServerMain::ServerMain()
{
	curCMD = CMDList::None;
}
ServerMain::~ServerMain(){}
ServerMain* ServerMain::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ServerMain();
	}
	return instance;
}

void ServerMain::BeginListen(int port)
{
	SetServerSocket(port);
	BindServerToPort();
	BeginToListen();
}
void ServerMain::SetServerSocket(int port)
{
	serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_addr.s_addr = ADDR_ANY;
	serverAddr.sin_port = htons(port);
}
void ServerMain::BindServerToPort()
{
	int error = bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR));
	if (error != 0)
		LogMsg("Server Bind Error");
}
void ServerMain::BeginToListen()
{
	int error = listen(serverSocket, LISTEN_MAX_LIST);
	if (error == -1)
		LogMsg("Server Listen Error");
}
