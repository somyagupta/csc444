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

#ifndef headerfilestopcalculating
#define headerfilestopcalculating

/*!
	\file stopcalculating.h
    \brief it tells us whether there was a stop signal or not
*/

#include "compileconfig.h"
#include <ttmath/ttmathtypes.h>
#include <windows.h>
#include <cstdio>


/*!
	\brief it tells us whether there was a stop signal or not
	
	we can pass a pointer to the object of this class to the certain 
	calculating methods especially for the method which calculates the 
	Factorial (1*2*3*4....), such methods as the Factorial takes a lot of time
	and sometimes we want to break the calculations for example when we've got 
	a new input string

	those methods which take a lot of time check the object of type NewStopCalculating
	and if there was a stop signal they stop and return immediately without returning
	a correct result

	the stop signal can be caused by another thread in our case by the first (gui) thread

	there'll be only one object of this class
	(it'll be as an variable of ThreadController class)
*/
class NewStopCalculating : public ttmath::StopCalculating
{
public:


	/*!
		the constructor
	*/
	NewStopCalculating()
	{
		stop = 0;
	}


	/*!
		the destructor
	*/
	~NewStopCalculating()
	{
		if(stop) CloseHandle(stop);
	}


	/*!
		it initializes an object of this class

		this method will be called from Init() of the ThreadController object
	*/
	bool Init() volatile
	{
		char buffer[80];

		// with 'GetTickCount()' we're generating an unique identifier of our event
		// (there can be another window of ttcalc)
		sprintf(buffer,"TTCalcStopEventForStoppingCalculations7756%u",
															(unsigned int)GetTickCount());
		
		// 'stop' will be manual-reseted and initialized as non-signaled
		if( (stop = CreateEvent(0,true,false,buffer))==NULL)
			return false;

	return true;
	}


	/*!
		when we want to stop the calculations for example when the user changed somewhat
		we call the Stop() method from the first (gui) thread
	*/
	void Stop() volatile
	{
		SetEvent(stop);
	}


	/*!
		when we start the calculations we call this method

		(we call it from WaitForCalculatingAndBlockForStop() method of
		ThreadController class)
	*/
	void Start() volatile
	{
		ResetEvent(stop);
	}


	/*!
		it returns 'true' if there was a stop signal

		it's useful for the second thread and functions which take plenty of time
		for calculating, when this method returns 'true' those methods break the
		calculations and return without returning a correct value
	*/
	virtual bool WasStopSignal() const volatile
	{
		if( WaitForSingleObject(stop,0) == WAIT_OBJECT_0 )
			return true;

	return false;
	}


private:

// manual-reset, initialized as non-signaled
HANDLE stop;

};


#endif
