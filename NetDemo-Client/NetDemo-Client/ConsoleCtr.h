#pragma once

#include <string>
#include <deque>
using namespace std;

class MsgCheckPoint;

class ConsoleCtr
{
#pragma region singlemode function
private:
	ConsoleCtr();
	static ConsoleCtr* clientConsole;
public:
	MsgCheckPoint* msgStation;
	static ConsoleCtr* GetInstance();
	~ConsoleCtr();
#pragma endregion

#pragma region input thread
public:
	void Begin();
	void Close();
#pragma endregion

#pragma region UserInput function
private:
	bool ifReceiveInput;
	string userInput;
	string serverInput;

	void PutMessageToConsole();
	void ClearUserInput();
#pragma endregion
};
