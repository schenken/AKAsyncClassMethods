// AKAsyncClassMethods.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "AKAsyncClassMethods.h"
#include "CThreadThingy.h"
#include "CFred.h"
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

CWinApp theApp;

using namespace std;

//-----------------------------------------------------------------------------

void DoIt()
{
	std::wcout << "\n\nAKAsyncClassMethods: Demo running class methods asynchronously\n";
	std::wcout << "\n(c) 2018 Andrew Kendall\n\nEnter ? for help\n\n";

	CThreadThingy threadThing;

	// Create a 'work' object.
	CFred fred;

	// Loop to get user input.
	std::wstring inputLine;
	do {
		std::wcout << L">";
		std::getline (std::wcin, inputLine);

		RemoveWhitespace rmws (inputLine);
		inputLine = rmws (static_cast<RemoveWhitespace::Mode>
			(RemoveWhitespace::Leading | RemoveWhitespace::Trailing | RemoveWhitespace::Condense));

		if (inputLine.size() == 0)
			continue;

		if (inputLine == L"?")
		{
			PrintHelp();
			continue;
		}

		if (inputLine == L"q")
			break;
		
		std::vector<std::wstring> vInput = Split (inputLine, L" ");

		if (vInput.size() < 3)
		{
			std::wcout << L"Invalid command - enter ? for help\n";
			continue;
		}

		// Main purpose of demo: Running a class member function 
		// asynchronously, ie. in it's own thread.
		if (vInput[0] == L"r")
		{
			std::wstring id = vInput[1];
			if (id.size() == 0)
			{
				std::wcout << "Invalid id - enter ? for help\n";
				continue;
			}
			int seconds = _wtoi (vInput[2].c_str());
			if (seconds <= 0)
			{
				std::wcout << L"Invalid number of seconds - enter ? for help\n";
				continue;
			}
			CFred::CFredData fredData (id, seconds);
			threadThing.RunInThread (&fred, &CFred::DoYourThing, fredData);
		}

	} while (1);

	// Important not to let objects go out of scope while
	// threads active.
	threadThing.PauseUntilAllThreadsFinished();
}

//-----------------------------------------------------------------------------
int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
            DoIt();
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
