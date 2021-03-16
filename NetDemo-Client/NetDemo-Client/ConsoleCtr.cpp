#include "ConsoleCtr.h"

ConsoleCtr* ConsoleCtr::clientConsole = nullptr;

ConsoleCtr::ConsoleCtr()
{
	userInput = "";
	inputOverHandlerList.clear();
	buffer = nullptr;
	ifKeepIO = false;
	ifOpenIO = false;
}

ConsoleCtr* ConsoleCtr::GetInstance()
{
	if (clientConsole == nullptr) {
		clientConsole = new ConsoleCtr();
	}
	return clientConsole;
}

ConsoleCtr::~ConsoleCtr()
{
	if (clientConsole != nullptr) {
		delete clientConsole;
	}
}

void ConsoleCtr::BeginIO()
{
	ifKeepIO = true;
	ifOpenIO = true;
	userIOThread = new thread(ConsoleCtr::BeginThread);
	userIOThread->detach();
}

void ConsoleCtr::PauseIO()
{
	ifKeepIO = false;
}

void ConsoleCtr::StopIO()
{
	ifOpenIO = false;
}

void ConsoleCtr::SetInput(char* newBuffer)
{
	buffer = newBuffer;
}

string ConsoleCtr::GetInput()
{
	return userInput;
}

void ConsoleCtr::BeginThread()
{
	if (clientConsole != NULL) {
		clientConsole->ServerClientIO();
	}
}

void ConsoleCtr::ServerClientIO()
{
	while (ifOpenIO)
	{
		if (ifKeepIO) {
			if (_kbhit()) {
				char tempInput = (char)_getche();
				InputCheck(tempInput);
				userInput += tempInput;
			}
			if (buffer != nullptr) {
				emptyContent = string(userInput.length(), ' ');
				cout << "\r" << emptyContent << "\r";
				cout << buffer << endl;
				cout << emptyContent;
				buffer = NULL;
			}
		}
	}
	return;
}

void ConsoleCtr::AddInputOverHandler(void (*handler)())
{
	inputOverHandlerList.push_back(handler);
}

void ConsoleCtr::InputCheck(const char& input)
{
	if (input == '\r' && inputOverHandlerList.size() > 0) {
		for (void (*tempHandler)() : inputOverHandlerList)
		{
			tempHandler();
		}
	}
}
