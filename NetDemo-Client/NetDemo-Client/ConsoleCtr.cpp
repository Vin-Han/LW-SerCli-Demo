#include "ConsoleCtr.h"

ConsoleCtr::ConsoleCtr()
{
	ifThreadOpen = false;
	userInput = "";
	inputHandlerList.clear();
	buffer = nullptr;

}

ConsoleCtr::~ConsoleCtr()
{

}

void ConsoleCtr::BeginThread()
{
	ifThreadOpen = true;
	inputThread = new thread(ConsoleCtr::ThreadFunction);
	inputThread->detach();

	BufferPutput();
}

void ConsoleCtr::ThreadFunction()
{
}

void ConsoleCtr::PlayerInput()
{
	while (1)
	{
		if (_kbhit()) {
			char tempInput = (char) _getche();
			InputCheck(tempInput);
			userInput += tempInput;
		}
	}
}

void ConsoleCtr::BufferPutput()
{
	while (1)
	{
		if (buffer != nullptr) {
			emptyContent = string(userInput.length(),' ');
			cout << "\r" << emptyContent << "\r";
			cout << buffer << endl;
			cout << emptyContent;
		}
	}
}

void ConsoleCtr::addInputOverHandler(void (*handler)(string&))
{
	inputHandlerList.push_back(handler);
}

void ConsoleCtr::SetBuffer(char* newBuffer)
{
	buffer = newBuffer;
}

void ConsoleCtr::InputCheck(char input)
{
	if (input == '\n') {
		for (void (*tempHandler)(string&) : inputHandlerList)
		{
			tempHandler(userInput);
		}
	}
}
