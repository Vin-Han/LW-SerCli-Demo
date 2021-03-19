#pragma once

#include <iostream>

class ConsoleCtr;
class SingleClient;

using namespace std;

class MsgCheckPoint
{
public:
	~MsgCheckPoint(){}
	static MsgCheckPoint* GetInstence();
	void ConsoleToClient(const string& Msg);
	void ClientToConsole(const string& Msg);
	int ClientToClient(const string& Msg);

	inline void SetConsole(ConsoleCtr* Console) { console = Console; }
	inline void SetClient(SingleClient* Client) { client = Client; }

private:
	static MsgCheckPoint* singleMsgCheck;

	ConsoleCtr* console;
	SingleClient* client;

	MsgCheckPoint(){}
};
