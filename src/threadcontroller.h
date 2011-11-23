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

#ifndef headerfilethreadcontroller
#define headerfilethreadcontroller

/*!
	\file threadcontroller.h
    \brief class ThreadController manages our two threads
*/

#include <ttmath/ttmathobjects.h>
#include <windows.h>
#include "stopcalculating.h"


/*! 
	\brief the object of this class (there's only one) will be 'managing' our two threads

	the first main thread is started with the application (when the system 
	runs the programme) and the second one is when the application creates it 
	at the beginning in WinMain function, the second thread is only used for calculating

	as you know we have some common objects for example the string for parsing,
	user-defined variables and functions etc. and those objects are set in the first
	thread (the first thread is joined with gui), and when we want to make our calculations
	we must put them to the second thread and then we need an object which helps us
	with that job

	there's only one object of this class in our application, we can get a pointer
	to it by using GetPrgRes() function and then by GetThreadController() method

	when we would like to change for example the input string first we must call
	StopCalculating() method then we can change what we want to change and then
	we must call StartCalculating(), for example if we wanted to change the precision
	of displaying we'd have to do:
		GetPrgRes()->GetThreadController()->StopCalculating();
		GetPrgRes()->SetPrecision( ..new_precision.. );
		GetPrgRes()->GetThreadController()->StartCalculating();
*/
class ThreadController
{
public:

	/*!
		the default constructor
		(notice that there'll be only one object of this class)
	*/
	ThreadController();


	/*!
		the destructor
	*/
	~ThreadController();


	/*!
		it initializes an object of this class

		we create two system event and initialize the 'stop_calculating' object
	*/
	bool Init() volatile;


	/*!
		when the second thread leaves the WaitForCalculatingAndBlockForStop() method
		then there's special time for making copy of certain objects (e.g. the input
		string, user-defined variables, functions etc.) and when the second thread 
		will have finished that then it call ReadyForStop() method
	*/
	void ReadyForStop() volatile;


	/*!
		if we want to close the application for example when the user pushed the
		close button we call StopCalculatingAndExitThread() from the first main thread (gui),
		it means that the second thread (calculations) will finish itself
	*/
	void StopCalculatingAndExitThread() volatile;


	/*!
		when we want to change something for caltulating for example the input string
		first we must call StopCalculating()

		StopCalculating() waits for the second thread (if it is in the special time
		of copying variables) then sets the 'stop object' for signaled and returns to
		the caller
	*/
	void StopCalculating() volatile;


	/*!
		when we have changed what we wanted we call StartCalculating()
		in other words it means that the calculations will start 
		(maybe now, maybe at once if the second thread is still working)
	*/
	void StartCalculating() volatile;


	/*!
		this is the main method which is used by the second thread,
		if there's nothing to do this method (and the second thread as well) waits
	*/
	volatile bool WaitForCalculatingAndBlockForStop() volatile;


	/*!
		this method returns the pointer to the 'stop object'
		it's used by the second thread during calculating
	*/
	const volatile ttmath::StopCalculating * GetStopObject()  volatile const;


	/*!	
		it returns 'true' if there was a stop signal during calculating
		the stop signal can be caused by the first thread (gui thread)
	*/
	bool WasStopSignal() volatile const;


private:
	
	// auto-reset, initialized as non-signaled
	HANDLE calculations;

	// manual-reset, initialized as signaled
	HANDLE ready_for_stop;

	bool exit_thread;

	NewStopCalculating stop_calculating;


	/*!
		we make the copy-constructor private so that nobody will be able
		to make a copy of the one object of this class
	*/
	ThreadController(const ThreadController &);

};


#endif
