#pragma once

// Class CThreadThingy is a utility class for enabling user class methods
// to be executed asynchronously, ie. in a thread.

#include <set>

//-----------------------------------------------------------------------------
// Free functions.

// Housekeeping to remove finished thread handles from setThreadHandles.
UINT ThreadMonitor (LPVOID pParam);

// Template method that resolves to the function executed by a thread (see method RunInThread).
// This is responsible for invoking the specifed user class method.
template <typename T, typename T1>
UINT ThreadProc (LPVOID pParam)
{
	if (pParam != nullptr)
	{
		CThreadData<T, T1>* thrInfo = static_cast<CThreadData<T, T1>*>(pParam);

		CThreadThingy* tt = thrInfo->tt;
		tt->mutex.Acquire();
		tt->setThreadHandles.insert (AfxGetThread()->m_hThread);
		//cout << "setThreadHandles size: " << setThreadHandles.size() << endl;
		tt->mutex.Release();
			
		(thrInfo->obj->*thrInfo->func)(thrInfo->userData);
		delete thrInfo;
	}
	return 0;
};

//-----------------------------------------------------------------------------

// Classes CMutexCS and CLockCS to control thread synchronization - using Critical Sections
class CMutexCS
{
    CRITICAL_SECTION critSection;

public:
	CMutexCS() { InitializeCriticalSection (&critSection); }
	~CMutexCS() { DeleteCriticalSection (&critSection); }
	void Acquire() { EnterCriticalSection (&critSection); }
	void Release() { LeaveCriticalSection (&critSection); }
};

class CLockCS
{
    CMutexCS& mutex;

public:
    CLockCS (CMutexCS& mutex) : mutex (mutex) { mutex.Acquire(); }
	~CLockCS() { mutex.Release(); }
};

//-----------------------------------------------------------------------------

class CThreadThingy
{

public:
	CThreadThingy();
	~CThreadThingy();

	// For controlling access to shared data.
	CMutexCS mutex;

	// Dictionary of active threads. This exists to provide a means by which
	// the main program can determine that all threads have completed before
	// ending. Methods ThreadMonitor and WaitForAllThreadsToFinish are what
	// control this.
	std::set<HANDLE> setThreadHandles;

	// For synchronising functions ThreadMonitor and WaitForAllThreadsToFinish.
	HANDLE noThreadsRunning;

	// Set by WaitForAllThreadsToFinish to signal ThreadMonitor to finish.
	bool finished;

	// Flag to indicate ThreadMonitor function is running.
	bool threadMonitorRunning;

	// Template method for launching a thread that will
	// execute the specifed user class method.
	template <typename T, typename T1>
	void RunInThread (T* obj, void (T::*func)(T1), T1 userData)
	{
		CThreadData<T, T1>* td = new CThreadData<T, T1>(this, obj, func, userData);
		AfxBeginThread (&ThreadProc<T, T1>, td, 0, 0);
	}

	void StartThreadMonitor() { AfxBeginThread (&ThreadMonitor, this, 0, 0); }

	void StopThreadMonitor() { finished = true; }

	// You call this whenever you want to sync your application with threads running,
	// ie. pause your app until all current threads have completed. You will do this
	// at least before exiting your program, but possible at other times too.
	void PauseUntilAllThreadsFinished();

	void Lock() { mutex.Acquire(); }

	void Unlock() { mutex.Release(); }

protected:

	// Template class for holding user class/method/data. Essential data so
	// we can execute the user class method and pass it custom data.
	template <typename T, typename T1>
	class CThreadData
	{
	public:
		CThreadThingy* tt;
		T* obj;
		typedef void (T::*Func) (T1);
		Func func;
		T1 userData;

		CThreadData<T, T1> (CThreadThingy* _tt, T* _obj, Func _func, T1 ud) 
			: tt (_tt), obj (_obj), func (_func), userData (ud) {}
	};

};

