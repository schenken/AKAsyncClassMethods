#pragma once

#include <string>
#include <iostream>

// CFred class represents functionality that does work.

class CFred
{

public:
	CFred();
	~CFred();

	// Structure for passing data to CFred methods.
	// Required to facilitate use of CThreadThingy::RunInThread, whose 3rd parameter
	// is a single object containing any data required by the user class method.
	struct CFredData
	{
		std::wstring id;
		int numSeconds;

		CFredData (std::wstring _id, int _numSeconds) : id (_id), numSeconds (_numSeconds) {}
	};

	// Simulates activity.
	void DoYourThing (CFredData fredData);
};
