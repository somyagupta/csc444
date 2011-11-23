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
namespace Functions
{
std::string caption,name,value;
int parameters;
bool adding;


BOOL CALLBACK DialogProcFunction(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static const int temporary_buffer_size = 200;
char * pchar;
char short_buffer[20];
int i;

	switch(message)
	{
	case WM_INITDIALOG:
		SetWindowText(hWnd, caption.c_str());
		
		SetDlgItemText(hWnd, IDC_STATIC_FUNCTION_NAME,  GetPrgRes()->GetLanguages()->GuiMessage(Languages::list_functions_header_1));
		SetDlgItemText(hWnd, IDC_STATIC_FUNCTION_PARAM, GetPrgRes()->GetLanguages()->GuiMessage(Languages::list_functions_header_2));
		SetDlgItemText(hWnd, IDC_STATIC_FUNCTION_VALUE, GetPrgRes()->GetLanguages()->GuiMessage(Languages::list_functions_header_3));

		SetDlgItemText(hWnd, IDOK,    GetPrgRes()->GetLanguages()->GuiMessage(Languages::button_ok));
		SetDlgItemText(hWnd, IDCANCEL,GetPrgRes()->GetLanguages()->GuiMessage(Languages::button_cancel));

		SetDlgItemText(hWnd,IDC_EDIT_FUNCTION_NAME, name.c_str());
		SetDlgItemText(hWnd,IDC_EDIT_FUNCTION_VALUE, value.c_str());

		for(i=0 ; i<10 ; ++i)
		{
			sprintf(short_buffer,"%u", i);
			SendDlgItemMessage(hWnd,IDC_COMBO_FUNCTION_PARAM,CB_ADDSTRING,0,(LPARAM)short_buffer);
		}

		if( parameters < 0 )
			parameters = 0;
		else
		if( parameters > 9 )
			parameters = 9;

		SendDlgItemMessage(hWnd,IDC_COMBO_FUNCTION_PARAM,CB_SETCURSEL,parameters,0);

		if( adding )
		{
			SetFocus(GetDlgItem(hWnd,IDC_EDIT_FUNCTION_NAME));
		}
		else
		{
			SetFocus(GetDlgItem(hWnd,IDC_EDIT_FUNCTION_VALUE));
			SendDlgItemMessage(hWnd, IDC_EDIT_FUNCTION_VALUE, EM_SETSEL, value.size(), value.size());
		}

		return false;

	case WM_COMMAND:
		if( LOWORD(wParam) == IDOK )
		{
			pchar = new char[temporary_buffer_size];			

			GetDlgItemText(hWnd, IDC_EDIT_FUNCTION_NAME, pchar, temporary_buffer_size);
			name = Variables::StripWhiteCharacters(pchar);

			GetDlgItemText(hWnd, IDC_EDIT_FUNCTION_VALUE, pchar, temporary_buffer_size);
			value = Variables::StripWhiteCharacters(pchar);

			parameters = (int)SendDlgItemMessage(hWnd,IDC_COMBO_FUNCTION_PARAM,CB_GETCURSEL,0,0);

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
		message = GetPrgRes()->GetLanguages()->GuiMessage(Languages::dialog_box_add_function_function_exists);
		break;

	case ttmath::err_incorrect_name:
		message = GetPrgRes()->GetLanguages()->GuiMessage(Languages::dialog_box_add_function_incorrect_name);
		break;

 	case ttmath::err_unknown_object:
		message = GetPrgRes()->GetLanguages()->GuiMessage(Languages::dialog_box_edit_function_unknown_function);
		break;

	default:
		message = GetPrgRes()->GetLanguages()->GuiMessage(Languages::unknown_error);
		break;
	}


	MessageBox(	hWnd, message, 
		GetPrgRes()->GetLanguages()->GuiMessage(Languages::message_box_caption),
		MB_ICONERROR);
}



int AddNewItemToFunctionList(HWND list, const std::string & name,
							  const std::string & value, int parameters)
{
LVITEM item;
char buffer[20];

	item.mask     = LVIF_TEXT;
	item.pszText  = const_cast<char*>( name.c_str() );
	item.iSubItem = 0;

	int id = ListView_InsertItem(list, &item);

	sprintf(buffer,"%u", parameters);
	ListView_SetItemText(list,id,1, buffer);

	ListView_SetItemText(list,id,2,const_cast<char*>( value.c_str() ));

return id;
}





void ChangeItemInFunctionList(	HWND list, int id, const std::string & old_name,
								const std::string & new_name, const std::string & value,
								int parameters)
{
	if( old_name != new_name )
	{
		ListView_DeleteItem(list, id);
		// as we're using ListView_DeleteItem here the WM_NOTIFY is generated
		// and system will call SetDisablingEditDeleteFunctionButtons
		// and eventually the focus will be changed into the 'add' button
		// (it's a small inconvenience)

		int new_id = AddNewItemToFunctionList(list, new_name, value, parameters);
		ListView_SetItemState(list,new_id, LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
	}
	else
	{
		char buffer[20];
		sprintf(buffer,"%u", parameters);
		ListView_SetItemText(list,id,1, buffer);
		ListView_SetItemText(list,id,2,const_cast<char*>( value.c_str() ));
	}
}




BOOL WmTabCommand_AddFunction(HWND, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd  = GetPrgRes()->GetTabWindow(TabWindowFunctions::tab_functions);
	caption    = GetPrgRes()->GetLanguages()->GuiMessage(Languages::dialog_box_add_function_caption);
	name       = "";
	value      = "";
	parameters = 1;
	ttmath::ErrorCode code;
	adding   = true;

	do
	{
		if( !DialogBox(GetPrgRes()->GetInstance(), MAKEINTRESOURCE(IDD_DIALOG_ADD_FUNCTION), hWnd, DialogProcFunction) )
			break;
	
		HWND list = GetDlgItem(hWnd, IDC_FUNCTIONS_LIST);

		GetPrgRes()->GetThreadController()->StopCalculating();
		code = GetPrgRes()->GetFunctions()->Add(name, value, parameters);
		GetPrgRes()->FunctionsChanged();
		GetPrgRes()->GetThreadController()->StartCalculating();

		if( code != ttmath::err_ok )
			ShowError(hWnd, code);
		else
		{
			int id = AddNewItemToFunctionList(list, name, value, parameters);
			Variables::SelectOnlyOneItem(list,id);
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
BOOL WmTabCommand_EditFunction(HWND, UINT message, WPARAM wParam, LPARAM)
{
HWND hWnd = GetPrgRes()->GetTabWindow(TabWindowFunctions::tab_functions);
HWND list = GetDlgItem(hWnd, IDC_FUNCTIONS_LIST);
adding  = false;

	if( ListView_GetSelectedCount(list) != 1 )
		// there must be only one item selected
		return true;

	int id = Variables::GetSelectedItem(list);
	if( id == -1 )
		return true;

	const int buffer_size = 300;
	char * buffer = new char[buffer_size];
	std::string old_name;
	ttmath::ErrorCode code;

	caption = GetPrgRes()->GetLanguages()->GuiMessage(Languages::dialog_box_edit_function_caption);
	
	ListView_GetItemText(list,id,0,buffer, buffer_size);
	old_name = name = buffer;

	ListView_GetItemText(list,id,1,buffer, buffer_size);
	parameters = atoi(buffer);

	ListView_GetItemText(list,id,2,buffer, buffer_size);
	value = buffer;

	delete [] buffer;

	do
	{
		if( !DialogBox(GetPrgRes()->GetInstance(), MAKEINTRESOURCE(IDD_DIALOG_ADD_FUNCTION), hWnd, DialogProcFunction) )
			break;

		GetPrgRes()->GetThreadController()->StopCalculating();

		// firstly we're trying to change the name
		code = GetPrgRes()->GetFunctions()->EditName(old_name, name);
		if( code == ttmath::err_ok )
			// if we've changed the name then we're changing the value and parameters
			code = GetPrgRes()->GetFunctions()->EditValue(name, value, parameters);

		GetPrgRes()->FunctionsChanged();
		GetPrgRes()->GetThreadController()->StartCalculating();

		if( code != ttmath::err_ok )
			ShowError(list, code);
		else
		{
			ChangeItemInFunctionList(list, id, old_name, name, value, parameters);
			GetPrgRes()->ReadVariablesFunctionsFromFile();
			GetPrgRes()->SaveToFile();
		}
	}
	while( code != ttmath::err_ok );

return true;
}


BOOL WmTabCommand_DeleteFunction(HWND, UINT, WPARAM, LPARAM)
{
HWND hWnd = GetPrgRes()->GetTabWindow(TabWindowFunctions::tab_functions);
HWND list = GetDlgItem(hWnd, IDC_FUNCTIONS_LIST);
int items = ListView_GetSelectedCount(list);

	if( items == 0 )
		// there must be at least one item selected
		return true;

	if( items > 1 )
	{
		// we're showing a message to confirm deleting
		if( MessageBox( hWnd,
						GetPrgRes()->GetLanguages()->GuiMessage(Languages::dialog_box_delete_function_confirm),
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
			if( GetPrgRes()->GetFunctions()->Delete(buffer) != ttmath::err_ok )
				all_deleted = false;
			else
				ListView_DeleteItem(list, id);

			GetPrgRes()->FunctionsChanged();
		}
	}
	
	
	GetPrgRes()->GetThreadController()->StartCalculating();
	GetPrgRes()->SaveToFile();

	delete [] buffer;

	if( !all_deleted )
		// there are some items which we've not deleted
		// probably an internal error
		MessageBox(	hWnd,
					GetPrgRes()->GetLanguages()->GuiMessage(Languages::dialog_box_function_not_all_deleted),
					GetPrgRes()->GetLanguages()->GuiMessage(Languages::message_box_caption),
					MB_ICONERROR);

return true;
}





} // namespace
} // namespace
