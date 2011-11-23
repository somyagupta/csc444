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
#include "tabs.h"


namespace TabWindowFunctions
{
namespace Variables
{
std::string caption,name,value;
bool adding;

/*!
	this function changes the whole string into a string consists of small letters
	(it returns the same pointer)
*/
char * ChangeToSmallLetters(char * string)
{
	for( char * p = string ; *p>='A' && *p<='Z' ; ++p )
		*p = *p - 'A' + 'a';

return string;
}


/*!
	this function deletes white characters at the beginning and at the end of a string

	it doesn't actually erase them but returns the pointer at the first
	non-white character and sets '\0' before the last white character
*/
char * StripWhiteCharacters(char * string)
{
char * start, * end;

	IniParser::CheckWhiteCharacters(string, (const char**)&start, (const char**)&end);

	*end = 0;

return start;
}


BOOL CALLBACK DialogProcVariables(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static const int temporary_buffer_size = 200;
char * pchar;

	switch(message)
	{
	case WM_INITDIALOG:
		SetWindowText(hWnd, caption.c_str());
		
		SetDlgItemText(hWnd, IDC_STATIC_VARIABLE_NAME,  GetPrgRes()->GetLanguages()->GuiMessage(Languages::list_variables_header_1));
		SetDlgItemText(hWnd, IDC_STATIC_VARIABLE_VALUE, GetPrgRes()->GetLanguages()->GuiMessage(Languages::list_variables_header_2));

		SetDlgItemText(hWnd, IDOK,    GetPrgRes()->GetLanguages()->GuiMessage(Languages::button_ok));
		SetDlgItemText(hWnd, IDCANCEL,GetPrgRes()->GetLanguages()->GuiMessage(Languages::button_cancel));

		SetDlgItemText(hWnd,IDC_EDIT_VARIABLE_NAME, name.c_str());
		SetDlgItemText(hWnd,IDC_EDIT_VARIABLE_VALUE, value.c_str());

		if( adding)
		{
			SetFocus(GetDlgItem(hWnd,IDC_EDIT_VARIABLE_NAME));
		}
		else
		{
			SetFocus(GetDlgItem(hWnd,IDC_EDIT_VARIABLE_VALUE));
			SendDlgItemMessage(hWnd, IDC_EDIT_VARIABLE_VALUE, EM_SETSEL, value.size(), value.size());
		}

		return false;

	case WM_COMMAND:
		if( LOWORD(wParam) == IDOK )
		{
			pchar = new char[temporary_buffer_size];			

			GetDlgItemText(hWnd, IDC_EDIT_VARIABLE_NAME, pchar, temporary_buffer_size);
			name = StripWhiteCharacters(pchar);

			GetDlgItemText(hWnd, IDC_EDIT_VARIABLE_VALUE, pchar, temporary_buffer_size);
			value = StripWhiteCharacters(pchar);

			delete [] pchar;

			EndDialog(hWnd,1);
		}
		else
		if( LOWORD(wParam) == IDCANCEL )
		{
			EndDialog(hWnd,0);
			return true;
		}

		break;
	}

return false;
}

void ShowError(HWND hWnd, ttmath::ErrorCode error)
{
const char * message;

	switch( error )
	{
	case ttmath::err_object_exists:
		message = GetPrgRes()->GetLanguages()->GuiMessage(Languages::dialog_box_add_variable_variable_exists);
		break;

	case ttmath::err_incorrect_name:
		message = GetPrgRes()->GetLanguages()->GuiMessage(Languages::dialog_box_add_variable_incorrect_name);
		break;

	case ttmath::err_unknown_object:
		message = GetPrgRes()->GetLanguages()->GuiMessage(Languages::dialog_box_edit_variable_unknown_variable);
		break;

	default:
		message = GetPrgRes()->GetLanguages()->GuiMessage(Languages::unknown_error);
		break;
	}


	MessageBox(	hWnd, message, 
		GetPrgRes()->GetLanguages()->GuiMessage(Languages::message_box_caption),
		MB_ICONERROR);
}



int AddNewItemToVariableList(HWND list, const std::string & name, const std::string & value)
{
LVITEM item;

	item.mask     = LVIF_TEXT;
	item.pszText  = const_cast<char*>( name.c_str() );
	item.iSubItem = 0;

	int id = ListView_InsertItem(list, &item);
	ListView_SetItemText(list,id,1,const_cast<char*>( value.c_str() ));

return id;
}





void ChangeItemInVariableList(HWND list, int id, const std::string & old_name,
								 const std::string & new_name, const std::string & value)
{
	if( old_name != new_name )
	{
		ListView_DeleteItem(list, id);
		// as we're using ListView_DeleteItem here the WM_NOTIFY is generated
		// and system will call SetDisablingEditDeleteVariableButtons
		// and eventually the focus will be changed into the 'add' button
		// (it's a small inconvenience)

		int new_id = AddNewItemToVariableList(list, new_name, value);
		ListView_SetItemState(list, new_id, LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
	}
	else
	{
		ListView_SetItemText(list,id,1,const_cast<char*>( value.c_str() ));
	}
}


/*!
	this functions returns the first item which is selected on a given list

	we don't use ListView_GetSelectionMark because it sometimes returns 
	a wrong value especially when we have set an item as to be focused
	on the list ourselves
*/
int GetSelectedItem(HWND list)
{
int id;
int len = ListView_GetItemCount(list);

	for(id = 0 ; id<len ; ++id)
		if( ListView_GetItemState(list, id, LVIS_SELECTED) == LVIS_SELECTED )
			return id;

return -1;
}


/*!
	this function removes the selection from all items
	and sets the selection only on to the one given
*/
void SelectOnlyOneItem(HWND list, int id)
{
int len = ListView_GetItemCount(list);

	for(int i = 0 ; i<len ; ++i)
		ListView_SetItemState(list, i, 0, LVIS_FOCUSED|LVIS_SELECTED);

	ListView_SetItemState(list, id, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
}


BOOL WmTabCommand_AddVariable(HWND, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = GetPrgRes()->GetTabWindow(TabWindowFunctions::tab_variables);
	caption   = GetPrgRes()->GetLanguages()->GuiMessage(Languages::dialog_box_add_variable_caption);
	name      = "";
	value     = "";
	ttmath::ErrorCode code;
	adding = true;

	do
	{
		if( !DialogBox(GetPrgRes()->GetInstance(), MAKEINTRESOURCE(IDD_DIALOG_ADD_VARIABLE), hWnd, DialogProcVariables) )
			break;
		
		HWND list = GetDlgItem(hWnd, IDC_VARIABLES_LIST);

		GetPrgRes()->GetThreadController()->StopCalculating();
		code = GetPrgRes()->GetVariables()->Add(name, value);
		GetPrgRes()->VariablesChanged();
		GetPrgRes()->GetThreadController()->StartCalculating();

		if( code != ttmath::err_ok )
			ShowError(hWnd, code);
		else
		{
			int id = AddNewItemToVariableList(list, name, value);
			SelectOnlyOneItem(list,id);
			GetPrgRes()->ReadVariablesFunctionsFromFile();
			GetPrgRes()->SaveToFile();
		}
	}
	while( code != ttmath::err_ok );

return true;
}



/*!
	(we're also using this method directly without using the main loop of messages)
	(we don't define the 'lParam' parameter there)
*/
BOOL WmTabCommand_EditVariable(HWND, UINT message, WPARAM wParam, LPARAM)
{
HWND hWnd = GetPrgRes()->GetTabWindow(TabWindowFunctions::tab_variables);
HWND list = GetDlgItem(hWnd, IDC_VARIABLES_LIST);
adding = false;

	if( ListView_GetSelectedCount(list) != 1 )
		// there must be only one item selected
		return true;

	int id = GetSelectedItem(list);
	if( id == -1 )
		return true;

	const int buffer_size = 300;
	char * buffer = new char[buffer_size];
	std::string old_name;
	ttmath::ErrorCode code;
	caption = GetPrgRes()->GetLanguages()->GuiMessage(Languages::dialog_box_edit_variable_caption);

	ListView_GetItemText(list,id,0,buffer, buffer_size);
	old_name = name = buffer;

	ListView_GetItemText(list,id,1,buffer, buffer_size);
	value = buffer;

	delete [] buffer;

	do
	{
		if( !DialogBox(GetPrgRes()->GetInstance(), MAKEINTRESOURCE(IDD_DIALOG_ADD_VARIABLE), hWnd, DialogProcVariables) )
			break;

		GetPrgRes()->GetThreadController()->StopCalculating();

		// firstly we're trying to change the name
		code = GetPrgRes()->GetVariables()->EditName(old_name, name);
		if( code == ttmath::err_ok )
			// if we've changed the name then we're changing the value
			code = GetPrgRes()->GetVariables()->EditValue(name, value);
			// the code should be err_ok here

		GetPrgRes()->VariablesChanged();
		GetPrgRes()->GetThreadController()->StartCalculating();

		if( code != ttmath::err_ok )
			ShowError(list, code);
		else
		{
			ChangeItemInVariableList(list, id, old_name, name, value);
			GetPrgRes()->ReadVariablesFunctionsFromFile();
			GetPrgRes()->SaveToFile();
		}
	}
	while( code != ttmath::err_ok );

return true;
}


BOOL WmTabCommand_DeleteVariable(HWND, UINT, WPARAM, LPARAM)
{
HWND hWnd = GetPrgRes()->GetTabWindow(TabWindowFunctions::tab_variables);
HWND list = GetDlgItem(hWnd, IDC_VARIABLES_LIST);
int items = ListView_GetSelectedCount(list);

	if( items == 0 )
		// there must be at least one item selected
		return true;

	if( items > 1 )
	{
		// we're showing a message to confirm deleting
		if( MessageBox( hWnd,
						GetPrgRes()->GetLanguages()->GuiMessage(Languages::dialog_box_delete_variable_confirm),
						GetPrgRes()->GetLanguages()->GuiMessage(Languages::message_box_caption),
						MB_ICONWARNING | MB_YESNO) == IDNO )
			return true;
	}

	int id;
	const int buffer_size = 300;
	char * buffer = new char[buffer_size];
	bool all_deleted = true;

	GetPrgRes()->ReadVariablesFunctionsFromFile();
	GetPrgRes()->GetThreadController()->StopCalculating();

	for( id = ListView_GetItemCount(list)-1 ; id!=-1 ; --id )
	{
		if( ListView_GetItemState(list, id, LVIS_SELECTED) == LVIS_SELECTED )
		{
			ListView_GetItemText(list,id,0,buffer,buffer_size);
			if( GetPrgRes()->GetVariables()->Delete(buffer) != ttmath::err_ok )
				all_deleted = false;
			else
				ListView_DeleteItem(list, id);

			GetPrgRes()->VariablesChanged();
		}
	}

	
	GetPrgRes()->GetThreadController()->StartCalculating();
	GetPrgRes()->SaveToFile();

	delete [] buffer;

	if( !all_deleted )
		// there are some items which we've not deleted
		// probably an internal error
		MessageBox(	hWnd,
					GetPrgRes()->GetLanguages()->GuiMessage(Languages::dialog_box_variable_not_all_deleted),
					GetPrgRes()->GetLanguages()->GuiMessage(Languages::message_box_caption),
					MB_ICONERROR);

return true;
}





} // namespace
} // namespace
