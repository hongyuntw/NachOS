// alarm.cc
//	Routines to use a hardware timer device to provide a
//	software alarm clock.  For now, we just provide time-slicing.
//
//	Not completely implemented.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "alarm.h"
#include "main.h"

//----------------------------------------------------------------------
// Alarm::Alarm
//      Initialize a software alarm clock.  Start up a timer device
//
//      "doRandom" -- if true, arrange for the hardware interrupts to 
//		occur at random, instead of fixed, intervals.
//----------------------------------------------------------------------

Alarm::Alarm(bool doRandom)
{
    timer = new Timer(doRandom, this);

}

//----------------------------------------------------------------------
// Alarm::CallBack
//	Software interrupt handler for the timer device. The timer device is
//	set up to interrupt the CPU periodically (once every TimerTicks).
//	This routine is called each time there is a timer interrupt,
//	with interrupts disabled.
//
//	Note that instead of calling Yield() directly (which would
//	suspend the interrupt handler, not the interrupted thread
//	which is what we wanted to context switch), we set a flag
//	so that once the interrupt handler is done, it will appear as 
//	if the interrupted thread called Yield at the point it is 
//	was interrupted.
//
//	For now, just provide time-slicing.  Only need to time slice 
//      if we're currently running something (in other words, not idle).
//	Also, to keep from looping forever, we check if there's
//	nothing on the ready list, and there are no other pending
//	interrupts.  In this case, we can safely halt.
//----------------------------------------------------------------------

void 
Alarm::CallBack() 
{
    Interrupt *interrupt = kernel->interrupt;
    MachineStatus status = interrupt->getStatus();
	bool wakeUp = mySleep.PutThreadToReady();
    if (status == IdleMode&& !wakeUp && mySleep.IsEmpty()) {	
        // is it time to quit?
        if (!interrupt->AnyFutureInterrupts()) {
	        timer->Disable();	// turn off the timer
	        cout<<"timer closing"<<endl;
	    }
    } 
    else{
        if(kernel->scheduler->getSchedulerType() == RR || kernel->scheduler->getSchedulerType() == SRTF ) {
            interrupt->YieldOnReturn();
        }
    }
    //else {			// there's someone to preempt
	//	interrupt->YieldOnReturn();
	//}
}
void 
Alarm::WaitUntil(int x){
	// disable interrupt
	kernel->interrupt->SetLevel(IntOff);
	Thread *currentThread = kernel->currentThread;	
	//cout << "testasdsd"<<endl;
	// call function
	mySleep.PutThreadToSleep(currentThread,x);
	// enable interrupt
	kernel->interrupt->SetLevel(IntOn);

}

Sleep::Sleep(){
	NowInterruptCount = 0;
}

SleepingThread::SleepingThread(Thread* t, int x){
    sleepingThread = t;
    WhenToWakeUp = x;
}


bool
Sleep::IsEmpty(){
    return threadvec.empty();
}


bool
Sleep::PutThreadToReady(){


    // set bool var to check is wake up or not
    bool wakeUp=false;


    //add my interrupt count
    NowInterruptCount++;

    for(int i = 0 ;i<threadvec.size();){

        if(NowInterruptCount>= threadvec[i].WhenToWakeUp){
        // time to wakeup
        wakeUp = true;
        // cout<<"wake uppppppppppp"<<endl;
        kernel->scheduler->ReadyToRun(threadvec[i].sleepingThread);
        //clear this sleeping thread
        threadvec.erase(threadvec.begin()+i); 
        }
        else{
            i++;
        }
    }
    return wakeUp;
}




void
Sleep::PutThreadToSleep(Thread* T , int x){


    //check can send interrupt
    ASSERT(kernel->interrupt->getLevel()==IntOff);


    //push a thread to vector,and store how long will it wakeup(NowInterruptCount+x)
    threadvec.push_back(SleepingThread(T,NowInterruptCount+x));

    //sleep is not finish,so dont need to find next thread
    T->Sleep(false);
}

