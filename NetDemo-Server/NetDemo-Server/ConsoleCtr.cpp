#include "ConsoleCtr.h"
#include <string>
#include "../../Common.h"


ConsoleCtr* ConsoleCtr::singleConsole = nullptr;

ConsoleCtr::ConsoleCtr()
{
	ifKeepInput = false;
	ownerInput = "";
}

ConsoleCtr::~ConsoleCtr()
{
}

void ConsoleCtr::OpenInputConsole()
{
	ifKeepInput = true;
	BeginInput();
}

void ConsoleCtr::BeginInput()
{
	while (ifKeepInput)
	{
		getline(cin, ownerInput);
		InputCheck();
	}
}

void ConsoleCtr::InputCheck()
{
	if (StringBeginWith(ownerInput,"OpenRoom"))
	{
		int roomNum = stoi(string(ownerInput, 8, 2));

	}
	else if (true)
	{

	}
	ownerInput = "";
}

