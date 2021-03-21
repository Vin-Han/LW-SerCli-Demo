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
	if (UserInputCheck(Msg)) {
		strcpy(client->sendMsg->msg, Msg.c_str());
		client->sendMsg->msgLen = Msg.size();
		client->ifSendMessage = true;
	}
}

void MsgCheckPoint::ClientToConsole(const string& Msg)
{
	ServerMsgCheck(Msg);
	console->MsgList.push_back(Msg);

}

int MsgCheckPoint::ClientToClient(char *Msg)
{
	return atoi(Msg);
}

bool MsgCheckPoint::UserInputCheck(const string& Msg)
{
	if (StringBeginWith(Msg,CMD_HEAD)) {
		string cmd = string(Msg,CMD_HEAD_LEN, Msg.size());
		return false;
	}
	return true;
}

void MsgCheckPoint::ServerMsgCheck(const string& Msg)
{
}

void MsgCheckPoint::CMDMsgCheck(const string& Msg)
{
}

