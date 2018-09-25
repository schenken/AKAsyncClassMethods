#include "pch.h"
#include "CFred.h"

CFred::CFred() {}

CFred::~CFred() {}

void CFred::DoYourThing (CFredData fredData)
{
	int secondsToRun = fredData.numSeconds;
	std::wstring id = fredData.id;

	for (int i = secondsToRun; i > 0; --i)
	{
		std::wcout << id << L" running - seconds remaining: " << i << std::endl;
		Sleep (1000);
	}

	std::wcout << L"*** Task " << id << L" has finished. ***" << std::endl;
}
