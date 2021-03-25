#pragma once

#include <thread>
#include <iostream>
using namespace std;

class ConsoleCtr
{
#pragma region single mode function
public:
	~ConsoleCtr();
	ConsoleCtr* GetInstance() {
		if (singleConsole == nullptr)
		{
			singleConsole = new ConsoleCtr();
		}
		return singleConsole;
	}
private:
	static ConsoleCtr* singleConsole;
	ConsoleCtr();
#pragma endregion

#pragma region input check
public:

private:
	string ownerInput;
#pragma endregion




#pragma region Open Input Function
public:
	void OpenInputConsole();
private:
	bool ifKeepInput;

	void BeginInput();

	void InputCheck();
#pragma endregion

};