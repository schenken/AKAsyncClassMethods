AKAsyncClassMethods is a simple command line utility (for Windows)
to demonstrate a way of running a class method asynchronously,
ie. in a thread. It was developed in Visual Studio 2017.

When you run AKAsyncClassMethods, enter ? at the prompt to see a
simple help page.

It's all done with class CThreadThingy, whose main function is RunInThread.
RunInThread takes three arguments:

	1. Pointer to your user class.
	
	2. Function pointer to a user class method.
	
	3. Arbitrary custom object that contains data which your user
	class method needs.

Therefore any user class method that you wish to run under the control
of CThreadThingy must be a void function that takes a single object.
For example, if you have user class CFred with method FooBar:

	void CFred::FooBar (int myNum)

The object passed in can be ANY custom type. Typically it might be a
class or struct in which in you put data that your class user method
requires. In the example code, see struct CFredData defined as publicly
accessible inside the CFred class.

The example code illustrates usage like this:

	CFred::CFredData fredData (id, seconds);
	threadThing.RunInThread (&fred, &CFred::DoYourThing, fredData);

User class method CFred::DoYourThing expects a CFred::CFredData object
to be passed in. CFred::CFredData provide a convenient constructor that
allows you to supply the required data when you instantiate it. In this
example, the user data is an arbitrary identifier and a number of seconds
that the thread should run.

Then you call the CThreadThingy::RunInThread method, which launches a
thread in which CFred::DoYourThing runs.

When you instantiate an instance of CThreadThingy, it starts up its own
control thread in which function ThreadMonitor runs. ThreadMonitor's
job is to check for user threads running. This is so that your
application can ensure it doesn't end until all such threads have 
completed - function CThreadThingy::PauseUntilAllThreadsFinished is
provided for this purpose.

(c) 2018 Andrew Kendall
