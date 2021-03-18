#include "ConsoleCtr.h"
#include <algorithm>
#include <string>

ConsoleCtr* ConsoleCtr::clientConsole = nullptr;
bool ConsoleCtr::ifKeepIO = false;
bool ConsoleCtr::ifOpenIO = false;

ConsoleCtr::ConsoleCtr()
{
	userInput = "";
	inputOverHandlerList.clear();
	outputList.clear();
	exitHandlerList.push_back(StopIO);
	ifRecvOver = false;

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
	msgMutex = new mutex();
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
				OutputMsg();
				cout << userInput;
			}
		}
	}
	return;
}

void ConsoleCtr::AddOutputMsg(vector<string>& msgList)
{
	msgMutex->lock(); 
	outputList = msgList; 
	msgMutex->unlock();
}

void ConsoleCtr::OutputMsg()
{
	msgMutex->lock();
	for (const string& tempMsg : outputList)
	{
		cout << tempMsg << endl;
	}
	outputList.clear();
	ifRecvOver = false;
	msgMutex->unlock();
}


bool ConsoleCtr::InputCheck(const char& input)
{
	if (input == 8) {
		if (userInput.length() > 0) {
			userInput.pop_back();
		}
		return false;
	}
	else if (input == '\r' && inputOverHandlerList.size() > 0) {
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
	bool result = userInput.compare(0, CMD_HEAD_LEN - 1, CMD_HEAD);
	if (result == true) {
		string command = string(userInput, CMD_HEAD_LEN, userInput.length());
		transform(command.begin(), command.end(),command.begin(),::tolower);
		ResetUserInput();
		if (command == "exit") {
			StopIO();
		}
	}
	return result;
}
