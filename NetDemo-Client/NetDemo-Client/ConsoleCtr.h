#pragma once

#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <vector>
#include <thread>

using namespace std;

class ConsoleCtr
{
#pragma region singlemode function
private:
	ConsoleCtr();
	static ConsoleCtr* clientConsole;
public:
	static ConsoleCtr* GetInstance();
	~ConsoleCtr();
#pragma endregion

#pragma region common function
public:
	void BeginIO();

	void PauseIO();

	void StopIO();

	void SetInput(char* newBuffer);

	string GetInput();

	void AddInputOverHandler(void (*handler)());

#pragma endregion


#pragma region thread function
private:
	static void BeginThread();

	void ServerClientIO();

	void InputCheck(const char& input);

private:
	thread* userIOThread;

	bool ifKeepIO;

	bool ifOpenIO;

#pragma endregion

#pragma region properties

private:

	string userInput;
	string emptyContent;
	char* buffer;
	vector<void(*)()> inputOverHandlerList;
#pragma endregion





};
