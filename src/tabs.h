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

#ifndef headerfiletabs
#define headerfiletabs

#include <windows.h>
#include <commctrl.h>
#include <string>
#include <memory>

#include "resource.h"
#include "messages.h"
#include <ttmath/ttmathtypes.h>
#include "programresources.h"



namespace TabWindowFunctions
{
extern int tab_standard;
extern int tab_variables;
extern int tab_functions;
extern int tab_precision;
extern int tab_display;
extern int tab_convert;
extern ttmath::ErrorCode last_code;

	void PrintErrorCode();
	BOOL CALLBACK TabWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetLanguage(HWND hTab);
	void SetSizeOfVariablesList(HWND tab, int tabx, int taby, int borderx, int bordery);
	void SetSizeOfFunctionsList(HWND tab, int tabx, int taby, int borderx, int bordery);
	void SetSizeOfConvertingLists(HWND tab, int tabx, int taby, int borderx, int bordery);
	BOOL WmTabCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetSizeOfDialogs();

	namespace Variables
	{
		char * ChangeToSmallLetters(char * string);
		char * StripWhiteCharacters(char * string);

		int AddNewItemToVariableList(HWND list, const std::string & name, const std::string & value);
		int GetSelectedItem(HWND list);
		void SelectOnlyOneItem(HWND list, int id);

		BOOL WmTabCommand_AddVariable(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		BOOL WmTabCommand_EditVariable(HWND hWnd, UINT message, WPARAM wParam, LPARAM);
		BOOL WmTabCommand_DeleteVariable(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	}

	namespace Functions
	{
		int AddNewItemToFunctionList(HWND list, const std::string & name, const std::string & value, int parameters);

		BOOL WmTabCommand_AddFunction(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		BOOL WmTabCommand_EditFunction(HWND hWnd, UINT message, WPARAM wParam, LPARAM);
		BOOL WmTabCommand_DeleteFunction(HWND hWnd, UINT message, WPARAM wParam, LPARAM);
	}
}


using TabWindowFunctions::Variables::GetSelectedItem;

#endif
