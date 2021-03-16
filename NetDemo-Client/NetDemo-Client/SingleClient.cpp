#include "SingleClient.h"

SingleClient* SingleClient::singleClient = nullptr;
bool SingleClient::ifUserInputOver = false;

SingleClient::SingleClient()
{
    memset(&serverAddr, 0, sizeof(serverAddr));  //每个字节都用0填充
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(1236);

    sendMsg = new Msg();
    recvMsg = new Msg();

    conSoleInstance = ConsoleCtr::GetInstance();
    conSoleInstance->AddInputOverHandler(InputOver_Handler);
}

SingleClient* SingleClient::GetClientInstance()
{
    if (singleClient == nullptr) {
        singleClient = new SingleClient();
    }
    return singleClient;
}

SingleClient::~SingleClient()
{
}

bool SingleClient::ConnectToServer()
{
    clientSocket = new SOCKET(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP));
    int error = connect(*clientSocket, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR));
    if (error == 0)
        return true;
    else {
        LogMsg("Client Connect Error");
        return false;
    }
}

bool SingleClient::SendToServer()
{
    int error;
    if (CheckPlayerInput()) {
        error = send(*clientSocket, sendMsg->msg, sendMsg->msgLen, 0);
    }
    else
    {
        error = send(*clientSocket, "1", 1, 0);
    }
    /*
    cout << "Input a string: ";
    cin >> sendMsg->msg;
    sendMsg->msgLen = strlen(sendMsg->msg);
    int error = send(*clientSocket, sendMsg->msg, sendMsg->msgLen, 0);
    */
    if (error != -1) {
        return true;
    }
    else {
        LogMsg("Client Send Msg Error");
        return false;
    }
}

bool SingleClient::RecvFromServer()
{
    recvMsg->msgLen = recv(*clientSocket, recvMsg->msg, BUFFER_MAX_LENG, 0);
    if (recvMsg->msgLen >= 0) {
        if (recvMsg->msgLen > 0) {
            conSoleInstance->SetInput(recvMsg->msg);
        }
        return true;
    }
    else {
        LogMsg("Client Receive Msg Error");
        return false;
    }
}

void SingleClient::CloseSocket()
{
    memset(sendMsg->msg, 0, BUFFER_MAX_LENG);
    memset(recvMsg->msg, 0, BUFFER_MAX_LENG);
    closesocket(*clientSocket);
}

bool SingleClient::CheckPlayerInput()
{
    if (ifUserInputOver == true) {
        strcpy(sendMsg->msg, conSoleInstance->GetInput().c_str());
        sendMsg->msgLen = conSoleInstance->GetInput().length() -1;
        ifUserInputOver = false;
        return true;
    }
    return false;
}
