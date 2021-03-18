#include "ConsoleCtr.h"
#include <algorithm>
#include <string>

ConsoleCtr* ConsoleCtr::clientConsole = nullptr;
bool ConsoleCtr::ifKeepIO = false;
bool ConsoleCtr::ifOpenIO = false;

ConsoleCtr::ConsoleCtr()
{
	userInput = "";
	commandHand = "CMD:";
	inputOverHandlerList.clear();
	outputList.clear();
	exitHandlerList.push_back(StopIO);
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
				if(InputCheck(tempInput))
					userInput += tempInput;
			}
			if (ifRecvOver) {
				emptyContent = string(userInput.length(), ' ');
				cout << "\r" << emptyContent << "\r";
				for (const string& tempMsg : outputList)
				{
					cout << tempMsg << endl;
				}
				cout << emptyContent;
				outputList.clear();
				ifRecvOver = false;
			}
		}
	}
	return;
}


bool ConsoleCtr::InputCheck(const char& input)
{
	if (input == '\r' && inputOverHandlerList.size() > 0) {
		if (!CommandCheck()) {
			for (void (*tempHandler)() : inputOverHandlerList)
			{
				tempHandler();
			}
		}
		return false;
	}
	else if (input == 27 && exitHandlerList.size() > 0) {
		for (void (*tempHandler)() : exitHandlerList)
		{
			tempHandler();
		}
		return false;
	}
	return true;
}

bool ConsoleCtr::CommandCheck()
{
	bool result = userInput.compare(0, commandHand.length(), commandHand);
	if (result == true) {
		string command = string(userInput, commandHand.length(), userInput.length());
		transform(command.begin(), command.end(),command.begin(),::tolower);
		ResetUserInput();
		if (command == "exit") {
			StopIO();
		}
	}
	return result;
}
