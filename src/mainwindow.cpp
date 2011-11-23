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

#include <cstring>
#include "compileconfig.h"
#include "winmain.h"
#include "update.h"
#include "pad.h"
#include "bigtypes.h"
#include "misc.h"



namespace MainWindowFunctions
{




/*!
	returning true if hWnd is a handle of an edit control
*/
bool IsEditControl(HWND hWnd)
{
char buffer[30];

	GetClassName( hWnd, buffer, sizeof(buffer)/sizeof(char) );

	if( EqualStrings(buffer, "edit") )
		return true;

return false;
}



/*!
	Loop through all the controls and remove the
	dark border that the previous default push
	button has.
*/
void ClearPushButtons(HWND hWnd)
{
HWND controls = GetWindow(hWnd, GW_CHILD);
LRESULT dlg_code;

	while(controls)
	{
		dlg_code = SendMessage(controls, WM_GETDLGCODE, 0, 0);

		if(dlg_code & DLGC_DEFPUSHBUTTON)
			SendMessage(controls, BM_SETSTYLE, (WPARAM)BS_PUSHBUTTON, true);

		controls = GetWindow(controls, GW_HWNDNEXT);
	}
}


/*!
	this method checks whether 'button' is a push button and if so
	sets it as the default push button (with black border)
*/
void CheckAndSetDefPushButton(HWND button)
{
	LRESULT dlg_code = SendMessage(button, WM_GETDLGCODE, 0, 0);

	if(dlg_code & DLGC_UNDEFPUSHBUTTON)
		SendMessage(button, BM_SETSTYLE, (WPARAM)BS_DEFPUSHBUTTON, true);
}


HWND GetNextControlInsideTab(HWND input_edit, HWND tab, HWND tab_dialog, HWND focus,
							 HWND first_control,
							 WPARAM wParam, bool direction_next)
{
	LRESULT dlg_code = SendMessage(focus, WM_GETDLGCODE, 0, 0);
	HWND next = 0;

	if( focus == first_control )
	{
		if( wParam==VK_TAB ||
			((dlg_code & DLGC_WANTARROWS)==0 && (dlg_code & DLGC_RADIOBUTTON)==0) )
		{
			if( !direction_next )
				next = tab;
			else
			{
				// if we have only radio buttons (precision tab)
				// the next item from GetNextDlgTabItem (or if we allow 
				// the system to look for it) will be the same as the focus
				// we must set it on the input_edit
				HWND nexttmp = GetNextDlgTabItem(tab_dialog, focus, false);
				if( nexttmp == focus )
					next = input_edit;
			}
		}
	}
	else
	// the focus is somewhere inside a tab
	{

		if( wParam==VK_TAB ||
			((dlg_code & DLGC_WANTARROWS)==0 && (dlg_code & DLGC_RADIOBUTTON)==0) )
		{
			if( direction_next )
			{
				HWND nexttmp = GetNextDlgTabItem(tab_dialog, focus, false);

				if( nexttmp == first_control )
				{
					next = input_edit;
				}
			}
		}
	}

return next;
}

/*!
	we're trying to get the next or previous control which should have 
	the focus now
*/
HWND GetNextControl(HWND focus, HWND main_window, HWND tab, WPARAM wParam)
{
	HWND next           = 0;
	HWND output_edit    = GetDlgItem(main_window, IDC_OUTPUT_EDIT);
	HWND input_edit     = GetDlgItem(main_window, IDC_INPUT_EDIT);
	bool shift          = (GetKeyState(VK_SHIFT) & 0xffff0000) != 0;
	bool direction_next = true;
	int tab_sel         = TabCtrl_GetCurSel(tab);
	HWND tab_dialog     = GetPrgRes()->GetTabWindow(tab_sel);
	HWND first_control  = GetNextDlgTabItem(tab_dialog, 0, false);


	// if an user has pressed 'up' or 'left' or 'tab'+'shift'
	// that means we're going 'back'
	if( wParam==VK_UP || wParam==VK_LEFT || (wParam==VK_TAB && shift) )
		direction_next = false;


	if( focus == input_edit )
	{
		if( wParam==VK_TAB || wParam==VK_UP || wParam==VK_DOWN ) 
		{
			if( direction_next )
			{
				next = output_edit;
			}
			else
			{
				next = GetNextDlgTabItem(tab_dialog, first_control, true);
				ClearPushButtons(tab_dialog);
			}
		}
	}
	else
	if( focus == output_edit )
	{
		if( wParam==VK_LEFT || wParam==VK_RIGHT )
			return 0;

		if( direction_next )
			// if we are on the output edit and the direction is 'forward' 
			// then the next control will be the 'tab'
			next = tab;
		else
			next = input_edit;
	}
	else
	if( focus == tab )
	{
		// if the tab control has the focus and an user has pressed 'left' or 'right'
		// we allow the system to manipulate the focus
		if( wParam==VK_LEFT || wParam==VK_RIGHT )
			return 0;

		if( direction_next )
		{
			// we're getting the first control on a specific dialog on the selected tab
			next = first_control;
			ClearPushButtons(tab_dialog);
		}
		else
		{
			// if the direction is 'back' we're setting the focus onto the output edit
			next = output_edit;
		}
	}
	else
		next = GetNextControlInsideTab(input_edit, tab, tab_dialog, focus,
							 first_control, wParam, direction_next);


return next;
}



/*!
	this function sets the focus on a specific control
	(the standard procedure from the operating system doesn't work well because
	the tab control would be omitted)

	remember to set WS_EX_CONTROLPARENT to the tab control and
	to all dialog boxes on the tab
*/
bool SetNextFocus(WPARAM wParam)
{
	HWND next = 0;
	HWND main_window = GetPrgRes()->GetMainWindow();
	HWND focus       = GetFocus();
	HWND tab         = GetDlgItem(main_window, IDC_TAB);

	next = GetNextControl(focus, main_window, tab, wParam);

	if( next )
	{
		SetFocus(next);
		CheckAndSetDefPushButton(next);

		return true;
	}

return false;
}


/*!
*/
bool CopyResult()
{
	HWND main_window = GetPrgRes()->GetMainWindow();	
	HWND output_edit = GetDlgItem(main_window, IDC_OUTPUT_EDIT);
	DWORD out_sel_start, out_sel_end;

	// we're getting the selection on the output edit
	// there may be no selection
	SendMessage(output_edit, EM_GETSEL, (WPARAM)&out_sel_start, (LPARAM)&out_sel_end);

	// setting the selection for the whole control
	SendMessage(output_edit, EM_SETSEL, 0, -1);

	// copying the result
	SendMessage(output_edit, WM_COPY, 0, 0);

	// and restoring the selection to the previous state
	SendMessage(output_edit, EM_SETSEL, out_sel_start, out_sel_end);

return true;
}



/*!
	if the focus is not on an edit window
	or it is on an edit but the edit has no selection
	then we're copying the result

	this is used when Ctrl+C is pressed
*/
bool CopySpecial()
{
HWND focus = GetFocus();

	if( IsEditControl(focus) )
	{
		DWORD sel_start, sel_end;
		SendMessage(focus, EM_GETSEL, (WPARAM)&sel_start, (LPARAM)&sel_end);

		if( sel_start != sel_end ) // something is selected
			return false;
	}

	CopyResult();

return true;
}





/*!
	this function is called when CTRL+V has been pressed
	this is not Paste but PasteFormula (everything is copied to the input edit, even
	if the focus is on an another edit)
*/
bool PasteFormula()
{
	HWND main_window = GetPrgRes()->GetMainWindow();	
	HWND input_edit = GetDlgItem(main_window, IDC_INPUT_EDIT);

	SendMessage(input_edit, WM_PASTE, 0, 0);
	SetFocus(input_edit);

return true;
}



void SetActiveTab(unsigned int i)
{
static int last_shown_dialog = -1;

	if( last_shown_dialog != -1 )
		ShowWindow( GetPrgRes()->GetTabWindow(last_shown_dialog), SW_HIDE);

	if( i >= GetPrgRes()->HowManyTabWindows() )
	{
		last_shown_dialog = -1;
		return;
	}

	ShowWindow(GetPrgRes()->GetTabWindow(i), SW_SHOWNORMAL);
	last_shown_dialog = i;
}


inline void SetMenuLanguageItem(HMENU menu, DWORD menu_id, Languages::GuiMsg msg)
{
Languages * pLang = GetPrgRes()->GetLanguages();

	ModifyMenu(menu, menu_id, MF_BYCOMMAND|MF_STRING, menu_id, pLang->GuiMessage(msg) );
}


void SetMenuLanguage(HWND hWnd)
{
HMENU menu = GetMenu(hWnd);

	if( !menu )
		return;

	Languages * pl = GetPrgRes()->GetLanguages();

	MENUITEMINFO mi;
	mi.cbSize = sizeof(mi);
	mi.fMask = MIIM_TYPE;
	mi.fType = MFT_STRING;

	// on popup menus we're using indexes because they don't want to work correctly
	// with the identifiers 
	mi.dwTypeData = const_cast<char*>( pl->GuiMessage(Languages::menu_view) );
	SetMenuItemInfo(menu, IDM_VIEW_INDEX, true, &mi);

	mi.dwTypeData = const_cast<char*>( pl->GuiMessage(Languages::menu_edit) );
	SetMenuItemInfo(menu, IDM_EDIT_INDEX, true, &mi);

	mi.dwTypeData = const_cast<char*>( pl->GuiMessage(Languages::menu_help) );
	SetMenuItemInfo(menu, IDM_HELP_INDEX, true, &mi);

	if( GetPrgRes()->GetUpdateExists() )
	{
		mi.dwTypeData = const_cast<char*>( pl->GuiMessage(Languages::menu_update_available) );
		SetMenuItemInfo(menu, IDM_UPDATE_AVAILABLE_INDEX, true, &mi);
	}

	mi.dwTypeData = const_cast<char*>( pl->GuiMessage(Languages::menu_language) );
	SetMenuItemInfo(GetSubMenu(menu,IDM_VIEW_INDEX), IDM_LANGUAGE_INDEX, true, &mi);



	SetMenuLanguageItem(menu, IDM_NEW_WINDOW,		Languages::menu_view_new_window);
	SetMenuLanguageItem(menu, IDM_NORMAL_VIEW,		Languages::menu_view_normal_view);
	SetMenuLanguageItem(menu, IDM_COMPACT_VIEW,		Languages::menu_view_compact_view);
	SetMenuLanguageItem(menu, IDM_ALWAYS_ON_TOP,	Languages::menu_view_always_on_top);
	SetMenuLanguageItem(menu, IDM_PAD,				Languages::menu_view_pad);
	SetMenuLanguageItem(menu, IDM_LANGUAGE_ENGLISH, Languages::menu_view_lang_english);
	SetMenuLanguageItem(menu, IDM_LANGUAGE_POLISH,	Languages::menu_view_lang_polish);
	SetMenuLanguageItem(menu, IDM_LANGUAGE_SPANISH,	Languages::menu_view_lang_spanish);
	SetMenuLanguageItem(menu, IDM_LANGUAGE_DANISH,	Languages::menu_view_lang_danish);
	SetMenuLanguageItem(menu, IDM_LANGUAGE_CHINESE,	Languages::menu_view_lang_chinese);
	SetMenuLanguageItem(menu, IDM_LANGUAGE_RUSSIAN,	Languages::menu_view_lang_russian);
	SetMenuLanguageItem(menu, IDM_LANGUAGE_SWEDISH,	Languages::menu_view_lang_swedish);
	SetMenuLanguageItem(menu, IDM_LANGUAGE_ITALIAN,	Languages::menu_view_lang_italian);
	SetMenuLanguageItem(menu, IDM_LANGUAGE_GERMAN,	Languages::menu_view_lang_german);
	SetMenuLanguageItem(menu, IDM_CLOSE_PROGRAM,	Languages::menu_view_close_program);
	SetMenuLanguageItem(menu, IDM_EDIT_UNDO,		Languages::menu_edit_undo);
	SetMenuLanguageItem(menu, IDM_EDIT_CUT,			Languages::menu_edit_cut);
	SetMenuLanguageItem(menu, IDM_EDIT_COPY,		Languages::menu_edit_copy);
	SetMenuLanguageItem(menu, IDM_EDIT_PASTE,		Languages::menu_edit_paste);
	SetMenuLanguageItem(menu, IDM_EDIT_DEL,			Languages::menu_edit_del);
	SetMenuLanguageItem(menu, IDM_EDIT_PASTE_FORMULA,	Languages::menu_edit_paste_formula);
	SetMenuLanguageItem(menu, IDM_EDIT_COPY_RESULT, Languages::menu_edit_copy_result);
	SetMenuLanguageItem(menu, IDM_EDIT_COPY_BOTH,	Languages::menu_edit_copy_both);
	SetMenuLanguageItem(menu, IDM_EDIT_SELECT_ALL,	Languages::menu_edit_select_all);
	SetMenuLanguageItem(menu, IDM_EDIT_BRACKET,		Languages::menu_edit_bracket);
	SetMenuLanguageItem(menu, IDM_EDIT_SWAP,		Languages::menu_edit_swap);
	SetMenuLanguageItem(menu, IDM_HELP_HELP,		Languages::menu_help_help);
	SetMenuLanguageItem(menu, IDM_HELP_PROJECT_PAGE, Languages::menu_help_project_page);
	SetMenuLanguageItem(menu, IDM_HELP_CHECK_UPDATE, Languages::menu_help_check_update);
	SetMenuLanguageItem(menu, IDM_HELP_ABOUT,		Languages::menu_help_about);

	DrawMenuBar(hWnd);
}


void WmInitDialogCalculateMainWindowSize(HWND hTab)
{
RECT r_tab, r_main, r_edit2;

	GetWindowRect(hTab, &r_tab);

	HWND main = GetPrgRes()->GetMainWindow();
	GetWindowRect(main, &r_main);

	HWND output_edit = GetDlgItem(main, IDC_OUTPUT_EDIT);
	GetWindowRect(output_edit, &r_edit2);

	int cy_compact = (r_edit2.bottom - r_main.top) + GetSystemMetrics(SM_CYSIZEFRAME);
	int cy_normal  = (r_tab.bottom - r_tab.top) + (r_tab.top - r_main.top) + GetSystemMetrics(SM_CYSIZEFRAME) + 8;
	int cx         = r_tab.right - r_tab.left + 2 * GetSystemMetrics(SM_CXSIZEFRAME);

	GetPrgRes()->SetYSizeNormal ( cy_normal );
	GetPrgRes()->SetYSizeCompact( cy_compact );
	GetPrgRes()->SetXSizeMin    ( cx );
}



HWND WmInitDialogCreateControlsOnTab(HWND hTab, LPCSTR dialog, DLGPROC dialog_proc)
{
HRSRC tab1res = FindResource(GetPrgRes()->GetInstance(), dialog, RT_DIALOG);
HGLOBAL tab1global  = LoadResource(GetPrgRes()->GetInstance(), tab1res);
DLGTEMPLATE * tab1template = (DLGTEMPLATE*)LockResource(tab1global);

	if( tab1template )
	{
		HWND hDialog = CreateDialogIndirect(GetPrgRes()->GetInstance(), tab1template, hTab , dialog_proc);
		
		// for windres

			SetWindowLong(hDialog, GWL_STYLE, GetWindowLong(hDialog, GWL_STYLE) & ~(WS_CAPTION | WS_OVERLAPPED) );

		// 

		int frame_cx   = GetSystemMetrics(SM_CXEDGE);
		int frame_cy   = GetSystemMetrics(SM_CYEDGE);
		int space = 10; // some more space between the tab items and child tabs

		RECT tab_item_r;
		TabCtrl_GetItemRect(hTab, 0, &tab_item_r);
		int tab_item = tab_item_r.bottom;

		RECT r;
		GetWindowRect(hTab, &r);

		SetWindowPos(hDialog, HWND_TOP, frame_cx, frame_cy + tab_item + space,
						r.right-r.left- 2*frame_cx,
						r.bottom-r.top- 2*frame_cy - tab_item - space, 0); 

	return hDialog;
	}

return 0;
}


void WmInitDialogCreateTab(HWND hTab, int tab_index, int tab_resource, DLGPROC tab_proc)
{
	HWND dialogh = WmInitDialogCreateControlsOnTab(
					hTab,
					MAKEINTRESOURCE(tab_resource),
					tab_proc );

	GetPrgRes()->SetTabWindow( tab_index, dialogh );
}


void WmInitDialogCreateAndInitTabs(HWND hTab)
{
using namespace TabWindowFunctions;

TCITEM tab_item;

	// the name of a tab we'll set when we call the 'SetLanguage()' function
	tab_item.mask = 0;

	int tab_inc = 0;

	tab_standard  = tab_inc++;
	tab_variables = tab_inc++;
	tab_functions = tab_inc++;

	#ifndef TTCALC_PORTABLE
	tab_precision = tab_inc++;
	#else
	tab_precision = -1; // this one will not be used
	#endif

	tab_display   = tab_inc++;

	#ifdef TTCALC_CONVERT
	tab_convert   = tab_inc++;
	#else
	tab_convert   = -1;
	#endif

	// this insertion must be in the ascending order
	// (the second parameter of 'TabCtrl_InsertItem')
	TabCtrl_InsertItem(hTab, tab_standard,  &tab_item);
	TabCtrl_InsertItem(hTab, tab_variables, &tab_item);
	TabCtrl_InsertItem(hTab, tab_functions, &tab_item);

	#ifndef TTCALC_PORTABLE
	TabCtrl_InsertItem(hTab, tab_precision, &tab_item);
	#endif

	TabCtrl_InsertItem(hTab, tab_display,   &tab_item);

	#ifdef TTCALC_CONVERT
	TabCtrl_InsertItem(hTab, tab_convert,   &tab_item);
	#endif

	WmInitDialogCreateTab(hTab, tab_standard,  IDD_DIALOG_STANDARD,  TabWindowProc);
	WmInitDialogCreateTab(hTab, tab_variables, IDD_DIALOG_VARIABLES, TabWindowProc);
	WmInitDialogCreateTab(hTab, tab_functions, IDD_DIALOG_FUNCTIONS, TabWindowProc);

	#ifndef TTCALC_PORTABLE
	WmInitDialogCreateTab(hTab, tab_precision, IDD_DIALOG_PRECISION, TabWindowProc);
	#endif

	WmInitDialogCreateTab(hTab, tab_display,   IDD_DIALOG_DISPLAY,   TabWindowProc);

	#ifdef TTCALC_CONVERT
	WmInitDialogCreateTab(hTab, tab_convert,   IDD_DIALOG_CONVERT,   TabWindowProc);
	#endif

	SetSizeOfDialogs();

	SendMessage(GetPrgRes()->GetTabWindow(tab_variables), WM_INIT_TAB_VARIABLES, 0,0);
	SendMessage(GetPrgRes()->GetTabWindow(tab_functions), WM_INIT_TAB_FUNCTIONS, 0,0);

	#ifndef TTCALC_PORTABLE
	SendMessage(GetPrgRes()->GetTabWindow(tab_precision), WM_INIT_TAB_PRECISION, 0,0);
	#endif

	SendMessage(GetPrgRes()->GetTabWindow(tab_display),   WM_INIT_TAB_DISPLAY,   0,0);

	#ifdef TTCALC_CONVERT
	SendMessage(GetPrgRes()->GetTabWindow(tab_convert),   WM_INIT_TAB_CONVERT,   0,0);
	#endif

	TabWindowFunctions::SetLanguage(hTab);

	TabCtrl_SetCurSel(hTab, tab_standard);
	SetActiveTab( tab_standard );
}



BOOL WmInitDialog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
HWND hTab = GetDlgItem(hWnd,IDC_TAB);

