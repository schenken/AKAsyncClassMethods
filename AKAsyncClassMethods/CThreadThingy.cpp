#include "pch.h"
#include "CThreadThingy.h"

// Free functions
UINT ThreadMonitor (LPVOID pParam)
{
	CThreadThingy* t = static_cast<CThreadThingy*>(pParam);

	{ CLockCS lock (t->mutex);
		t->threadMonitorRunning = true;
	}

	bool done = false;
	bool activeThreads = true;
	while (!done || activeThreads)
	{
		{ CLockCS lock (t->mutex);
			std::set<HANDLE>::const_iterator it = t->setThreadHandles.begin();
			while (it != t->setThreadHandles.end())
			{
				HANDLE hThread = *it;

				// Check for signal that thread has finished.
				DWORD result = WaitForSingleObject (hThread, 0);
				if (result == WAIT_FAILED)
				{
					// WAIT_FAILED occurs if handle is invalid, ie. thread finished.
					it = t->setThreadHandles.erase (it);
					continue;
				}
				++it;
			}
			//cout << "No. active threads: " << setThreadHandles.size() << endl;
			done = t->finished;
			activeThreads = t->setThreadHandles.size() > 0;

			if (!activeThreads)
				SetEvent (t->noThreadsRunning);
		}
		Sleep (1000);
	}

	{ CLockCS lock (t->mutex);
		t->threadMonitorRunning = false;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// CThreadThingy implementation

CThreadThingy::CThreadThingy()
{
}

CThreadThingy::~CThreadThingy()
{
}

void CThreadThingy::PauseUntilAllThreadsFinished()
{
	{ CLockCS lock (mutex);
		if (!threadMonitorRunning)
			StartThreadMonitor();
	}

	noThreadsRunning = CreateEvent (NULL, FALSE, FALSE, NULL);
	DWORD r = WaitForSingleObject (noThreadsRunning, INFINITE);
}
