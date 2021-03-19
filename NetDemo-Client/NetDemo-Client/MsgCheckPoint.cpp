#include "MsgCheckPoint.h"

#include "ConsoleCtr.h"
#include "SingleClient.h"
#include "../../Common.h"

MsgCheckPoint* MsgCheckPoint::singleMsgCheck = nullptr;


MsgCheckPoint* MsgCheckPoint::GetInstence()
{
	if (singleMsgCheck == nullptr) {
		singleMsgCheck = new MsgCheckPoint();
	}
	return singleMsgCheck;
}

void MsgCheckPoint::ConsoleToClient(const string& Msg)
{
	console->MsgList.push_back(Msg);
}

void MsgCheckPoint::ClientToConsole(const string& Msg)
{
	strcpy(client->sendMsg->msg, Msg.c_str());
	client->sendMsg->msgLen = Msg.size();
}

int MsgCheckPoint::ClientToClient(const string& Msg)
{
	return 0;
}