	GetPrgRes()->SetMainWindow(hWnd);

	WmInitDialogCalculateMainWindowSize(hTab);
	WmInitDialogCreateAndInitTabs(hTab);

	HICON program_icon = LoadIcon(GetPrgRes()->GetInstance(),MAKEINTRESOURCE(IDI_ICON2));
	if( program_icon )
		SendMessage(hWnd,WM_SETICON,ICON_BIG,(LPARAM)program_icon);

	GetPrgRes()->SetAlwaysOnTop( GetPrgRes()->GetAlwaysOnTop() );
	GetPrgRes()->SetView       ( GetPrgRes()->GetView()        );

	SendDlgItemMessage(hWnd,IDC_INPUT_EDIT,EM_SETLIMITTEXT,GetPrgRes()->GetBufferSize()-1,0);
	SetMenuLanguage(hWnd);

#ifdef TTCALC_PORTABLE
	SetWindowText(hWnd, "TTCalc portable");
#endif

return true;
}

/*
	a function for WM_MOVE
*/
BOOL WmMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
RECT r;

	GetWindowRect(hWnd,&r);

	GetPrgRes()->SetXPos( r.left );
	GetPrgRes()->SetYPos( r.top );

return 0;
}


/*
	a function for WM_CLOSE
*/
BOOL WmClose(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->GetThreadController()->StopCalculatingAndExitThread();
	DestroyWindow(GetPrgRes()->GetMainWindow());
	PostQuitMessage(0);

return 0;
}


