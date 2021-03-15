#pragma once

#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <thread>
#include <vector>

using namespace std;

class ConsoleCtr
{
public:
	ConsoleCtr();
	~ConsoleCtr();

private:
	static void ThreadFunction();
	
	void PlayerInput();

	void BeginThread();

	void InputCheck(char input);
	
	void BufferPutput();

	void addInputOverHandler( void (*handler)(string&));

	void SetBuffer(char* newBuffer);

private:
	bool ifThreadOpen;
	string userInput;
	string emptyContent;
	char* buffer;
	thread* inputThread;
	vector<void(*)(string&)> inputHandlerList;
	static void (*threadFun)();
};
