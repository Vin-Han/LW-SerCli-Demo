#pragma once

#include <iostream>
#include <deque>
#include <thread>
using namespace std;

class MsgCheckPoint;

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

#pragma region input thread
public:
	void BeginInputThread();
	void CloseInputThread();
private:
	thread* inputThread;
	bool ifReceiveInput;

	static void BuildThread();

#pragma endregion

#pragma region UserInput function
public:
	deque<string> MsgList;

private:
	string userInput;
	MsgCheckPoint* msgCheck;

	void PutMessageToConsole();
#pragma endregion

#pragma region console controller function
private:
	void ClearUserInput();
	void SetCursorToLastLine();
#pragma endregion

};