/*
	a function for WM_GETMINMAXINFO
*/
BOOL WmGetMinMaxInfo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
MINMAXINFO * lpmmi = (LPMINMAXINFO) lParam;

	lpmmi->ptMinTrackSize.x = GetPrgRes()->GetXSizeMin();

	if( GetPrgRes()->GetView() == ProgramResources::view_normal )
	{
		lpmmi->ptMinTrackSize.y = GetPrgRes()->GetYSizeNormal();
	}
	else
	{// kompaktowy
		lpmmi->ptMinTrackSize.y = GetPrgRes()->GetYSizeCompact();
		lpmmi->ptMaxTrackSize.y = GetPrgRes()->GetYSizeCompact();
	}

return 0;
}


/*!
	a function for WM_SIZING

	when user drags the border of the main window this method checks if
	the new boundary is near to the standard width and height, if it is
	the dragging box is attracted to the standard size
*/
BOOL WmSizing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
RECT * pr  = (RECT*) lParam;
int cx_min = GetPrgRes()->GetXSizeMin();
const int margin = 20;
	
	if( GetPrgRes()->GetView() == ProgramResources::view_normal )
	{
		int cy_min = GetPrgRes()->GetYSizeNormal();

		if( (pr->bottom - pr->top) < cy_min + margin )
		{
			if( (wParam == WMSZ_BOTTOM) || (wParam == WMSZ_BOTTOMRIGHT) || (wParam == WMSZ_BOTTOMLEFT) )
				pr->bottom = pr->top + cy_min;
			else
			if( (wParam == WMSZ_TOP) || (wParam == WMSZ_TOPRIGHT) || (wParam == WMSZ_TOPLEFT) )
				pr->top = pr->bottom - cy_min;
		}
	}
	
	if( (pr->right-pr->left) < cx_min + margin )
	{
		if( (wParam == WMSZ_RIGHT) || (wParam == WMSZ_BOTTOMRIGHT) || (wParam == WMSZ_TOPRIGHT) )
			pr->right = pr->left + cx_min;
		else
		if( (wParam == WMSZ_LEFT) || (wParam == WMSZ_BOTTOMLEFT) || (wParam == WMSZ_TOPLEFT) )
			pr->left  = pr->right - cx_min;
	}

