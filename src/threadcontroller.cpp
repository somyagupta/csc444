/*
 * This file is a part of TTCalc - a mathematical calculator
 * and is distributed under the (new) BSD licence.
 * Author: Tomasz Sowa <t.sowa@ttmath.org>
 */

/* 
 * Copyright (c) 2006-2008, Tomasz Sowa
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *    
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *    
 *  * Neither the name Tomasz Sowa nor the names of contributors to this
 *    project may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "threadcontroller.h"
#include <cstdio>



ThreadController::ThreadController()
{
	calculations   = 0;
	ready_for_stop = 0;
	exit_thread    = false;
}


ThreadController::~ThreadController()
{
	if(calculations)   CloseHandle(calculations);
	if(ready_for_stop) CloseHandle(ready_for_stop);
}


bool ThreadController::Init() volatile
{
	char * buffer = new char[300];

	// with 'GetTickCount()' we're generating an unique identifier of our event
	// (there can be another window of ttcalc)
	sprintf((char*)buffer,"TTCalcEventForManagingThreads9928%u",
													(unsigned int)GetTickCount());

	// 'calculations' will be for auto-reseting and initialized as non-signaled
	if( (calculations = CreateEvent(0,false,false,(char*)buffer))==NULL)
	{
		delete [] buffer;
		return false;
	}

	sprintf((char*)buffer,"TTCalcEventReadyForStop5567%u",
													(unsigned int)GetTickCount());

	// 'ready_for_stop' will be for manual-reseting and initialized as signaled
	// 'manual-reset' means that we must call ResetEvent() function (from WinAPI)
	// to manually reset the state to nonsignaled
	if( (ready_for_stop = CreateEvent(0,true,true,(char*)buffer))==NULL)
	{
		delete [] buffer;
		return false;
	}

	delete [] buffer;

return stop_calculating.Init();
}


void ThreadController::ReadyForStop() volatile
{
	SetEvent( ready_for_stop );
}


void ThreadController::StopCalculatingAndExitThread() volatile
{
	WaitForSingleObject(ready_for_stop,INFINITE);

	stop_calculating.Stop();
	exit_thread = true;
	SetEvent(calculations);
}


void ThreadController::StopCalculating() volatile
{
	WaitForSingleObject(ready_for_stop, INFINITE);

	stop_calculating.Stop();
}


void ThreadController::StartCalculating() volatile
{
	SetEvent(calculations);
}


volatile bool ThreadController::WaitForCalculatingAndBlockForStop() volatile
{
	WaitForSingleObject(calculations,INFINITE);
	ResetEvent(ready_for_stop);

	stop_calculating.Start();

return !exit_thread;
}


const volatile ttmath::StopCalculating * ThreadController::GetStopObject()  volatile const
{
	return &stop_calculating;
}


bool ThreadController::WasStopSignal() volatile const
{
	return stop_calculating.WasStopSignal();
}


// private 
ThreadController::ThreadController(const ThreadController &)
{
}



