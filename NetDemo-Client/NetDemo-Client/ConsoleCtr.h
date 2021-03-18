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

	inline void PauseIO(){ ifKeepIO = false; }

	static void StopIO(){ ifOpenIO = false; }

	inline const string GetInput(){ return userInput; }

	inline void ResetUserInput() { userInput = ""; }

#pragma endregion

#pragma region handler function
public:
	bool InputCheck(const char& input);
	bool CommandCheck();

	inline void AddInputOverHandler(void (*handler)()) { inputOverHandlerList.push_back(handler); }
	inline void AddExitHandler(void (*handler)()) { inputOverHandlerList.push_back(handler); }

private:
	string commandHand;
	vector<void(*)()> inputOverHandlerList;
	vector<void(*)()> exitHandlerList;
#pragma endregion


#pragma region thread function
private:
	static void BeginThread();

	void ServerClientIO();

private:
	thread* userIOThread;

	static bool ifKeepIO;

	static bool ifOpenIO;

#pragma endregion

#pragma region properties
private:
	string userInput;
	string emptyContent;

#pragma endregion
#pragma region output function
private:
	bool ifRecvOver;

public:
	vector<string> outputList;
	void AddOutputMsg(vector<string>& msgList) { outputList = msgList; }
	inline void OutputMsgOver() { ifRecvOver = true; }

#pragma endregion





};
