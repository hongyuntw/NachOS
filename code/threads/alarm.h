// alarm.h 
//	Data structures for a software alarm clock.
//
//	We make use of a hardware timer device, that generates
//	an interrupt every X time ticks (on real systems, X is
//	usually between 0.25 - 10 milliseconds).
//
//	From this, we provide the ability for a thread to be
//	woken up after a delay; we also provide time-slicing.
//
//	NOTE: this abstraction is not completely implemented.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ALARM_H
#define ALARM_H

#include "copyright.h"
#include "utility.h"
#include "callback.h"
#include "timer.h"
#include "thread.h"
#include <vector>
class SleepingThread{
public:
	SleepingThread(Thread*,int);
	Thread* sleepingThread;
	int WhenToWakeUp;

};
// my sleep controller
class Sleep{
public:
	Sleep();
	void PutThreadToSleep(Thread *,int);
	bool PutThreadToReady();
	bool IsEmpty();
private:
	int NowInterruptCount;
	std::vector<SleepingThread> threadvec;
	

};

// The following class defines a software alarm clock. 
class Alarm : public CallBackObj {
  public:
    Alarm(bool doRandomYield);	// Initialize the timer, and callback 
				// to "toCall" every time slice.
    ~Alarm() { delete timer; }
    
    void WaitUntil(int x);	// suspend execution until time > now + x
    Thread*  sleep_thread; 	

	// my own function
	//void Sleep(Thread* current_thread,int x);
	

  private:
    Timer *timer;		// the hardware timer device

    void CallBack();		// called when the hardware
				// timer generates an interrupt

	Sleep mySleep;
};

#endif // ALARM_H
