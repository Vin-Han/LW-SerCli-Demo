#include "MsgCheckPoint.h"

#include "../../Common.h"

#include "ConsoleCtr.h"
#include "SingleClient.h"




MsgCheckPoint* MsgCheckPoint::singleMsgCheck = nullptr;

MsgCheckPoint* MsgCheckPoint::GetInstence(ConsoleCtr* Console, SingleClient* Client)
{
	if (singleMsgCheck == nullptr) {
		singleMsgCheck = new MsgCheckPoint(Console, Client);
		Console->msgStation = singleMsgCheck;
	}
	return singleMsgCheck;
}

MsgCheckPoint::MsgCheckPoint(ConsoleCtr* Console, SingleClient* Client) :console(Console), client(Client){}

bool MsgCheckPoint::ConsoleToClient(const string& Msg)
{
	if (UserInputCheck(Msg)) {
		strcpy(client->sendMsg->msg, Msg.c_str());
		client->sendMsg->msgLen = Msg.size();
		client->ifSendMsg = true;
	}
	else
	{
		return false;
	}
}

bool MsgCheckPoint::ClientToConsole(string& Msg)
{
	if (client->msgList.size() > 0)
	{
		Msg = client->msgList.front();
		client->msgList.pop_front();
		return true;
	}
	else
	{
		return false;
	}

}

bool MsgCheckPoint::UserInputCheck(const string& Msg)
{
	return true;
}


