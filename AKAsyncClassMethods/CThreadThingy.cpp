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

			CThreadThingy::MapThreadHandles::const_iterator it = t->mapThreadHandles.begin();
			while (it != t->mapThreadHandles.end())
			{
				HANDLE hThread = it->first;
				bool threadCompleted = it->second;

				// Check for signal that thread has finished.
				DWORD result = WaitForSingleObject (hThread, 0);
				if (result == WAIT_FAILED || threadCompleted)
				{
					// WAIT_FAILED occurs if handle is invalid, ie. thread finished.
					it = t->mapThreadHandles.erase (it);
					continue;
				}

				++it;
			}

			//cout << "No. active threads: " << mapThreadHandles.size() << endl;
			done = t->finished;
			activeThreads = t->mapThreadHandles.size() > 0;

			if (!activeThreads)
				SetEvent (t->hEvent_NoThreadsRunning);
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

CThreadThingy::CThreadThingy() :
finished (false), threadMonitorRunning (false)
{
	StartThreadMonitor();
}

CThreadThingy::~CThreadThingy()
{
}

void CThreadThingy::PauseUntilAllThreadsFinished()
{
	hEvent_NoThreadsRunning = CreateEvent (NULL, FALSE, FALSE, NULL);
	DWORD r = WaitForSingleObject (hEvent_NoThreadsRunning, INFINITE);
}
