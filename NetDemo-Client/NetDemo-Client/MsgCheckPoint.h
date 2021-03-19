#pragma once

#include <iostream>

class ConsoleCtr;
class SingleClient;

using namespace std;

class MsgCheckPoint
{
#pragma region single mode function
public:
	static MsgCheckPoint* GetInstence();
	~MsgCheckPoint() {}
private:
	static MsgCheckPoint* singleMsgCheck;
	MsgCheckPoint() {}
#pragma endregion

#pragma region init function
public:
	inline void SetConsole(ConsoleCtr* Console) { console = Console; }
	inline void SetClient(SingleClient* Client) { client = Client; }

private:

	ConsoleCtr* console;
	SingleClient* client;
#pragma endregion

#pragma region data change function
public:
	void ConsoleToClient(const string& Msg);
	void ClientToConsole(const string& Msg);
	int ClientToClient(char* Msg);

#pragma endregion

#pragma region msg check function
private:
		
	bool UserInputCheck(const string& Msg);

	void ServerMsgCheck(const string& Msg);

	void CMDMsgCheck(const string& Msg);
#pragma endregion

};
