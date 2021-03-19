#include "ConsoleCtr.h"

#include "MsgCheckPoint.h"
#include "../../Common.h"

#include <conio.h>
#include <algorithm>
#include <string>

ConsoleCtr* ConsoleCtr::clientConsole = nullptr;

ConsoleCtr::ConsoleCtr()
{
	userInput = "";
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

//--------------------------------------------------------------------------------------------//
void ConsoleCtr::BeginInputThread()
{
	ifReceiveInput = true;
	msgCheck = MsgCheckPoint::GetInstence();
	inputThread = new thread(BuildThread);
	inputThread->detach();
}

void ConsoleCtr::CloseInputThread()
{
	ifReceiveInput = false;
}

void ConsoleCtr::BuildThread()
{
	ConsoleCtr::GetInstance()->PutMessageToConsole();
	return;
}

//--------------------------------------------------------------------------------------------//
void ConsoleCtr::PutMessageToConsole()
{
	while (ifReceiveInput)
	{
		if (_kbhit()) {
			getline(cin, userInput);
			msgCheck->ConsoleToClient(userInput);
			ClearUserInput();
		}
		while (MsgList.size() > 0)
		{
			cout << MsgList.front() << "\n";
			MsgList.pop_front();
		}
	}
	return;
}

//--------------------------------------------------------------------------------------------//
void ConsoleCtr::ClearUserInput()
{
	SetCursorToLastLine();
	cout << string(userInput.size(), ' ') << "\r";
}

void ConsoleCtr::SetCursorToLastLine()
{
	int X, Y;
	GetConsoleXY(X, Y);
	SetConSoleXY(0, Y - 1);
}

