#pragma once

#include <string>
using namespace std;

#define LINK_CMD_LEN 9
#define CLOSE_LINK "CloseLink"

class ConsoleCtr;
class SingleClient;

class MsgCheckPoint
{
#pragma region single mode function
public:
	static MsgCheckPoint* GetInstence(ConsoleCtr* Console, SingleClient* Client);
	~MsgCheckPoint() {}
private:
	static MsgCheckPoint* singleMsgCheck;

	ConsoleCtr* console;
	SingleClient* client;

	MsgCheckPoint(ConsoleCtr* Console, SingleClient* Client);
#pragma endregion

#pragma region data change function
public:
	bool ConsoleToClient(const string& Msg);
	bool ClientToConsole(string& Msg);
#pragma endregion

#pragma region msg check function
private:		
	bool UserInputCheck(const string& Msg);

#pragma endregion

};