return 1;
}


/*
	a function for WM_SIZE
*/
BOOL WmSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
int fwSizeType = int(wParam);
RECT r;
POINT p;

	if( fwSizeType == SIZE_MAXIMIZED )
		GetPrgRes()->SetMaximized(true);
	else
	if( fwSizeType == SIZE_RESTORED )
		GetPrgRes()->SetMaximized(false);

	// client area
	int cx = LOWORD(lParam);
	int cy = HIWORD(lParam);
	int resize_flags = SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER;

	HWND input_edit = GetDlgItem(hWnd, IDC_INPUT_EDIT);
	GetWindowRect(input_edit, &r);
	SetWindowPos(input_edit, 0, 0, 0, cx, r.bottom-r.top, resize_flags);

	HWND output_edit = GetDlgItem(hWnd, IDC_OUTPUT_EDIT);
	GetWindowRect(output_edit, &r);
	SetWindowPos(output_edit, 0, 0, 0, cx, r.bottom-r.top, resize_flags);

	HWND tab = GetDlgItem(hWnd, IDC_TAB);
	GetWindowRect(tab, &r);
	p.x = r.left;
	p.y = r.top;
	ScreenToClient(hWnd,&p);
	SetWindowPos(tab, 0, 0, 0, cx, cy-p.y, resize_flags);

	int borderx = GetSystemMetrics(SM_CXDLGFRAME) * 2;
	int bordery = GetSystemMetrics(SM_CYDLGFRAME) * 2;

	TabWindowFunctions::SetSizeOfVariablesList(tab, cx, cy-p.y, borderx, bordery);
	TabWindowFunctions::SetSizeOfFunctionsList(tab, cx, cy-p.y, borderx, bordery);
	TabWindowFunctions::SetSizeOfConvertingLists(tab, cx, cy-p.y, borderx, bordery);

	if( fwSizeType != SIZE_MINIMIZED && fwSizeType != SIZE_MAXIMIZED &&
		GetPrgRes()->GetView() != ProgramResources::view_compact )
	{
		GetWindowRect(hWnd,&r);
		GetPrgRes()->SetXSize( r.right - r.left );
		GetPrgRes()->SetYSize( r.bottom - r.top );
	}

return 0;
}

