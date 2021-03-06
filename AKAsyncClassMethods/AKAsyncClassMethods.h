#pragma once

#include "resource.h"

//-------------------------------------------------------------------------
const WCHAR *helpText =
	L"\nUsage:\n"
	L"\n"
    L"    r <id> <numSeconds>\n"
	L"\n"
    L"        Runs a \"task\" in a thread for <numSeconds> where <id>\n"
    L"        is an arbitrary identifier, eg.\n"
	L"\n"
    L"            r andy 10\n"
	L"\n"
    L"        Runs task \"andy\" for 10 seconds.\n"
	L"\n"
    L"    q\n"
	L"\n"
    L"        Exit the program (after all current task threads\n"
    L"        have completed).\n"
	L"\n"
    L"    ?\n"
	L"\n"
    L"        Display this page.\n"
	L"\n\n";

void PrintHelp() { std::wcout << helpText; }

void DoIt();