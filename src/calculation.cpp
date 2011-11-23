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

#include "compileconfig.h"
#include "parsermanager.h"
#include "tabs.h"
#include <process.h>

/*!
	the function for the second thread
*/
unsigned __stdcall CalculationsProcedure(void *)
{
	ParserManager parser_manager;
	parser_manager.Init();

	// the main loop of calculations
	while( GetPrgRes()->GetThreadController()->WaitForCalculatingAndBlockForStop() )
	{
		HWND main_window = GetPrgRes()->GetMainWindow();

		// for the first we must copy all variables which we're using
		// (at this moment all calling from the main thread are stopped)
		parser_manager.MakeCopyOfVariables();
		
		// then we can set 'thread_controller' as being ready for the 'stop' signal
		GetPrgRes()->GetThreadController()->ReadyForStop();
		// (now the main thread can call various methods for changing the state)

		// and we're cleaning the output edit and sending a message about calculating 
		SetDlgItemText(main_window, IDC_OUTPUT_EDIT, "");
		PostMessage(main_window, WM_SET_LAST_ERROR, (WPARAM)ttmath::err_still_calculating, 0);

		// we're showing info about converting
		parser_manager.ShowConvertingInfo();

		// and finally we're parsing the input string
		parser_manager.Parse();

		// after parsing we're sending a message about the result of parsing
		PostMessage(main_window, WM_SET_LAST_ERROR, (WPARAM)parser_manager.GetLastCode(), 0);

		// at the end we're printing the result
		// if there was a stop signal we continue the main loop without printing any values
		if( ! GetPrgRes()->GetThreadController()->WasStopSignal() )
			if( parser_manager.PrintResult() )
				PostMessage(main_window, WM_SET_LAST_ERROR, (WPARAM)parser_manager.GetLastCode(), 0);


	}


_endthreadex(0);
return 0;
}

 
