#include "ConsoleCtr.h"

#include "MsgCheckPoint.h"
#include "../../Common.h"
#include <conio.h>

ConsoleCtr* ConsoleCtr::clientConsole = nullptr;

ConsoleCtr::ConsoleCtr()
{
	userInput = "";
	serverInput = "";
}

ConsoleCtr::~ConsoleCtr()
{

}

ConsoleCtr* ConsoleCtr::GetInstance()
{
	if (clientConsole == nullptr) {
		clientConsole = new ConsoleCtr();
	}
	return clientConsole;
}

void ConsoleCtr::Begin()
{
	ifReceiveInput = true;
	PutMessageToConsole();
}

void ConsoleCtr::Close()
{
	ifReceiveInput = false;
	delete this;
}

void ConsoleCtr::PutMessageToConsole()
{
	cin.get();
	while (ifReceiveInput)
	{
		if (_kbhit()) {
			cin.clear(); 
			cin.sync();
			getline(cin, userInput);
			msgStation->ConsoleToClient(userInput);
			ClearUserInput();
		}
		while (msgStation->ClientToConsole(serverInput))
		{
			cout << serverInput << "\n";
		}
	}
	return;
}

void ConsoleCtr::ClearUserInput()
{
	int X, Y;
	GetConsoleXY(X, Y);
	SetConSoleXY(0, Y - 1);
	cout << string(userInput.size()+1, ' ') << "\r";
	userInput = "";
}