void WmInitMenuPopUpView(HMENU menu)
{
	if( GetPrgRes()->GetAlwaysOnTop() )
		CheckMenuItem(menu,IDM_ALWAYS_ON_TOP,MF_BYCOMMAND|MF_CHECKED);
	else
		CheckMenuItem(menu,IDM_ALWAYS_ON_TOP,MF_BYCOMMAND|MF_UNCHECKED);


	if( GetPrgRes()->GetView() == ProgramResources::view_normal )
	{
		CheckMenuItem(menu,IDM_NORMAL_VIEW,MF_BYCOMMAND|MF_CHECKED);
		CheckMenuItem(menu,IDM_COMPACT_VIEW,MF_BYCOMMAND|MF_UNCHECKED);
	}
	else
	{
		CheckMenuItem(menu,IDM_COMPACT_VIEW,MF_BYCOMMAND|MF_CHECKED);
		CheckMenuItem(menu,IDM_NORMAL_VIEW,MF_BYCOMMAND|MF_UNCHECKED);
	}


	if( GetPrgRes()->IsPadVisible() )
	{
		CheckMenuItem(menu,IDM_PAD,MF_BYCOMMAND|MF_CHECKED);
	}
	else
	{
		CheckMenuItem(menu,IDM_PAD,MF_BYCOMMAND|MF_UNCHECKED);
	}


	// languages

	CheckMenuItem(menu, IDM_LANGUAGE_ENGLISH, MF_BYCOMMAND|MF_UNCHECKED);
	CheckMenuItem(menu, IDM_LANGUAGE_POLISH,  MF_BYCOMMAND|MF_UNCHECKED);
	CheckMenuItem(menu, IDM_LANGUAGE_SPANISH, MF_BYCOMMAND|MF_UNCHECKED);
	CheckMenuItem(menu, IDM_LANGUAGE_DANISH,  MF_BYCOMMAND|MF_UNCHECKED);
	CheckMenuItem(menu, IDM_LANGUAGE_CHINESE, MF_BYCOMMAND|MF_UNCHECKED);
	CheckMenuItem(menu, IDM_LANGUAGE_RUSSIAN, MF_BYCOMMAND|MF_UNCHECKED);
	CheckMenuItem(menu, IDM_LANGUAGE_SWEDISH, MF_BYCOMMAND|MF_UNCHECKED);

	Languages::Country lang = GetPrgRes()->GetLanguages()->GetCurrentLanguage();

	if( lang == Languages::en )
		CheckMenuItem(menu, IDM_LANGUAGE_ENGLISH, MF_BYCOMMAND|MF_CHECKED);
	else
	if( lang == Languages::pl )
		CheckMenuItem(menu, IDM_LANGUAGE_POLISH,  MF_BYCOMMAND|MF_CHECKED);
	else
	if( lang == Languages::sp )
		CheckMenuItem(menu, IDM_LANGUAGE_SPANISH, MF_BYCOMMAND|MF_CHECKED);
	else
	if( lang == Languages::da )
		CheckMenuItem(menu, IDM_LANGUAGE_DANISH,  MF_BYCOMMAND|MF_CHECKED);
	else
	if( lang == Languages::chn )
		CheckMenuItem(menu, IDM_LANGUAGE_CHINESE, MF_BYCOMMAND|MF_CHECKED);
	else
	if( lang == Languages::ru )
		CheckMenuItem(menu, IDM_LANGUAGE_RUSSIAN, MF_BYCOMMAND|MF_CHECKED);
	else
	if( lang == Languages::swe )
		CheckMenuItem(menu, IDM_LANGUAGE_SWEDISH, MF_BYCOMMAND|MF_CHECKED);
	else
	if( lang == Languages::it )
		CheckMenuItem(menu, IDM_LANGUAGE_ITALIAN, MF_BYCOMMAND|MF_CHECKED);
	else
	if( lang == Languages::ger )
		CheckMenuItem(menu, IDM_LANGUAGE_GERMAN, MF_BYCOMMAND|MF_CHECKED);
}


