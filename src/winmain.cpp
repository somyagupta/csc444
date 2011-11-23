/*
 * This file is a part of TTCalc - a mathematical calculator
 * and is distributed under the (new) BSD licence.
 * Author: Tomasz Sowa <t.sowa@ttmath.org>
 */

/* 
 * Copyright (c) 2006-2011, Tomasz Sowa
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
#include "winmain.h"
#include "update.h"

/*!
	\file mainwin.cpp
    \brief There's defined the entry point to the application in this file
*/

#include <string>
#include <ttmath/ttmath.h>


/*!
	the application starts here
*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR par, int)
{
	if( strcmp(par, "createconfig") == 0 )
	{
		GetPrgRes()->SetConfigName();
		GetPrgRes()->SaveToFile();
		return 0;
	}

	try
	{
	GetPrgRes()->SetInstance(hInstance);

	// we're using GetLanguages() simultaneously in two threads, this is to be sure
	// that everything is initialized before the second thread is started
	GetPrgRes()->GetLanguages()->InitAll();
	GetPrgRes()->GetLanguages()->SetCurrentLanguage(Languages::en);
	//GetPrgRes()->GetConvert()->Init();
	GetPrgRes()->SetNameOfFiles();
	GetPrgRes()->ReadFromFile();

	INITCOMMONCONTROLSEX common_ctrl;
	common_ctrl.dwSize = sizeof(common_ctrl);
	common_ctrl.dwICC  = ICC_WIN95_CLASSES;

	if( !InitCommonControlsEx(&common_ctrl) )
		return ShowError( Languages::cant_init_common_controls );

	if( !GetPrgRes()->GetThreadController()->Init() )
		return ShowError( Languages::cant_init_calculations );

	unsigned int thread_id;
	uintptr_t thread_handle;
	if( (thread_handle = _beginthreadex(0,0,CalculationsProcedure, 0, 0, &thread_id)) == 0 )
		return ShowError( Languages::cant_create_thread );

	bool maximized_from_file = GetPrgRes()->GetMaximized();
	CreateDialog( hInstance, MAKEINTRESOURCE(IDD_MAIN_DIALOG), 0, MainWindowProc);

	if( !GetPrgRes()->GetMainWindow() )
		return ShowError( Languages::cant_create_main_window );
 
	if( maximized_from_file )
	{
		GetPrgRes()->SetMaximized(true);
		ShowWindow(GetPrgRes()->GetMainWindow(), SW_SHOWMAXIMIZED);
	}
	else
	{
		GetPrgRes()->SetMaximized(false);
		ShowWindow(GetPrgRes()->GetMainWindow(), SW_SHOWNORMAL);
	}

	if( !CreatePadWindow() )
		return ShowError( Languages::cant_create_pad );

	// checking for an update (in an another thread)
	CheckUpdateInBackground();

	// the main loop of messages
	MainMessagesLoop();

	// there can be another variables/functions saved by other instantion of TTCalc
	// we're reading them now
	GetPrgRes()->ReadVariablesFunctionsFromFile();
	GetPrgRes()->SaveToFile();

	DestroyPadWindow();
	CloseHandle( (HANDLE)thread_handle );

	}
	catch(...)
	{
		ShowError( Languages::unknown_error );
	}

return 0;
}


/*!
	it displays a message box with an error
	(it's only used during initiation)
*/
int ShowError( Languages::GuiMsg error_code )
{
	MessageBox(0, GetPrgRes()->GetLanguages()->GuiMessage( error_code ),
				  GetPrgRes()->GetLanguages()->GuiMessage( Languages::message_box_error_caption ),
				  MB_ICONERROR);

return static_cast<int>( error_code );
}


bool AcceleratorForMainWindow(MSG & msg)
{
bool sent = true;

	switch( msg.wParam )
	{
		case 'C':
			sent = MainWindowFunctions::CopySpecial();
			break;

		case 'R':
			sent = MainWindowFunctions::CopyResult();
			break;

		case 'V':
			sent = MainWindowFunctions::PasteFormula();
			break;

		case 'B':
			sent = SendMessage(GetPrgRes()->GetMainWindow(), WM_COMMAND, IDM_EDIT_COPY_BOTH, 0);
			break;

		case 'W':
			sent = SendMessage(GetPrgRes()->GetMainWindow(), WM_COMMAND, IDM_EDIT_SWAP, 0);
			break;

		case '0':
			SendMessage(GetPrgRes()->GetMainWindow(), WM_COMMAND, IDC_BUTTON_BRACKETS_ALL, 0);
			break;

		default:
			sent = false;
	}

return sent;
}


bool AcceleratorForPadEdit(MSG & msg)
{
	if( msg.wParam == 'A' )
	{
		SendMessage(GetPrgRes()->GetPadWindow(), WM_COMMAND, MENUPAD_EDIT_SELECTALL, 0);
		return true;
	}

return false;
}


bool CheckAcceleratorKeys(MSG & msg)
{
	if( msg.message != WM_KEYDOWN )
		return false;
	
	if( (GetKeyState(VK_CONTROL) & 0xff00) == 0 )
		return false;

	if( msg.hwnd == GetPrgRes()->GetPadEdit() )
		return AcceleratorForPadEdit(msg);

return AcceleratorForMainWindow(msg);
}


void SendMessage(MSG & msg)
{
	bool sent = CheckAcceleratorKeys(msg);

	// we give our own special function for navigating from the keyboard
	// if our function returns false then we use a standard navigation
	// from the system
	if( !sent && msg.message == WM_KEYDOWN &&
	  ( msg.wParam == VK_TAB  || 
	    msg.wParam == VK_DOWN || msg.wParam == VK_UP  ||
	    msg.wParam == VK_LEFT || msg.wParam == VK_DOWN  ) )

	{	
		if( MainWindowFunctions::SetNextFocus(msg.wParam) )
			sent = true;
	}


	// firt we try to send our message to dialogs
	// (the dialogs on the tab control)
	if( !sent )
	{
		for(unsigned int i=0 ; i<GetPrgRes()->HowManyTabWindows() && !sent ; ++i)
		{
			if( IsDialogMessage(GetPrgRes()->GetTabWindow(i), &msg) )
				sent = true;
		}
	}

	// if it's not a message to any of our dialogs we send it
	// to the main window (it's a dialog as well)
	if( !sent )
	{
		if( !IsDialogMessage(GetPrgRes()->GetMainWindow(), &msg) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}


/*!
	the main loop of messages
*/
void MainMessagesLoop()
{
MSG msg;

	while( GetMessage(&msg,0,0,0) )
		SendMessage(msg);
}



/*!
	if there are some messages then they will be sent
*/
void CheckMessages()
{
MSG msg;

	while( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) )
		SendMessage(msg);
}



/*!
	the window-procedure for the main window
*/
BOOL CALLBACK MainWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static Messages<BOOL> messages;

	if( messages.Empty() )
		// initiation
		MainWindowFunctions::CreateMainMessagesTable(messages);

// in the Messages class we're using the std::map
// so that we have the logarythmic time to find the special function to call
return messages.Call(message, hWnd, message, wParam, lParam);
}