void WmInitMenuPopUpEdit(HWND hWnd, HMENU menu)
{
bool edit          = false; // an edit control has the focus
bool edit_sel      = false; // there is something selected on the edit
bool edit_readonly = false; // the edit is read only

	HWND focus = GetFocus();
	edit = IsEditControl(focus);

	if( edit )
	{
		DWORD sel_start, sel_end;
		SendMessage(focus, EM_GETSEL, (WPARAM)&sel_start, (LPARAM)&sel_end);

		if( sel_start != sel_end )
			edit_sel = true;

		if( (GetWindowLong(focus, GWL_STYLE) & ES_READONLY) != 0 )
			edit_readonly = true;
	}


	// undo
	if( edit && !edit_readonly && SendMessage(focus, EM_CANUNDO, 0 ,0) )
		EnableMenuItem(menu,IDM_EDIT_UNDO,MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(menu,IDM_EDIT_UNDO,MF_BYCOMMAND | MF_GRAYED);

	// cut
	if( edit && edit_sel && !edit_readonly )
		EnableMenuItem(menu,IDM_EDIT_CUT,MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(menu,IDM_EDIT_CUT,MF_BYCOMMAND | MF_GRAYED);

	// paste
	if( edit && !edit_readonly )
		EnableMenuItem(menu,IDM_EDIT_PASTE,MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(menu,IDM_EDIT_PASTE,MF_BYCOMMAND | MF_GRAYED);

	// del
	if( edit && edit_sel && !edit_readonly )
		EnableMenuItem(menu,IDM_EDIT_DEL,MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(menu,IDM_EDIT_DEL,MF_BYCOMMAND | MF_GRAYED);

	// copy
	if( edit && edit_sel )
		EnableMenuItem(menu,IDM_EDIT_COPY,MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(menu,IDM_EDIT_COPY,MF_BYCOMMAND | MF_GRAYED);

	// copy result
	int input_size  = SendDlgItemMessage(hWnd, IDC_INPUT_EDIT,  EM_LINELENGTH, 0, 0);
	int output_size = SendDlgItemMessage(hWnd, IDC_OUTPUT_EDIT, EM_LINELENGTH, 0, 0);

	if( output_size != 0 )
		EnableMenuItem(menu,IDM_EDIT_COPY_RESULT,MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(menu,IDM_EDIT_COPY_RESULT,MF_BYCOMMAND | MF_GRAYED);

	// copy both
	if( input_size!=0 || output_size!=0 )
		EnableMenuItem(menu,IDM_EDIT_COPY_BOTH,MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(menu,IDM_EDIT_COPY_BOTH,MF_BYCOMMAND | MF_GRAYED);

	// paste formula
	if( IsClipboardFormatAvailable(CF_TEXT) )
		EnableMenuItem(menu,IDM_EDIT_PASTE_FORMULA,MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(menu,IDM_EDIT_PASTE_FORMULA,MF_BYCOMMAND | MF_GRAYED);

	// select all, bracket
	if( input_size > 0 )
	{
		EnableMenuItem(menu,IDM_EDIT_SELECT_ALL,MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(menu,IDM_EDIT_BRACKET,MF_BYCOMMAND | MF_ENABLED);
	}
	else
	{
		EnableMenuItem(menu,IDM_EDIT_SELECT_ALL,MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(menu,IDM_EDIT_BRACKET,MF_BYCOMMAND | MF_GRAYED);
	}

	// swap
	if( output_size > 0 )
		EnableMenuItem(menu, IDM_EDIT_SWAP, MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(menu, IDM_EDIT_SWAP, MF_BYCOMMAND | MF_GRAYED);
}


BOOL WmInitMenuPopUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( LOWORD(lParam) == IDM_VIEW_INDEX )
	{
		WmInitMenuPopUpView( (HMENU)wParam );

	return true;
	}
	else
	if( LOWORD(lParam) == IDM_EDIT_INDEX )
	{
		WmInitMenuPopUpEdit( hWnd, (HMENU)wParam );

	return true;
	}

return false;
}


/*
	a function for WM_NOTIFY
*/
BOOL WmNotify(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
NMHDR * pnmh = (LPNMHDR) lParam;
HWND hTab = GetDlgItem(hWnd,IDC_TAB);

	if( pnmh->hwndFrom==hTab && pnmh->code==TCN_SELCHANGE )
		SetActiveTab( TabCtrl_GetCurSel(hTab) );

return true;
}


/*
	a group of functions using when the WM_COMMAND message is sent
*/


BOOL WmCommand_AlwaysOnTop(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->SetAlwaysOnTop( !GetPrgRes()->GetAlwaysOnTop() );

return true;
}


BOOL WmCommand_NormalView(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->SetView(ProgramResources::view_normal);

return true;
}


BOOL WmCommand_CompactView(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->SetView(ProgramResources::view_compact);

return true;
}


BOOL WmCommand_Pad(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->ShowPad( !GetPrgRes()->IsPadVisible() );

return true;
}


BOOL WmCommand_HelpAbout(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DialogBox(GetPrgRes()->GetInstance(),MAKEINTRESOURCE(IDD_ABOUT_DIALOG),hWnd,AboutProc);

return true;
}


BOOL WmCommand_HelpCheckUpdate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ShowUpdateDialog(hWnd);

return true;
}

BOOL WmCommand_UpdateAvailable(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ShowUpdateDialogAfterCheck(hWnd);

return true;
}



BOOL WmCommand_NewWindow(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
STARTUPINFO si;
PROCESS_INFORMATION pi;

	char * buffer = new char[MAX_PATH+1];

	GetModuleFileName(GetPrgRes()->GetInstance(),buffer, MAX_PATH);

	memset(&si,0,sizeof(si));
	si.cb = sizeof(si);
	CreateProcess(buffer,const_cast<char*>(""),0,0,false,NORMAL_PRIORITY_CLASS,0,0,&si,&pi);

	delete [] buffer;

return true;
}



BOOL WmCommand_CloseProgram(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->GetThreadController()->StopCalculatingAndExitThread();
	DestroyWindow(GetPrgRes()->GetMainWindow());
	PostQuitMessage(0);

return true;
}



BOOL WmCommand_InputEditNotify(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( HIWORD(wParam) != EN_CHANGE )
		return false;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetDlgItemText(hWnd,IDC_INPUT_EDIT, (char*)GetPrgRes()->GetBuffer(), GetPrgRes()->GetBufferSize());
	GetPrgRes()->GetThreadController()->StartCalculating();

return true;
}



BOOL WmCommand_EditUndo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND focus = GetFocus();
	SendMessage(focus, EM_UNDO, 0, 0);

return true;
}


BOOL WmCommand_EditPasteFormula(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND input_edit = GetDlgItem(hWnd, IDC_INPUT_EDIT);
	SendMessage(input_edit, WM_PASTE, 0, 0);
	SetFocus(input_edit);

return true;
}


BOOL WmCommand_EditCopy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND focus = GetFocus();
	SendMessage(focus, WM_COPY, 0, 0);

return true;
}

BOOL WmCommand_EditCut(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND focus = GetFocus();
	SendMessage(focus, WM_CUT, 0, 0);

return true;
}

BOOL WmCommand_EditPaste(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND focus = GetFocus();
	SendMessage(focus, WM_PASTE, 0, 0);

return true;
}


BOOL WmCommand_EditDel(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND focus = GetFocus();

	if( IsEditControl(focus) )
		SendMessage(focus, EM_REPLACESEL, 1, (LPARAM)"");

return true;
}

BOOL WmCommand_EditBracket(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SendMessage(GetPrgRes()->GetMainWindow(), WM_COMMAND, IDC_BUTTON_BRACKETS_ALL, 0);

return true;
}


BOOL WmCommand_EditSwap(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char * buf = GetPrgRes()->GetBufferTemp();
	GetDlgItemText(hWnd, IDC_OUTPUT_EDIT, buf, GetPrgRes()->GetBufferSize());

	if( buf[0] == 0 )
		return true; // output edit is empty

	SetDlgItemText(hWnd, IDC_INPUT_EDIT, buf);

	// setting the caret at the end of the input edit
	int len = SendDlgItemMessage(hWnd, IDC_INPUT_EDIT, EM_LINELENGTH, 0, 0);
	SendDlgItemMessage(hWnd, IDC_INPUT_EDIT, EM_SETSEL, len, len);

return true;
}


BOOL WmCommand_EditCopyResult(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CopyResult();

return true;
}


BOOL WmCommand_EditCopyBoth(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	unsigned int in_len  = SendDlgItemMessage(hWnd, IDC_INPUT_EDIT,  EM_LINELENGTH, 0, 0);
	unsigned int out_len = SendDlgItemMessage(hWnd, IDC_OUTPUT_EDIT, EM_LINELENGTH, 0, 0);
	bool set_correct     = false;

	if( in_len >= GetPrgRes()->GetBufferSize() || out_len >= GetPrgRes()->GetBufferSize() )
		return true;

	if( !OpenClipboard(hWnd) )
		return true;

	EmptyClipboard();
	HGLOBAL global = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, in_len+out_len + 20);

	if( global )
	{
		char * text = (char*)GlobalLock(global);
		
		if( text )
		{	
			char * buf = GetPrgRes()->GetBufferTemp();

			*(WORD*)buf = in_len + 1;
			SendDlgItemMessage(hWnd, IDC_INPUT_EDIT, EM_GETLINE, 0, (LPARAM)buf);
			sprintf(text, "%s = ", buf);

			*(WORD*)buf = out_len + 1;
			SendDlgItemMessage(hWnd, IDC_OUTPUT_EDIT, EM_GETLINE, 0, (LPARAM)buf);
			std::strcat(text, buf);

			GlobalUnlock(global);
			set_correct = (bool)SetClipboardData(CF_TEXT, global);
		}
	}

	CloseClipboard();

	/*
		don't call GlobalFree(global) if SetClipboardData() returned true

		http://msdn.microsoft.com/en-us/library/ms649051%28VS.85%29.aspx
		If SetClipboardData succeeds, the system owns the object identified by the hMem parameter.
		The application may not write to or free the data once ownership has been transferred to
		the system, but it can lock and read from the data until the CloseClipboard function is called
	*/

	if( global && !set_correct )
		GlobalFree(global);

return true;
}


/*!
	an user has pressed 'ESC' key in the main window
	we're setting the entire input edit as being selected
*/
BOOL WmCommand_IDCANCEL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND iedit = GetDlgItem(hWnd, IDC_INPUT_EDIT);
	SendMessage(iedit, EM_SETSEL, 0, -1);
	SetFocus(iedit);

return true;
}


BOOL WmSetLastError(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
using TabWindowFunctions::last_code;

	last_code = (ttmath::ErrorCode)wParam;

	HWND button_err = GetDlgItem( GetPrgRes()->GetTabWindow(
										TabWindowFunctions::tab_standard),
										IDC_BUTTON_ERROR );

	if( last_code == ttmath::err_ok || last_code == ttmath::err_nothing_has_read )
		EnableWindow(button_err, false);
	else
		EnableWindow(button_err, true);


	if( last_code == ttmath::err_unknown_variable				||
		last_code == ttmath::err_division_by_zero				|| 
		last_code == ttmath::err_overflow						||
		last_code == ttmath::err_unknown_function				||
		last_code == ttmath::err_improper_argument )
		TabWindowFunctions::PrintErrorCode();

return true;
}


void SetOutputEditLanguage(HWND hWnd)
{
	if( TabWindowFunctions::last_code != ttmath::err_ok )
	{
		if( SendDlgItemMessage(hWnd, IDC_OUTPUT_EDIT, WM_GETTEXTLENGTH, 0, 0) > 0 )
		{
			// there should be an error string on the output edit
			// and we change it into a new language
			TabWindowFunctions::PrintErrorCode();
		}
	}
}


BOOL WmCommand_LanguageEnglish(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->GetLanguages()->SetCurrentLanguage(Languages::en);

	SetMenuLanguage(hWnd);
	SetOutputEditLanguage(hWnd);
	TabWindowFunctions::SetLanguage( GetDlgItem(hWnd, IDC_TAB) );
	SetPadLanguage();

return true;
}


BOOL WmCommand_LanguagePolish(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->GetLanguages()->SetCurrentLanguage(Languages::pl);

	SetMenuLanguage(hWnd);
	SetOutputEditLanguage(hWnd);
	TabWindowFunctions::SetLanguage( GetDlgItem(hWnd, IDC_TAB) );
	SetPadLanguage();

return true;
}


BOOL WmCommand_LanguageSpanish(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->GetLanguages()->SetCurrentLanguage(Languages::sp);

	SetMenuLanguage(hWnd);
	SetOutputEditLanguage(hWnd);
	TabWindowFunctions::SetLanguage( GetDlgItem(hWnd, IDC_TAB) );
	SetPadLanguage();

return true;
}

BOOL WmCommand_LanguageDanish(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->GetLanguages()->SetCurrentLanguage(Languages::da);

	SetMenuLanguage(hWnd);
	SetOutputEditLanguage(hWnd);
	TabWindowFunctions::SetLanguage( GetDlgItem(hWnd, IDC_TAB) );
	SetPadLanguage();

return true;
}

BOOL WmCommand_LanguageChinese(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->GetLanguages()->SetCurrentLanguage(Languages::chn);

	SetMenuLanguage(hWnd);
	SetOutputEditLanguage(hWnd);
	TabWindowFunctions::SetLanguage( GetDlgItem(hWnd, IDC_TAB) );
	SetPadLanguage();

return true;
}

BOOL WmCommand_LanguageRussian(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->GetLanguages()->SetCurrentLanguage(Languages::ru);

	SetMenuLanguage(hWnd);
	SetOutputEditLanguage(hWnd);
	TabWindowFunctions::SetLanguage( GetDlgItem(hWnd, IDC_TAB) );
	SetPadLanguage();

return true;
}

BOOL WmCommand_LanguageSwedish(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->GetLanguages()->SetCurrentLanguage(Languages::swe);

	SetMenuLanguage(hWnd);
	SetOutputEditLanguage(hWnd);
	TabWindowFunctions::SetLanguage( GetDlgItem(hWnd, IDC_TAB) );
	SetPadLanguage();

return true;
}

BOOL WmCommand_LanguageItalian(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->GetLanguages()->SetCurrentLanguage(Languages::it);

	SetMenuLanguage(hWnd);
	SetOutputEditLanguage(hWnd);
	TabWindowFunctions::SetLanguage( GetDlgItem(hWnd, IDC_TAB) );
	SetPadLanguage();

return true;
}


BOOL WmCommand_LanguageGerman(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->GetLanguages()->SetCurrentLanguage(Languages::ger);

	SetMenuLanguage(hWnd);
	SetOutputEditLanguage(hWnd);
	TabWindowFunctions::SetLanguage( GetDlgItem(hWnd, IDC_TAB) );
	SetPadLanguage();

return true;
}

BOOL WmHelp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
SHELLEXECUTEINFO exec;

	exec.cbSize = sizeof(exec);
	exec.fMask  = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
    exec.hwnd   = 0; 
    exec.lpVerb = "open"; 
    exec.lpFile = GetPrgRes()->GetHelpFile().c_str(); 
    exec.lpParameters = 0; 
    exec.lpDirectory  = ""; 
    exec.nShow  = SW_SHOWNORMAL; 

	if( !ShellExecuteEx(&exec) )
	{
		MessageBox(hWnd,
			GetPrgRes()->GetLanguages()->GuiMessage(Languages::cant_find_help),
			GetPrgRes()->GetLanguages()->GuiMessage(Languages::message_box_caption),
			MB_ICONERROR);
	}

return true;
}


BOOL WmProjectPage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
SHELLEXECUTEINFO exec;

	exec.cbSize = sizeof(exec);
	exec.fMask  = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
    exec.hwnd   = 0; 
    exec.lpVerb = "open"; 
    //exec.lpFile = "http://sourceforge.net/projects/ttcalc"; 
	exec.lpFile = "http://ttcalc.sourceforge.net"; 
    exec.lpParameters = 0; 
    exec.lpDirectory  = ""; 
    exec.nShow  = SW_SHOWNORMAL; 

	if( !ShellExecuteEx(&exec) )
	{
		MessageBox(hWnd,
			GetPrgRes()->GetLanguages()->GuiMessage(Languages::cant_open_project_page),
			GetPrgRes()->GetLanguages()->GuiMessage(Languages::message_box_caption),
			MB_ICONERROR);
	}

return true;
}


BOOL WmUpdateExists(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
MENUITEMINFO mi;

	mi.cbSize = sizeof(mi);
	mi.fMask  = MIIM_TYPE | MIIM_ID;
	mi.fType  = MFT_STRING;
	mi.dwTypeData = const_cast<char*>(GetPrgRes()->GetLanguages()->GuiMessage(Languages::menu_update_available));
	mi.cch    = 0;
	mi.wID    = IDM_UPDATE_AVAILABLE;
	
	HMENU menu = GetMenu(hWnd);

	if( !menu )
		return true;

	InsertMenuItem(menu, IDM_UPDATE_AVAILABLE_INDEX, true, &mi);
	GetPrgRes()->SetUpdateExists(true); // used for the dynamic menu (for changing languages)
	DrawMenuBar(hWnd);

return true;
}


void CreateCommandMessagesTable(Messages<BOOL> & cmessages)
{
	cmessages.Associate(IDM_ALWAYS_ON_TOP, WmCommand_AlwaysOnTop);
	cmessages.Associate(IDM_NORMAL_VIEW, WmCommand_NormalView);
	cmessages.Associate(IDM_COMPACT_VIEW, WmCommand_CompactView);
	cmessages.Associate(IDM_PAD, WmCommand_Pad);
	cmessages.Associate(IDM_HELP_ABOUT, WmCommand_HelpAbout);
	cmessages.Associate(IDM_HELP_CHECK_UPDATE, WmCommand_HelpCheckUpdate);
	cmessages.Associate(IDM_UPDATE_AVAILABLE, WmCommand_UpdateAvailable);
	cmessages.Associate(IDM_NEW_WINDOW, WmCommand_NewWindow);
	cmessages.Associate(IDM_CLOSE_PROGRAM, WmCommand_CloseProgram);
	cmessages.Associate(IDC_INPUT_EDIT, WmCommand_InputEditNotify);
	cmessages.Associate(IDM_EDIT_UNDO, WmCommand_EditUndo);
	cmessages.Associate(IDM_EDIT_PASTE_FORMULA, WmCommand_EditPasteFormula);
	cmessages.Associate(IDM_EDIT_COPY_RESULT, WmCommand_EditCopyResult);
	cmessages.Associate(IDM_EDIT_COPY_BOTH, WmCommand_EditCopyBoth);
	cmessages.Associate(IDM_EDIT_COPY, WmCommand_EditCopy);
	cmessages.Associate(IDM_EDIT_CUT, WmCommand_EditCut);
	cmessages.Associate(IDM_EDIT_PASTE, WmCommand_EditPaste);
	cmessages.Associate(IDM_EDIT_DEL, WmCommand_EditDel);
	cmessages.Associate(IDM_EDIT_BRACKET, WmCommand_EditBracket);
	cmessages.Associate(IDM_EDIT_SWAP, WmCommand_EditSwap);
	cmessages.Associate(IDCANCEL, WmCommand_IDCANCEL);
	cmessages.Associate(IDM_EDIT_SELECT_ALL, WmCommand_IDCANCEL);
	cmessages.Associate(IDM_LANGUAGE_ENGLISH, WmCommand_LanguageEnglish);
	cmessages.Associate(IDM_LANGUAGE_POLISH, WmCommand_LanguagePolish);
	cmessages.Associate(IDM_LANGUAGE_SPANISH, WmCommand_LanguageSpanish);
	cmessages.Associate(IDM_LANGUAGE_DANISH, WmCommand_LanguageDanish);
	cmessages.Associate(IDM_LANGUAGE_CHINESE, WmCommand_LanguageChinese);
	cmessages.Associate(IDM_LANGUAGE_RUSSIAN, WmCommand_LanguageRussian);
	cmessages.Associate(IDM_LANGUAGE_SWEDISH, WmCommand_LanguageSwedish);
	cmessages.Associate(IDM_LANGUAGE_ITALIAN, WmCommand_LanguageItalian);
	cmessages.Associate(IDM_LANGUAGE_GERMAN,  WmCommand_LanguageGerman);
	cmessages.Associate(IDM_HELP_HELP, WmHelp);
	cmessages.Associate(IDM_HELP_PROJECT_PAGE, WmProjectPage);
}


BOOL WmCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static Messages<BOOL> command_messages;
	
	if( command_messages.Empty() )
		// initiation
		CreateCommandMessagesTable(command_messages);

	if( command_messages.IsMessage(LOWORD(wParam)) )
		return command_messages.Call(LOWORD(wParam), hWnd, message, wParam, lParam);
	else
		return TabWindowFunctions::WmTabCommand(hWnd, message, wParam, lParam);
}



void CreateMainMessagesTable(Messages<BOOL> & messages)
{
	messages.Associate(WM_INITDIALOG, WmInitDialog);
	messages.Associate(WM_MOVE, WmMove);
	messages.Associate(WM_CLOSE, WmClose);
	messages.Associate(WM_GETMINMAXINFO, WmGetMinMaxInfo);
	messages.Associate(WM_SIZE, WmSize);
	messages.Associate(WM_INITMENUPOPUP, WmInitMenuPopUp);
	messages.Associate(WM_COMMAND, WmCommand);
	messages.Associate(WM_NOTIFY, WmNotify);
	messages.Associate(WM_SIZING, WmSizing);
	messages.Associate(WM_SET_LAST_ERROR, WmSetLastError);
	messages.Associate(WM_HELP, WmHelp);
	messages.Associate(WM_UPDATE_EXISTS, WmUpdateExists);
}


/*!
	this method prepares a text which is printed on the about dialog box
*/
void CreateAboutText(char * buffer)
{
char compiler[50];

	#ifdef __GNUC__

		#ifdef __VERSION__
			sprintf(compiler,"GCC %s",  __VERSION__);
		#else
			sprintf(
				compiler,
				"GCC %d.%d.%d",
				__GNUC__,
				__GNUC_MINOR__,
				__GNUC_PATCHLEVEL__
				);
		#endif

	#elif _MSC_VER

		char msvc_ver[20];
		int i;
		sprintf(msvc_ver, "%d", _MSC_VER);
		/*
			from msdn:
			_MSC_VER reports the major and minor versions of the compiler.
			For example, 1310 for Microsoft Visual C++ .NET 2003. 1310 represents
			version 13 and a 1.0 point release. The Visual C++ 2005 compiler
			version is 1400.

			If there is, for example, version 1400 we rather want to show it as 14.00,
			so we put a simple dot on the third position
		*/
		for(i=0; msvc_ver[i] ; ++i);
		for( ; i>=2 ; --i)
			msvc_ver[i+1] = msvc_ver[i];

		msvc_ver[2] = '.';

		sprintf(
			compiler,
			"Microsoft Visual C++ %s",
			msvc_ver
			);

	#else

		sprintf(compiler, "unknown");

	#endif


	sprintf(
		buffer,
		GetPrgRes()->GetLanguages()->GuiMessage(Languages::about_text),
		TTCALC_MAJOR_VER, TTCALC_MINOR_VER, TTCALC_REVISION_VER, 
#ifndef TTCALC_PORTABLE
		"",
#else
		GetPrgRes()->GetLanguages()->GuiMessage(Languages::about_text_portable_version),
#endif
		(TTCALC_PRERELEASE_VER!=0)? " prerelease" : "",
		TTMATH_MAJOR_VER, TTMATH_MINOR_VER, TTMATH_REVISION_VER,
		(TTMATH_PRERELEASE_VER!=0)? " prerelease" : "",
		TTMathBigMax::LibTypeStr(),
		compiler,
#ifndef TTCALC_PORTABLE
		""
#else
		GetPrgRes()->GetLanguages()->GuiMessage(Languages::about_text_exe_packer)
#endif
		);

}

/*!
	it displays the about-window
*/
BOOL CALLBACK AboutProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
char * about_buffer;
Languages * lang;

	switch(message)
	{
	case WM_INITDIALOG:
		lang = GetPrgRes()->GetLanguages();
		SetWindowText(hWnd, lang->GuiMessage(Languages::about_box_title) );
		SetWindowText(GetDlgItem(hWnd, IDOK), lang->GuiMessage(Languages::about_box_button_close) );

		about_buffer = new char[2000];
		CreateAboutText(about_buffer);
		SetDlgItemText(hWnd, IDC_ABOUT_TEXT, about_buffer);
		delete [] about_buffer;

		return true;


	case WM_COMMAND:
		if(LOWORD(wParam)==IDOK || LOWORD(wParam)==IDCANCEL)
		{
			EndDialog(hWnd,0);
			return true;
		}

		break;
	}

return false;
}

} // namespace MainWindowFunctions

