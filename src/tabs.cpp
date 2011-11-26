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
int tab_standard;
int tab_variables;
int tab_functions;
int tab_precision;
int tab_display;
int tab_convert;

ttmath::ErrorCode last_code = ttmath::err_ok;


void PrintErrorCode()
{
	SetDlgItemText(GetPrgRes()->GetMainWindow(),IDC_OUTPUT_EDIT,
		GetPrgRes()->GetLanguages()->ErrorMessage(last_code) );
}




int CalcBracketsPrev(const char * buf, int start)
{
int last;
int brackets;

	last = start;
	--start;

	while( true )
	{
		for( ; start>=0 && buf[start]!=';' ; --start )
			if( buf[start] != ' ' )
				last = start;

		if( start <= 0 )
			return last;

		--start; // skipping a semicolon
		brackets = 0;

		for( ; start>=0 ; --start )
		{
			if( buf[start] == '(' )
				++brackets;
			else
			if( buf[start] == ')' )
				--brackets;

			if( brackets > 0 )
			{
				last = start;
				break;
			}
		}
	}
}





int CalcBracketsNext(const char * buf, int size, int start)
{
int last;
int brackets;

	if( start >= size )
		return size;

	last = start;

	while( true )
	{
		for( ; start<size && buf[start]!=';' ; ++start )
			if( buf[start] != ' ' )
				last = start;

		if( start >= size )
		{
			if( buf[last] == ';' || buf[last] == ' ' )
				return last;
			else
				return last+1;
		}

		++start; // skipping a semicolon
		brackets = 0;

		for( ; start<size ; ++start )
		{
			if( buf[start] == '(' )
				++brackets;
			else
			if( buf[start] == ')' )
				--brackets;

			if( brackets < 0 )
			{
				last = start;
				break;
			}
		}
	}
}


bool IsWhite(int c)
{
	return c==' ' || c=='\t';
}


bool IsOpeningOperand(int c)
{
	return c=='-' || c=='+' || c=='*' ||c=='/' || c=='%' ||c=='^' || c=='(';
}


bool IsOpeningOperandBefore(const char * buf, int end)
{
	while( end > 0 && IsWhite(buf[end-1]) )
		end -= 1;

	if( end == 0 )
		return false;

return IsOpeningOperand(buf[end-1]);
}



void CalcBracketsPos(HWND input, int & start, int & end)
{
DWORD sel_start, sel_end;

	SendMessage(input, EM_GETSEL, (WPARAM)&sel_start, (LPARAM)&sel_end);

	start = sel_start;
	end   = sel_end;

	if( start != end )
		return;

	char * buf = GetPrgRes()->GetBufferTemp();
	int len = SendMessage(input, WM_GETTEXT, GetPrgRes()->GetBufferSize(), (LPARAM)buf);

	if( len > 0 && !IsOpeningOperandBefore(buf, end) )
	{
		start = CalcBracketsPrev(buf, sel_start);
		end   = CalcBracketsNext(buf, len, sel_start);
	}
}



void InsertText(const char * first, const char * last, int cursor_back = 0)
{
	HWND input = GetDlgItem(GetPrgRes()->GetMainWindow(), IDC_INPUT_EDIT);

	int start, end;
	CalcBracketsPos(input, start, end);

	size_t len_first = strlen(first);

	SendMessage(input, EM_SETSEL, start, start);
	SendMessage(input, EM_REPLACESEL, 1, LPARAM(first));
	
	SendMessage(input, EM_SETSEL, end+len_first, end+len_first);
	SendMessage(input, EM_REPLACESEL, 1, LPARAM(last));

	if( start == end )
	{
		// was empty string - we're setting the cursor inside the texts
		SendMessage(input, EM_SETSEL, end+len_first, end+len_first);
	}
	else
	{
		size_t len_last = strlen(last);
		SendMessage(input, EM_SETSEL, end+len_first+len_last-cursor_back, end+len_first+len_last-cursor_back);
	}

	SetFocus(input);
}




/*
	this functions inserts a given string into the first edit control
	and then moves the caret
*/
void InsertText(const char * text, int move = 0)
{
HWND input_edit = GetDlgItem( GetPrgRes()->GetMainWindow(), IDC_INPUT_EDIT );

	SetFocus(input_edit);
	
	SendMessage(input_edit, EM_REPLACESEL, true, (LPARAM)text);

	if( move != 0 )
	{
		DWORD l1,l2;

		SendMessage(input_edit, EM_GETSEL, (WPARAM)&l1, (LPARAM)&l2 );
		SendMessage(input_edit, EM_SETSEL, l1+move, l2+move );
	}
}


/*
	a function for WM_COMMAND and LOWORD(wParam)
	from IDC_BUTTON_PRESS_0 to IDC_BUTTON_PRESS_9
*/
BOOL WmTabCommand_Press_0_9(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SetFocus(GetDlgItem(GetPrgRes()->GetMainWindow(),IDC_INPUT_EDIT));
	SendDlgItemMessage(GetPrgRes()->GetMainWindow(),IDC_INPUT_EDIT,WM_CHAR,LOWORD(wParam)-IDC_BUTTON_PRESS_0+'0',0x20001);

return true;
}


BOOL WmTabCommand_Press_e(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("e",0);

return true;
}


BOOL WmTabCommand_Press_comma(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText(".",0);

return true;
}



BOOL WmTabCommand_Press_pi(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("pi",0);

return true;
}


BOOL WmTabCommand_Press_sin(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("sin(", ")");

return true;
}


BOOL WmTabCommand_Press_cos(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("cos(", ")");

return true;
}


BOOL WmTabCommand_Press_tan(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("tan(", ")");

return true;
}


BOOL WmTabCommand_Press_cot(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("cot(", ")");

return true;
}


BOOL WmTabCommand_Press_ln(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("ln(", ")");

return true;
}


BOOL WmTabCommand_Press_log(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("log(", " ; )", 1);

return true;
}


BOOL WmTabCommand_Press_abs(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("abs(", ")");

return true;
}


BOOL WmTabCommand_Press_factorial(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("factorial(", ")");

return true;
}


BOOL WmTabCommand_Press_div(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("/", 0);

return true;
}

BOOL WmTabCommand_Press_mul(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("*",0);

return true;
}

BOOL WmTabCommand_Press_sub(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("-",0);

return true;
}

BOOL WmTabCommand_Press_add(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("+",0);

return true;
}


BOOL WmTabCommand_Press_first_bracket(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("(",0);

return true;
}


BOOL WmTabCommand_Press_last_bracket(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText(")",0);

return true;
}


BOOL WmTabCommand_Press_clear(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//SetDlgItemText(GetPrgRes()->GetMainWindow(), IDC_INPUT_EDIT, "");
	
	//HWND input_edit = GetDlgItem( GetPrgRes()->GetMainWindow(), IDC_INPUT_EDIT );
	//SetFocus(input_edit);

return true;
}


BOOL WmTabCommand_Press_error(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( last_code != ttmath::err_ok )
	{
		PrintErrorCode();
		HWND input_edit = GetDlgItem( GetPrgRes()->GetMainWindow(), IDC_INPUT_EDIT );
		SetFocus(input_edit);
	}


return true;
}


BOOL WmTabCommand_Press_asin(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("asin(", ")");

return true;
}

BOOL WmTabCommand_Press_acos(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("acos(", ")");

return true;
}

BOOL WmTabCommand_Press_atan(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("atan(", ")");

return true;
}

BOOL WmTabCommand_Press_acot(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("acot(", ")");

return true;
}

BOOL WmTabCommand_Press_sgn(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("sgn(", ")");

return true;
}

BOOL WmTabCommand_Press_mod(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("mod(", " ; )", 1);

return true;
}


BOOL WmTabCommand_Press_power(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("^",0);

return true;
}


BOOL WmTabCommand_Press_int(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("int(", ")");

return true;
}


BOOL WmTabCommand_Press_round(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("round(", ")");

return true;
}


BOOL WmTabCommand_Press_exp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("exp(", ")");

return true;
}


BOOL WmTabCommand_IDOK(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
HWND main = GetPrgRes()->GetMainWindow();
HWND tab  = GetDlgItem(main, IDC_TAB);
int sel   = TabCtrl_GetCurSel(tab);


	if( sel == tab_variables )
	{
		if( GetFocus() == GetDlgItem(GetPrgRes()->GetTabWindow(tab_variables), IDC_VARIABLES_LIST ) )
			Variables::WmTabCommand_EditVariable(hWnd, WM_COMMAND, IDC_BUTTON_EDIT_VARIABLE, 0);
	}
	else
	if( sel == tab_functions )
	{
		if( GetFocus() == GetDlgItem(GetPrgRes()->GetTabWindow(tab_functions), IDC_FUNCTIONS_LIST ) )
			Functions::WmTabCommand_EditFunction(hWnd, WM_COMMAND, IDC_BUTTON_EDIT_FUNCTION, 0);
	}

return true;
}

BOOL WmTabCommand_IDCANCEL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SendMessage(GetPrgRes()->GetMainWindow(), WM_COMMAND, IDCANCEL, 0);

return true;
}


// pos is the 'grouping' position (0 means none)
void SetDisablingGroupingDigits(HWND hWnd, int pos = -1)
{
	if( pos < 0 )
		pos = SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_GROUPING, CB_GETCURSEL, 0, 0);

	if( pos <= 0 )
	{
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_DISPLAY_GROUPING_DIGITS), false);
	}
	else
	{
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_DISPLAY_GROUPING_DIGITS), true);
	}
}


void SetLanguageTabStandard(HWND hWnd)
{
	SetDlgItemText( hWnd,IDC_BUTTON_CLEAR, GetPrgRes()->GetLanguages()->GuiMessage(Languages::button_clear) );
}

void SetLanguageTabVariables(HWND hWnd)
{
	SetDlgItemText( hWnd,IDC_BUTTON_ADD_VARIABLE, GetPrgRes()->GetLanguages()->GuiMessage(Languages::button_add) );
	SetDlgItemText( hWnd,IDC_BUTTON_EDIT_VARIABLE, GetPrgRes()->GetLanguages()->GuiMessage(Languages::button_edit) );
	SetDlgItemText( hWnd,IDC_BUTTON_DELETE_VARIABLE, GetPrgRes()->GetLanguages()->GuiMessage(Languages::button_delete) );

	HWND list = GetDlgItem(hWnd, IDC_VARIABLES_LIST);
	LVCOLUMN column;

	column.mask    = LVCF_TEXT;
	column.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::list_variables_header_1) );
	ListView_SetColumn(list,0,&column);

	column.mask    = LVCF_TEXT;
	column.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::list_variables_header_2) );
	ListView_SetColumn(list,1,&column);
}


void SetLanguageTabFunctions(HWND hWnd)
{
	SetDlgItemText( hWnd,IDC_BUTTON_ADD_FUNCTION, GetPrgRes()->GetLanguages()->GuiMessage(Languages::button_add) );
	SetDlgItemText( hWnd,IDC_BUTTON_EDIT_FUNCTION, GetPrgRes()->GetLanguages()->GuiMessage(Languages::button_edit) );
	SetDlgItemText( hWnd,IDC_BUTTON_DELETE_FUNCTION, GetPrgRes()->GetLanguages()->GuiMessage(Languages::button_delete) );

	HWND list = GetDlgItem(hWnd, IDC_FUNCTIONS_LIST);
	LVCOLUMN column;

	column.mask    = LVCF_TEXT;
	column.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::list_functions_header_1) );
	ListView_SetColumn(list,0,&column);

	column.mask    = LVCF_TEXT;
	column.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::list_functions_header_2) );
	ListView_SetColumn(list,1,&column);

	column.mask    = LVCF_TEXT;
	column.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::list_functions_header_3) );
	ListView_SetColumn(list,2,&column);
}


void SetLanguageTabPrecision(HWND hWnd)
{
	#ifndef TTCALC_PORTABLE

	SetDlgItemText(hWnd, IDC_RADIO_PRECISION_1, GetPrgRes()->GetLanguages()->GuiMessage(Languages::radio_precision_1) );
	SetDlgItemText(hWnd, IDC_RADIO_PRECISION_2, GetPrgRes()->GetLanguages()->GuiMessage(Languages::radio_precision_2) );
	SetDlgItemText(hWnd, IDC_RADIO_PRECISION_3, GetPrgRes()->GetLanguages()->GuiMessage(Languages::radio_precision_3) );

	SetDlgItemText(hWnd, IDC_LABEL_PRECISION_1_INFO, GetPrgRes()->GetLanguages()->GuiMessage(Languages::precision_1_info) );
	SetDlgItemText(hWnd, IDC_LABEL_PRECISION_2_INFO, GetPrgRes()->GetLanguages()->GuiMessage(Languages::precision_2_info) );
	SetDlgItemText(hWnd, IDC_LABEL_PRECISION_3_INFO, GetPrgRes()->GetLanguages()->GuiMessage(Languages::precision_3_info) );

	#endif
}


void SetLanguageTabDisplayComboRounding(HWND hWnd)
{
char buffer[50];
int i;
const int max_r = 9;

	Languages * pl = GetPrgRes()->GetLanguages();

	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_ROUNDING, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_ROUNDING, CB_ADDSTRING, 0, (LPARAM)pl->GuiMessage(Languages::combo_rounding_none) );
	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_ROUNDING, CB_ADDSTRING, 0, (LPARAM)pl->GuiMessage(Languages::combo_rounding_integer) );

	for(i=1 ; i<=max_r ; ++i)
	{
		sprintf(buffer,"%s %d %s",
			pl->GuiMessage(Languages::combo_rounding_to_number),
			i,
			pl->GuiMessage(Languages::combo_rounding_after_comma) );

		SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_ROUNDING, CB_ADDSTRING, 0, (LPARAM)buffer);
	}

	int r = GetPrgRes()->GetDisplayRounding();

	if( r > max_r )
		r = max_r;

	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_ROUNDING, CB_SETCURSEL, r+1, 0);
}


void SetLanguageTabDisplayComboGrouping(HWND hWnd)
{
char buffer[50];
int i;
const int max_r = 9;

	Languages * pl = GetPrgRes()->GetLanguages();

	// grouping
	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_GROUPING, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_GROUPING, CB_ADDSTRING, 0, (LPARAM)pl->GuiMessage(Languages::display_grouping_none));
	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_GROUPING, CB_ADDSTRING, 0, (LPARAM)pl->GuiMessage(Languages::display_grouping_space));
	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_GROUPING, CB_ADDSTRING, 0, (LPARAM)" `");
	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_GROUPING, CB_ADDSTRING, 0, (LPARAM)" '");
	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_GROUPING, CB_ADDSTRING, 0, (LPARAM)" .");
	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_GROUPING, CB_ADDSTRING, 0, (LPARAM)" ,");
	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_GROUPING, CB_SETCURSEL, GetPrgRes()->GetGrouping(), 0);

	// grouping digits
	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_GROUPING_DIGITS, CB_RESETCONTENT, 0, 0);

	for(i=1 ; i<=max_r ; ++i)
	{
		sprintf(buffer,"%d %s",	i, pl->GuiMessage(Languages::display_grouping_digits) );
		SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_GROUPING_DIGITS, CB_ADDSTRING, 0, (LPARAM)buffer);
	}

	int r = GetPrgRes()->GetGroupingDigits();

	if( r > max_r )
		r = max_r;

	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_GROUPING_DIGITS, CB_SETCURSEL, r-1, 0);
}


void SetLanguageTabDisplay(HWND hWnd)
{
	Languages * pl = GetPrgRes()->GetLanguages();

	SetLanguageTabDisplayComboRounding(hWnd);
	SetLanguageTabDisplayComboGrouping(hWnd);
	SetDisablingGroupingDigits(hWnd, GetPrgRes()->GetGrouping());

	SetDlgItemText(hWnd, IDC_LABEL_DISPLAY_INPUT, pl->GuiMessage(Languages::display_input));
	SetDlgItemText(hWnd, IDC_LABEL_DISPLAY_OUTPUT, pl->GuiMessage(Languages::display_output));
	SetDlgItemText(hWnd, IDC_LABEL_DISPLAY_ROUNDING, pl->GuiMessage(Languages::display_rounding));
	SetDlgItemText(hWnd, IDC_LABEL_DISPLAY_GROUPING, pl->GuiMessage(Languages::display_grouping));
	SetDlgItemText(hWnd, IDC_LABEL_INPUT_DECIMAL_POINT, pl->GuiMessage(Languages::display_input_decimal_point));
	SetDlgItemText(hWnd, IDC_LABEL_OUTPUT_DECIMAL_POINT, pl->GuiMessage(Languages::display_output_decimal_point));
	SetDlgItemText(hWnd, IDC_LABEL_PARAM_SEPARATE, pl->GuiMessage(Languages::display_param_sep));

	SetDlgItemText(hWnd, IDC_RADIO_DISPLAY_ALWAYS_SCIENTIFIC, pl->GuiMessage(Languages::display_always_scientific));
	SetDlgItemText(hWnd, IDC_RADIO_DISPLAY_NOT_ALWAYS_SCIENTIFIC, pl->GuiMessage(Languages::display_not_always_scientific));

	SetDlgItemText(hWnd, IDC_LABEL_DISPLAY_SCIENTIFIC, pl->GuiMessage(Languages::display_as_scientific));
	SetDlgItemText(hWnd, IDC_CHECK_REMOVE_ZEROES, pl->GuiMessage(Languages::check_remove_zeroes));
	SetDlgItemText(hWnd, IDC_LABEL_DISPLAY_DEG_RAD_GRAD, pl->GuiMessage(Languages::display_deg_rad_grad));

	SendDlgItemMessage(hWnd, IDC_COMBO_INPUT_DECIMAL_POINT, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hWnd, IDC_COMBO_INPUT_DECIMAL_POINT, CB_ADDSTRING, 0, (LPARAM)pl->GuiMessage(Languages::display_input_decimal_point_item1));
	SendDlgItemMessage(hWnd, IDC_COMBO_INPUT_DECIMAL_POINT, CB_ADDSTRING, 0, (LPARAM)" .");
	SendDlgItemMessage(hWnd, IDC_COMBO_INPUT_DECIMAL_POINT, CB_ADDSTRING, 0, (LPARAM)" ,");
	SendDlgItemMessage(hWnd, IDC_COMBO_INPUT_DECIMAL_POINT, CB_SETCURSEL, GetPrgRes()->GetInputDecimalPoint(), 0);

	SendDlgItemMessage(hWnd, IDC_COMBO_PARAM_SEPARATE, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hWnd, IDC_COMBO_PARAM_SEPARATE, CB_ADDSTRING, 0, (LPARAM)" ;");
	SendDlgItemMessage(hWnd, IDC_COMBO_PARAM_SEPARATE, CB_ADDSTRING, 0, (LPARAM)pl->GuiMessage(Languages::display_param_sep_item2));
	SendDlgItemMessage(hWnd, IDC_COMBO_PARAM_SEPARATE, CB_ADDSTRING, 0, (LPARAM)pl->GuiMessage(Languages::display_param_sep_item3));
	SendDlgItemMessage(hWnd, IDC_COMBO_PARAM_SEPARATE, CB_SETCURSEL, GetPrgRes()->GetParamSep(), 0);

}


/*!

-1 if there's no such item
*/
int FindItem(HWND list, int i)
{
LVFINDINFO info;

	info.flags  = LVFI_PARAM;
	info.lParam = i;

return ListView_FindItem(list, -1, &info);
}


void TabConvertFillUpUnitList(HWND list, int conv_type)
{
	Convert * pconv = GetPrgRes()->GetConvert();
	unsigned int len = pconv->GetUnitSize();
	unsigned int i, index;

	ListView_DeleteAllItems(list);

	LVITEM item;
	item.mask = LVIF_PARAM | LVIF_TEXT;
	item.iSubItem = 0;

	for( i=index=0 ; i<len ; ++i )
	{
		if( pconv->GetUnitType(i) == conv_type )
		{
			item.lParam = i;
			item.iItem = index++;
			int country = static_cast<unsigned int>(GetPrgRes()->GetLanguages()->GetCurrentLanguage());
			item.pszText = const_cast<char*>(pconv->GetUnitName(country, i));

			ListView_InsertItem(list, &item);
		}
	}
}


void TabConvertFillUpUnitLists(HWND hWnd)
{
HWND list2b = GetDlgItem(hWnd, IDC_LIST_UNIT_INPUT2);
HWND list3b = GetDlgItem(hWnd, IDC_LIST_UNIT_OUTPUT2);
int input, output;

	int convtype = GetPrgRes()->GetConvert()->GetCurrentType();

	TabConvertFillUpUnitList(list2b, convtype);
	TabConvertFillUpUnitList(list3b, convtype);
	GetPrgRes()->GetConvert()->GetCurrentUnit( convtype, input, output);

	if( input >= 0 )
	{
		int i = FindItem(list2b, input);

		if( i >= 0 )
			ListView_SetItemState(list2b, i, LVIS_SELECTED, LVIS_SELECTED); //LVIS_FOCUSED
	}

	if( output >= 0 )
	{
		int i = FindItem(list3b, output);

		if( i>=0 )
			ListView_SetItemState(list3b, i, LVIS_SELECTED, LVIS_SELECTED); //LVIS_FOCUSED
	}
}



/*!
	we must check whether the item id is really selected
	(this method is called from WM_NOTIFY and can be another states on the item)
*/
void TabConvertTypeChanged(HWND hWnd, int id)
{
HWND list1 = GetDlgItem(hWnd, IDC_LIST_UNIT_TYPE);

	if( ListView_GetItemState(list1, id, LVIS_SELECTED | LVIS_FOCUSED) != (LVIS_SELECTED | LVIS_FOCUSED) )
		return;


	// the item is selected now
	LVITEM item;
	item.iItem = id;
	item.iSubItem = 0;
	item.mask = LVIF_PARAM;
	if( !ListView_GetItem(list1, &item) )
		return;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->GetConvert()->SetCurrentType( (int)item.lParam );
	GetPrgRes()->GetThreadController()->StartCalculating();

	TabConvertFillUpUnitLists(hWnd);
}



/*!
	we must check whether the item id is really selected
	(this method is called from WM_NOTIFY and can be another states on the item)
*/
void TabConvertUnitInputChanged(HWND hWnd, int id)
{
HWND list2b = GetDlgItem(hWnd, IDC_LIST_UNIT_INPUT2);

	if( ListView_GetItemState(list2b, id, LVIS_SELECTED | LVIS_FOCUSED) != (LVIS_SELECTED | LVIS_FOCUSED) )
		return;

	// the item is selected
	LVITEM item;
	item.iItem = id;
	item.iSubItem = 0;
	item.mask = LVIF_PARAM;
	if( !ListView_GetItem(list2b, &item) )
		return;

	int convtype = GetPrgRes()->GetConvert()->GetCurrentType();
	int input, output;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->GetConvert()->GetCurrentUnit(convtype, input, output);
	GetPrgRes()->GetConvert()->SetCurrentUnit(convtype, (int)item.lParam, output );
	GetPrgRes()->GetThreadController()->StartCalculating();
}


/*!
	we must check whether the item id is really selected
	(this method is called from WM_NOTIFY and can be another states on the item)
*/
void TabConvertUnitOutputChanged(HWND hWnd, int id)
{
HWND list3b = GetDlgItem(hWnd, IDC_LIST_UNIT_OUTPUT2);

	if( ListView_GetItemState(list3b, id, LVIS_SELECTED | LVIS_FOCUSED) != (LVIS_SELECTED | LVIS_FOCUSED) )
		return;


	// the item is selected
	LVITEM item;
	item.iItem = id;
	item.iSubItem = 0;
	item.mask = LVIF_PARAM;
	if( !ListView_GetItem(list3b, &item) )
		return;

	int convtype = GetPrgRes()->GetConvert()->GetCurrentType();
	int input, output;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->GetConvert()->GetCurrentUnit(convtype, input, output);
	GetPrgRes()->GetConvert()->SetCurrentUnit(convtype, input, (int)item.lParam );
	GetPrgRes()->GetThreadController()->StartCalculating();
}





void SetLanguageTabConvert(HWND hWnd)
{
Languages * plang = GetPrgRes()->GetLanguages();
Convert * pconv   = GetPrgRes()->GetConvert();

	SetDlgItemText(hWnd, IDC_STATIC_UNIT_CONVERSION, plang->GuiMessage(Languages::convert_type) );
	SetDlgItemText(hWnd, IDC_STATIC_UNIT_INPUT, plang->GuiMessage(Languages::convert_input) );
	SetDlgItemText(hWnd, IDC_STATIC_UNIT_OUTPUT, plang->GuiMessage(Languages::convert_output) );
	SetDlgItemText(hWnd, IDC_COMBO_DYNAMIC_OUTPUT, plang->GuiMessage(Languages::convert_dynamic_output) );

	HWND list1 = GetDlgItem(hWnd, IDC_LIST_UNIT_TYPE);

	ListView_DeleteAllItems(list1);

	LVITEM item;
	item.mask = LVIF_PARAM | LVIF_TEXT;
	item.iSubItem = 0;

	unsigned int conv_type_size = pconv->GetTypeSize();

	for(unsigned int i = 0 ; i<conv_type_size ; ++i)
	{
		item.lParam = i;
		item.iItem = i;
		int country = static_cast<unsigned int>(plang->GetCurrentLanguage());
		item.pszText = const_cast<char*>(pconv->GetTypeName(country, i));

		ListView_InsertItem(list1, &item);
	}

	int type = FindItem(list1, GetPrgRes()->GetConvert()->GetCurrentType() );
	if( type != -1 )
		ListView_SetItemState(list1, type, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
		// there'll be generated wm_notify message and the next lists will be changed then
}




void SetLanguage(HWND hTab)
{
TCITEM tab;
	
	tab.mask = TCIF_TEXT;

	tab.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::tab_standard) );
	TabCtrl_SetItem(hTab,tab_standard, &tab);

	tab.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::tab_variables) );
	TabCtrl_SetItem(hTab,tab_variables, &tab);

	tab.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::tab_functions) );
	TabCtrl_SetItem(hTab,tab_functions, &tab);

	#ifndef TTCALC_PORTABLE
	tab.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::tab_precision) );
	TabCtrl_SetItem(hTab,tab_precision, &tab);
	#endif

	tab.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::tab_display) );
	TabCtrl_SetItem(hTab,tab_display, &tab);

	#ifdef TTCALC_CONVERT
	tab.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::tab_convert) );
	TabCtrl_SetItem(hTab,tab_convert, &tab);
	#endif

	SetLanguageTabStandard(  GetPrgRes()->GetTabWindow(tab_standard)  );
	SetLanguageTabVariables( GetPrgRes()->GetTabWindow(tab_variables) );
	SetLanguageTabFunctions( GetPrgRes()->GetTabWindow(tab_functions) );

	#ifndef TTCALC_PORTABLE
	SetLanguageTabPrecision( GetPrgRes()->GetTabWindow(tab_precision) );
	#endif

	SetLanguageTabDisplay( GetPrgRes()->GetTabWindow(tab_display) );

	#ifdef TTCALC_CONVERT
	SetLanguageTabConvert( GetPrgRes()->GetTabWindow(tab_convert) );
	#endif
		
	InvalidateRect(hTab, 0, false);
}



BOOL WmTabCommand_SetPrecision1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	#ifndef TTCALC_PORTABLE

	// there are another messages besides that one
	// which is sent when a user clicks on the radio button
	if( IsDlgButtonChecked(hWnd, IDC_RADIO_PRECISION_1) != BST_CHECKED )
		return false;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->SetPrecision(0);
	GetPrgRes()->GetThreadController()->StartCalculating();

	#endif

return true;
}


BOOL WmTabCommand_SetPrecision2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	#ifndef TTCALC_PORTABLE

	// there are another messages besides that one
	// which is sent when a user clicks on the radio button
	if( IsDlgButtonChecked(hWnd, IDC_RADIO_PRECISION_2) != BST_CHECKED )
		return false;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->SetPrecision(1);
	GetPrgRes()->GetThreadController()->StartCalculating();

	#endif

return true;
}


BOOL WmTabCommand_SetPrecision3(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	#ifndef TTCALC_PORTABLE

	// there are another messages besides that one
	// which is sent when a user clicks on the radio button
	if( IsDlgButtonChecked(hWnd, IDC_RADIO_PRECISION_3) != BST_CHECKED )
		return false;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->SetPrecision(2);
	GetPrgRes()->GetThreadController()->StartCalculating();

	#endif

return true;
}


BOOL WmTabCommand_DisplayInputChanged(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( HIWORD(wParam) != CBN_SELCHANGE )
		return false;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->SetBaseInput( (int)SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_INPUT, CB_GETCURSEL, 0, 0) + 2 );
	GetPrgRes()->GetThreadController()->StartCalculating();

return true;
}


BOOL WmTabCommand_DisplayOutputChanged(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( HIWORD(wParam) != CBN_SELCHANGE )
		return false;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->SetBaseOutput( (int)SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_OUTPUT, CB_GETCURSEL, 0, 0) + 2);
	GetPrgRes()->GetThreadController()->StartCalculating();

return true;
}


BOOL WmTabCommand_DisplayDecimalPointChanged(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( HIWORD(wParam) != CBN_SELCHANGE )
		return false;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->SetDecimalPoint( (int)SendDlgItemMessage(hWnd, IDC_COMBO_OUTPUT_DECIMAL_POINT, CB_GETCURSEL, 0, 0) );
	GetPrgRes()->GetThreadController()->StartCalculating();

return true;
}


BOOL WmTabCommand_DisplayRoundingChanged(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( HIWORD(wParam) != CBN_SELCHANGE )
		return false;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->SetDisplayRounding( (int)SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_ROUNDING, CB_GETCURSEL, 0, 0) - 1);
	GetPrgRes()->GetThreadController()->StartCalculating();

return true;
}

BOOL WmTabCommand_RemoveZeroesChanged(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->GetThreadController()->StopCalculating();

	if( IsDlgButtonChecked(hWnd, IDC_CHECK_REMOVE_ZEROES) == BST_CHECKED )
		GetPrgRes()->SetRemovingZeroes(true);
	else
		GetPrgRes()->SetRemovingZeroes(false);

	GetPrgRes()->GetThreadController()->StartCalculating();

return true;
}


BOOL WmTabCommand_DisplayDegRadGradChanged(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( HIWORD(wParam) != CBN_SELCHANGE )
		return false;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->SetDegRadGrad( (int)SendDlgItemMessage(hWnd, IDC_COMBO_DEG_RAD_GRAD, CB_GETCURSEL, 0, 0) );
	GetPrgRes()->GetThreadController()->StartCalculating();

return true;
}


void SetDisablingDisplayWhenScientific(HWND hWnd)
{
	if( IsDlgButtonChecked(hWnd, IDC_RADIO_DISPLAY_ALWAYS_SCIENTIFIC) == BST_CHECKED )
	{
		EnableWindow(GetDlgItem(hWnd, IDC_UPDOWN_DISPLAY_WHEN_SCIENTIFIC), false);
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_DISPLAY_WHEN_SCIENTIFIC), false);
	}
	else
	{
		EnableWindow(GetDlgItem(hWnd, IDC_UPDOWN_DISPLAY_WHEN_SCIENTIFIC), true);
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_DISPLAY_WHEN_SCIENTIFIC), true);
	}
}




BOOL WmTabCommand_DisplayGrouping(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( HIWORD(wParam) != CBN_SELCHANGE )
		return false;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->SetGrouping( (int)SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_GROUPING, CB_GETCURSEL, 0, 0) );
	GetPrgRes()->SetGroupingDigits( (int)SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_GROUPING_DIGITS, CB_GETCURSEL, 0, 0) + 1 );
	GetPrgRes()->GetThreadController()->StartCalculating();

	SetDisablingGroupingDigits(hWnd, GetPrgRes()->GetGrouping());

return true;
}


BOOL WmTabCommand_DisplayInputDecimalPoint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( HIWORD(wParam) != CBN_SELCHANGE )
		return false;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->SetInputDecimalPoint( (int)SendDlgItemMessage(hWnd, IDC_COMBO_INPUT_DECIMAL_POINT, CB_GETCURSEL, 0, 0) );
	GetPrgRes()->GetThreadController()->StartCalculating();

return true;
}



BOOL WmTabCommand_DisplayParamSep(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( HIWORD(wParam) != CBN_SELCHANGE )
		return false;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->SetParamSep( (int)SendDlgItemMessage(hWnd, IDC_COMBO_PARAM_SEPARATE, CB_GETCURSEL, 0, 0) );
	GetPrgRes()->GetThreadController()->StartCalculating();

return true;
}




BOOL WmTabCommand_SetDisplayAlwaysScientific(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// there are another messages besides that one
	// which is sent when a user clicks on the radio button
	if( IsDlgButtonChecked(hWnd, IDC_RADIO_DISPLAY_ALWAYS_SCIENTIFIC) != BST_CHECKED )
		return false;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->SetDisplayAlwaysScientific(true);
	GetPrgRes()->GetThreadController()->StartCalculating();

	SetDisablingDisplayWhenScientific(hWnd);

return true;
}


BOOL WmTabCommand_SetDisplayNotAlwaysScientific(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// there are another messages besides that one
	// which is sent when a user clicks on the radio button
	if( IsDlgButtonChecked(hWnd, IDC_RADIO_DISPLAY_NOT_ALWAYS_SCIENTIFIC) != BST_CHECKED )
		return false;

	GetPrgRes()->GetThreadController()->StopCalculating();
	GetPrgRes()->SetDisplayAlwaysScientific(false);
	GetPrgRes()->GetThreadController()->StartCalculating();

	SetDisablingDisplayWhenScientific(hWnd);

return true;
}



BOOL WmTabCommand_SetDisplayWhenScientific(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static bool correct_insertion = false;

	if( HIWORD(wParam)==EN_CHANGE && !correct_insertion )
	{	
		char buffer[20];

		GetDlgItemText(hWnd, IDC_EDIT_DISPLAY_WHEN_SCIENTIFIC, buffer, sizeof(buffer)/sizeof(char));
		int w2,w1 = atoi(buffer);

		GetPrgRes()->GetThreadController()->StopCalculating();
		GetPrgRes()->SetDisplayWhenScientific(w1);
		w2 = GetPrgRes()->GetDisplayWhenScientific();
		GetPrgRes()->GetThreadController()->StartCalculating();
	
		if( w1 != w2 )
		{
			sprintf(buffer,"%d", w2);

			//
			correct_insertion = true;
			SetDlgItemText(hWnd, IDC_EDIT_DISPLAY_WHEN_SCIENTIFIC, buffer);
			correct_insertion = false;
		}
	}



return true;
}





BOOL WmTabCommand_Press_bracketsall(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("(", ")");

return true;
}


BOOL WmTabCommand_Press_1div_bracketsall(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("1/(", ")");

return true;
}


BOOL WmTabCommand_Press_semicolon(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText(" ; ", 0);

return true;
}


BOOL WmTabCommand_Press_percentage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("%", 0);

return true;
}


BOOL WmTabCommand_Press_floor(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("floor(", ")");

return true;
}


BOOL WmTabCommand_Press_ceil(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("ceil(", ")");

return true;
}


BOOL WmTabCommand_Press_min(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("min(", ")");

return true;
}


BOOL WmTabCommand_Press_max(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("max(", ")");

return true;
}


BOOL WmTabCommand_Press_avg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("avg(", ")");

return true;
}


BOOL WmTabCommand_Press_sum(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("sum(", ")");

return true;
}


BOOL WmTabCommand_Press_root(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("root(", " ; )", 1);

return true;
}


BOOL WmTabCommand_Press_gamma(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("gamma(", ")");

return true;
}

BOOL WmTabCommand_Press_sqrt(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InsertText("sqrt(", ")");

return true;
}



void CreateTabCommandMessagesTable(Messages<BOOL> & cmessages)
{
	cmessages.Associate(IDC_BUTTON_PRESS_0, WmTabCommand_Press_0_9);
	cmessages.Associate(IDC_BUTTON_PRESS_1, WmTabCommand_Press_0_9);
	cmessages.Associate(IDC_BUTTON_PRESS_2, WmTabCommand_Press_0_9);
	cmessages.Associate(IDC_BUTTON_PRESS_3, WmTabCommand_Press_0_9);
	cmessages.Associate(IDC_BUTTON_PRESS_4, WmTabCommand_Press_0_9);
	cmessages.Associate(IDC_BUTTON_PRESS_5, WmTabCommand_Press_0_9);
	cmessages.Associate(IDC_BUTTON_PRESS_6, WmTabCommand_Press_0_9);
	cmessages.Associate(IDC_BUTTON_PRESS_7, WmTabCommand_Press_0_9);
	cmessages.Associate(IDC_BUTTON_PRESS_8, WmTabCommand_Press_0_9);
	cmessages.Associate(IDC_BUTTON_PRESS_9, WmTabCommand_Press_0_9);
	cmessages.Associate(IDC_BUTTON_COMMA,   WmTabCommand_Press_comma);

	cmessages.Associate(IDC_BUTTON_E,  WmTabCommand_Press_e);
	cmessages.Associate(IDC_BUTTON_PI, WmTabCommand_Press_pi);
	cmessages.Associate(IDC_BUTTON_SIN, WmTabCommand_Press_sin);
	cmessages.Associate(IDC_BUTTON_COS, WmTabCommand_Press_cos);
	cmessages.Associate(IDC_BUTTON_TAN, WmTabCommand_Press_tan);
	cmessages.Associate(IDC_BUTTON_COT, WmTabCommand_Press_cot);
	cmessages.Associate(IDC_BUTTON_LN, WmTabCommand_Press_ln);
	cmessages.Associate(IDC_BUTTON_LOG, WmTabCommand_Press_log);
	cmessages.Associate(IDC_BUTTON_ABS, WmTabCommand_Press_abs);
	cmessages.Associate(IDC_BUTTON_FACTORIAL, WmTabCommand_Press_factorial);
	cmessages.Associate(IDC_BUTTON_SEMICOLON, WmTabCommand_Press_semicolon);
	cmessages.Associate(IDC_BUTTON_PERCENTAGE, WmTabCommand_Press_percentage);
	cmessages.Associate(IDC_BUTTON_FLOOR, WmTabCommand_Press_floor);
	cmessages.Associate(IDC_BUTTON_CEIL, WmTabCommand_Press_ceil);
	cmessages.Associate(IDC_BUTTON_MIN, WmTabCommand_Press_min);
	cmessages.Associate(IDC_BUTTON_MAX, WmTabCommand_Press_max);
	cmessages.Associate(IDC_BUTTON_AVG, WmTabCommand_Press_avg);
	cmessages.Associate(IDC_BUTTON_SUM, WmTabCommand_Press_sum);
	cmessages.Associate(IDC_BUTTON_ROOT, WmTabCommand_Press_root);
	cmessages.Associate(IDC_BUTTON_GAMMA, WmTabCommand_Press_gamma);


	
	cmessages.Associate(IDC_BUTTON_DIV, WmTabCommand_Press_div);
	cmessages.Associate(IDC_BUTTON_MUL, WmTabCommand_Press_mul);
	cmessages.Associate(IDC_BUTTON_SUB, WmTabCommand_Press_sub);
	cmessages.Associate(IDC_BUTTON_ADD, WmTabCommand_Press_add);
	cmessages.Associate(IDC_BUTTON_FIRST_BRACKET, WmTabCommand_Press_first_bracket);
	cmessages.Associate(IDC_BUTTON_LAST_BRACKET, WmTabCommand_Press_last_bracket);
	
	cmessages.Associate(IDC_BUTTON_CLEAR, WmTabCommand_Press_clear);
	cmessages.Associate(IDC_BUTTON_ERROR, WmTabCommand_Press_error);

	cmessages.Associate(IDC_BUTTON_ASIN, WmTabCommand_Press_asin);
	cmessages.Associate(IDC_BUTTON_ACOS, WmTabCommand_Press_acos);
	cmessages.Associate(IDC_BUTTON_ATAN, WmTabCommand_Press_atan);
	cmessages.Associate(IDC_BUTTON_ACOT, WmTabCommand_Press_acot);
	cmessages.Associate(IDC_BUTTON_SGN, WmTabCommand_Press_sgn);
	cmessages.Associate(IDC_BUTTON_MOD, WmTabCommand_Press_mod);

	cmessages.Associate(IDC_BUTTON_POWER, WmTabCommand_Press_power);
	cmessages.Associate(IDC_BUTTON_INT, WmTabCommand_Press_int);
	cmessages.Associate(IDC_BUTTON_ROUND, WmTabCommand_Press_round);
	cmessages.Associate(IDC_BUTTON_EXP, WmTabCommand_Press_exp);
	cmessages.Associate(IDC_BUTTON_BRACKETS_ALL, WmTabCommand_Press_bracketsall);
	cmessages.Associate(IDC_BUTTON_1DIV_BRACKETS_ALL, WmTabCommand_Press_1div_bracketsall);
	cmessages.Associate(IDC_BUTTON_SQRT, WmTabCommand_Press_sqrt);


	cmessages.Associate(IDC_BUTTON_ADD_VARIABLE, Variables::WmTabCommand_AddVariable);
	cmessages.Associate(IDC_BUTTON_EDIT_VARIABLE, Variables::WmTabCommand_EditVariable);
	cmessages.Associate(IDC_BUTTON_DELETE_VARIABLE, Variables::WmTabCommand_DeleteVariable);

	cmessages.Associate(IDOK, WmTabCommand_IDOK);
	cmessages.Associate(IDCANCEL, WmTabCommand_IDCANCEL);

	cmessages.Associate(IDC_BUTTON_ADD_FUNCTION, Functions::WmTabCommand_AddFunction);
	cmessages.Associate(IDC_BUTTON_EDIT_FUNCTION, Functions::WmTabCommand_EditFunction);
	cmessages.Associate(IDC_BUTTON_DELETE_FUNCTION, Functions::WmTabCommand_DeleteFunction);

	#ifndef TTCALC_PORTABLE

	cmessages.Associate(IDC_RADIO_PRECISION_1, WmTabCommand_SetPrecision1);
	cmessages.Associate(IDC_RADIO_PRECISION_2, WmTabCommand_SetPrecision2);
	cmessages.Associate(IDC_RADIO_PRECISION_3, WmTabCommand_SetPrecision3);

	#endif

	cmessages.Associate(IDC_COMBO_DISPLAY_INPUT, WmTabCommand_DisplayInputChanged);
	cmessages.Associate(IDC_COMBO_DISPLAY_OUTPUT, WmTabCommand_DisplayOutputChanged);
	cmessages.Associate(IDC_COMBO_DISPLAY_ROUNDING, WmTabCommand_DisplayRoundingChanged);
	cmessages.Associate(IDC_CHECK_REMOVE_ZEROES, WmTabCommand_RemoveZeroesChanged);
	cmessages.Associate(IDC_RADIO_DISPLAY_ALWAYS_SCIENTIFIC, WmTabCommand_SetDisplayAlwaysScientific);
	cmessages.Associate(IDC_RADIO_DISPLAY_NOT_ALWAYS_SCIENTIFIC, WmTabCommand_SetDisplayNotAlwaysScientific);
	cmessages.Associate(IDC_EDIT_DISPLAY_WHEN_SCIENTIFIC, WmTabCommand_SetDisplayWhenScientific);
	cmessages.Associate(IDC_COMBO_OUTPUT_DECIMAL_POINT, WmTabCommand_DisplayDecimalPointChanged);
	cmessages.Associate(IDC_COMBO_DEG_RAD_GRAD, WmTabCommand_DisplayDegRadGradChanged);
	cmessages.Associate(IDC_COMBO_DISPLAY_GROUPING, WmTabCommand_DisplayGrouping);
	cmessages.Associate(IDC_COMBO_DISPLAY_GROUPING_DIGITS, WmTabCommand_DisplayGrouping);
	cmessages.Associate(IDC_COMBO_INPUT_DECIMAL_POINT, WmTabCommand_DisplayInputDecimalPoint);
	cmessages.Associate(IDC_COMBO_PARAM_SEPARATE, WmTabCommand_DisplayParamSep);
}

/*
	a function for WM_COMMAND for dialogs which are on the tab
*/
BOOL WmTabCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static Messages<BOOL> command_messages;
	
	if( command_messages.Empty() )
		// initiation
		CreateTabCommandMessagesTable(command_messages);

return command_messages.Call(LOWORD(wParam), hWnd, message, wParam, lParam);
}

BOOL WmTabInitDialog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return true;
}


/*!
	this function enables or disables the 'edit' and 'delete' buttons
	on the variables' or functions' tab depending on how many variables
	are selected in the list
*/
void SetDisablingEditDeleteVariableOrFunctionButtons(int count, HWND add,
													 HWND edit, HWND del, HWND focus)
{
	if( count == 1 )
		// if there is one selected item we're enabling the edit button
		EnableWindow(edit, true);
	else
	{
		// if there is not one selected item (or none) we're disabling the edit
		EnableWindow(edit, false);
	
		// if the focus is on the edit button we must change it 
		// for example into the add button
		// (we must change the BS_DEFPUSHBUTTON style as well)
		if( focus == edit )
		{
			SetFocus(add);
			SendMessage(add, BM_SETSTYLE, (WPARAM)BS_DEFPUSHBUTTON, true);
			SendMessage(edit, BM_SETSTYLE, (WPARAM)BS_PUSHBUTTON, true);
		}
	}


	if( count != 0 )
		// if there is one or more items selected we're enabling the del button
		EnableWindow(del, true);
	else
	{
		// if there are not selected items we're disabling the del
		EnableWindow(del, false);
		
		if( focus == del )
		{
			SetFocus(add);
			SendMessage(add, BM_SETSTYLE, (WPARAM)BS_DEFPUSHBUTTON, true);
			SendMessage(del, BM_SETSTYLE, (WPARAM)BS_PUSHBUTTON, true);
		}
	}
}


/*!
	this function enables or disables the 'edit' and 'delete' buttons
	on the variable's tab depending on how many variables are selected in the list
*/
void SetDisablingEditDeleteVariableButtons(HWND hWnd)
{
HWND focus = GetFocus();
HWND list  = GetDlgItem(hWnd, IDC_VARIABLES_LIST);
HWND add   = GetDlgItem(hWnd, IDC_BUTTON_ADD_VARIABLE);
HWND edit  = GetDlgItem(hWnd, IDC_BUTTON_EDIT_VARIABLE);
HWND del   = GetDlgItem(hWnd, IDC_BUTTON_DELETE_VARIABLE);
int count  = ListView_GetSelectedCount(list);

	SetDisablingEditDeleteVariableOrFunctionButtons(count, add, edit, del, focus);
}


/*!
	this function enables or disables the 'edit' and 'delete' buttons
	on the function's tab depending on how many variables are selected in the list
*/
void SetDisablingEditDeleteFunctionButtons(HWND hWnd)
{
HWND focus = GetFocus();
HWND list  = GetDlgItem(hWnd, IDC_FUNCTIONS_LIST);
HWND add   = GetDlgItem(hWnd, IDC_BUTTON_ADD_FUNCTION);
HWND edit  = GetDlgItem(hWnd, IDC_BUTTON_EDIT_FUNCTION);
HWND del   = GetDlgItem(hWnd, IDC_BUTTON_DELETE_FUNCTION);
int count  = ListView_GetSelectedCount(list);

	SetDisablingEditDeleteVariableOrFunctionButtons(count, add, edit, del, focus);
}


int FillUpVariableList(HWND list)
{
	// we don't have to block the second thread here
	// because it can only read from variables' table
	// (we can read and the second thread can read in the same time as well)
	ttmath::Objects::CIterator iv = GetPrgRes()->GetVariables()->Begin();

	int size;
	for( size=0 ; iv != GetPrgRes()->GetVariables()->End() ; ++iv, ++size )
		Variables::AddNewItemToVariableList(list, iv->first, iv->second.value);

return size;
}


int FillUpFunctionList(HWND list)
{
	// we don't have to block the second thread here
	// because it can only read from functions' table
	// (we can read and the second thread can read in the same time as well)
	ttmath::Objects::CIterator iv = GetPrgRes()->GetFunctions()->Begin();

	int size;
	for( size=0 ; iv != GetPrgRes()->GetFunctions()->End() ; ++iv, ++size )
		Functions::AddNewItemToFunctionList(list, iv->first, iv->second.value, iv->second.param);

return size;
}


BOOL WmInitTabVariables(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
HWND list = GetDlgItem(hWnd, IDC_VARIABLES_LIST);
LVCOLUMN column;
RECT r;

	GetWindowRect(list, &r);

	int list_cx = r.right - r.left;
	int header1_cx = 70;

	column.mask    = LVCF_TEXT | LVCF_WIDTH;
	column.cx      = header1_cx;
	column.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::list_variables_header_1) );
	ListView_InsertColumn(list, 0, &column);

	column.cx      = list_cx - header1_cx;
	column.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::list_variables_header_2) );
	ListView_InsertColumn(list, 1, &column);

	ListView_SetExtendedListViewStyle(list,LVS_EX_FULLROWSELECT);
	SetDisablingEditDeleteVariableButtons(hWnd);

	if( FillUpVariableList(list) > 0 )
		ListView_SetItemState(list, 0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

return true;
}

BOOL WmInitTabFunctions(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
HWND list = GetDlgItem(hWnd, IDC_FUNCTIONS_LIST);
LVCOLUMN column;
RECT r;

	GetWindowRect(list, &r);

	int list_cx = r.right - r.left;
	int header1_cx = 70;
	int header2_cx = 70;

	column.mask    = LVCF_TEXT | LVCF_WIDTH;
	column.cx      = header1_cx;
	column.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::list_functions_header_1) );
	ListView_InsertColumn(list, 0, &column);

	column.cx      = header2_cx;
	column.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::list_functions_header_2) );
	ListView_InsertColumn(list, 1, &column);

	column.cx      = list_cx - header1_cx - header2_cx;
	column.pszText = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::list_functions_header_3) );
	ListView_InsertColumn(list, 2, &column);

	ListView_SetExtendedListViewStyle(list,LVS_EX_FULLROWSELECT);

	SetDisablingEditDeleteFunctionButtons(hWnd);

	if( FillUpFunctionList(list) > 0 )
		ListView_SetItemState(list, 0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

return true;
}


BOOL WmInitTabPrecision(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	#ifndef TTCALC_PORTABLE
	CheckDlgButton(hWnd, IDC_RADIO_PRECISION_1 + GetPrgRes()->GetPrecision(), BST_CHECKED);
	#endif

return true;
}




BOOL WmInitTabDisplay(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
int i;
char buffer[20];

	for(i=2 ; i<=16 ; ++i)
	{
		sprintf(buffer,"%d",i);		

		switch( i )
		{
		case 2:
			strcat(buffer, " (bin)");
			break;

		case 8:
			strcat(buffer, " (oct)");
			break;

		case 10:
			strcat(buffer, " (dec)");
			break;

		case 16:
			strcat(buffer, " (hex)");
			break;
		}

		SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_INPUT, CB_ADDSTRING, 0, (LPARAM)buffer);
		SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_OUTPUT, CB_ADDSTRING, 0, (LPARAM)buffer);
	} // for

	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_INPUT, CB_SETCURSEL, GetPrgRes()->GetBaseInput()-2, 0);
	SendDlgItemMessage(hWnd, IDC_COMBO_DISPLAY_OUTPUT, CB_SETCURSEL, GetPrgRes()->GetBaseOutput()-2, 0);


	if( GetPrgRes()->GetDisplayAlwaysScientific() )
		CheckDlgButton(hWnd, IDC_RADIO_DISPLAY_ALWAYS_SCIENTIFIC, BST_CHECKED);
	else
		CheckDlgButton(hWnd, IDC_RADIO_DISPLAY_NOT_ALWAYS_SCIENTIFIC, BST_CHECKED);

	SetDisablingDisplayWhenScientific(hWnd);
	SetDisablingGroupingDigits(hWnd);

	SendDlgItemMessage(hWnd, IDC_UPDOWN_DISPLAY_WHEN_SCIENTIFIC, UDM_SETBASE, 10,0);
	SendDlgItemMessage(hWnd, IDC_UPDOWN_DISPLAY_WHEN_SCIENTIFIC, UDM_SETRANGE, 0, MAKELONG(99,1));
	SendDlgItemMessage(hWnd, IDC_UPDOWN_DISPLAY_WHEN_SCIENTIFIC, UDM_SETPOS, 0, 5);
	SendDlgItemMessage(hWnd, IDC_UPDOWN_DISPLAY_WHEN_SCIENTIFIC, UDM_SETBUDDY, (WPARAM)GetDlgItem(hWnd, IDC_EDIT_DISPLAY_WHEN_SCIENTIFIC), 0);
	SendDlgItemMessage(hWnd, IDC_EDIT_DISPLAY_WHEN_SCIENTIFIC, EM_SETLIMITTEXT, 2, 0);

	sprintf(buffer,"%d", GetPrgRes()->GetDisplayWhenScientific());
	SetDlgItemText(hWnd, IDC_EDIT_DISPLAY_WHEN_SCIENTIFIC, buffer);

	SendDlgItemMessage(hWnd, IDC_COMBO_OUTPUT_DECIMAL_POINT, CB_ADDSTRING, 0, (LPARAM)" .");
	SendDlgItemMessage(hWnd, IDC_COMBO_OUTPUT_DECIMAL_POINT, CB_ADDSTRING, 0, (LPARAM)" ,");
	SendDlgItemMessage(hWnd, IDC_COMBO_OUTPUT_DECIMAL_POINT, CB_SETCURSEL, GetPrgRes()->GetDecimalPoint(), 0);

	if( GetPrgRes()->GetRemovingZeroes() )
		CheckDlgButton(hWnd, IDC_CHECK_REMOVE_ZEROES, BST_CHECKED);

	SendDlgItemMessage(hWnd, IDC_COMBO_DEG_RAD_GRAD, CB_ADDSTRING, 0, (LPARAM)"deg");
	SendDlgItemMessage(hWnd, IDC_COMBO_DEG_RAD_GRAD, CB_ADDSTRING, 0, (LPARAM)"rad");
	SendDlgItemMessage(hWnd, IDC_COMBO_DEG_RAD_GRAD, CB_ADDSTRING, 0, (LPARAM)"grad");
	SendDlgItemMessage(hWnd, IDC_COMBO_DEG_RAD_GRAD, CB_SETCURSEL, GetPrgRes()->GetDegRadGrad(), 0);

return true;
}



BOOL WmInitTabConvert(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
HWND list1 = GetDlgItem(hWnd, IDC_LIST_UNIT_TYPE);
HWND list2b = GetDlgItem(hWnd, IDC_LIST_UNIT_INPUT2);
HWND list3b = GetDlgItem(hWnd, IDC_LIST_UNIT_OUTPUT2);

	ListView_SetExtendedListViewStyle(list1, LVS_EX_FULLROWSELECT);
	ListView_SetExtendedListViewStyle(list2b, LVS_EX_FULLROWSELECT);
	ListView_SetExtendedListViewStyle(list3b, LVS_EX_FULLROWSELECT);

	LVCOLUMN col;

	col.mask = LVCF_FMT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;

	col.cx = 70;
	ListView_InsertColumn(list1, 0, &col);

	col.cx = 90;
	ListView_InsertColumn(list2b, 0, &col);

	col.cx = 90;
	ListView_InsertColumn(list3b, 0, &col);

return true;
}


BOOL WmNotify(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
NMHDR * pnmh     = (LPNMHDR) lParam;
HWND var_list    = GetDlgItem(hWnd,IDC_VARIABLES_LIST);
HWND fun_list    = GetDlgItem(hWnd,IDC_FUNCTIONS_LIST);


	if( pnmh->hwndFrom == var_list )
	{
		if( pnmh->code == LVN_ITEMCHANGED )
		{
			SetDisablingEditDeleteVariableButtons( hWnd );
			return true;
		}
		else
		if( pnmh->code == UINT(NM_DBLCLK) )
		{
			Variables::WmTabCommand_EditVariable(hWnd, WM_COMMAND, IDC_BUTTON_EDIT_VARIABLE, 0);	
			return true;
		}
		else
		if( pnmh->code == LVN_KEYDOWN )
		{
			NMLVKEYDOWN * pnkd = (NMLVKEYDOWN*) lParam;
			
			if( pnkd->wVKey == VK_DELETE )
				Variables::WmTabCommand_DeleteVariable(0,0,0,0);
		}
	}

	if( pnmh->hwndFrom == fun_list )
	{
		if( pnmh->code == LVN_ITEMCHANGED )
		{
			SetDisablingEditDeleteFunctionButtons( hWnd );
			return true;
		}
		else
		if( pnmh->code == UINT(NM_DBLCLK) )
		{
			Functions::WmTabCommand_EditFunction(hWnd, WM_COMMAND, IDC_BUTTON_EDIT_FUNCTION, 0);
			return true;
		}
		else
		if( pnmh->code == LVN_KEYDOWN )
		{
			NMLVKEYDOWN * pnkd = (NMLVKEYDOWN*) lParam;
			
			if( pnkd->wVKey == VK_DELETE )
				Functions::WmTabCommand_DeleteFunction(0,0,0,0);
		}

	}


#ifdef TTCALC_CONVERT
HWND conv_list1  = GetDlgItem(hWnd,IDC_LIST_UNIT_TYPE);
HWND conv_list2b = GetDlgItem(hWnd,IDC_LIST_UNIT_INPUT2);
HWND conv_list3b = GetDlgItem(hWnd,IDC_LIST_UNIT_OUTPUT2);

	if( pnmh->hwndFrom==conv_list1 || pnmh->hwndFrom==conv_list2b || pnmh->hwndFrom==conv_list3b )
	{
		if( pnmh->code == LVN_ITEMCHANGED )
		{
			NMLISTVIEW * pnmv = (NMLISTVIEW*) lParam;

			if( pnmv->iItem != -1 )
			{
				if( pnmh->hwndFrom == conv_list1 )
					TabConvertTypeChanged(hWnd, pnmv->iItem);
				if( pnmh->hwndFrom == conv_list2b )
					TabConvertUnitInputChanged(hWnd, pnmv->iItem);
				else
				if( pnmh->hwndFrom == conv_list3b )
					TabConvertUnitOutputChanged(hWnd, pnmv->iItem);
			}
		}
	}
#endif

return false;
}




void CreateTabMessagesTable(Messages<BOOL> & messages)
{
	messages.Associate(WM_INITDIALOG,			WmTabInitDialog);
	messages.Associate(WM_COMMAND,				WmTabCommand);
	messages.Associate(WM_INIT_TAB_VARIABLES,	WmInitTabVariables);
	messages.Associate(WM_INIT_TAB_FUNCTIONS,	WmInitTabFunctions);
	
	#ifndef TTCALC_PORTABLE
	messages.Associate(WM_INIT_TAB_PRECISION,	WmInitTabPrecision);
	#endif

	messages.Associate(WM_INIT_TAB_DISPLAY,		WmInitTabDisplay);

	#ifdef TTCALC_CONVERT
	messages.Associate(WM_INIT_TAB_CONVERT,		WmInitTabConvert);
	#endif

	messages.Associate(WM_NOTIFY,				WmNotify);
}



BOOL CALLBACK TabWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static Messages<BOOL> messages;

	if( messages.Empty() )
		// initiation
		CreateTabMessagesTable(messages);

return messages.Call(message, hWnd, message, wParam, lParam);
}


POINT ResizeTabDialog(HWND tab, HWND dialog, int tabx, int taby, int borderx, int bordery)
{
RECT r_dialog;
POINT p, new_dialog_size;

	GetWindowRect(dialog, &r_dialog);
	p.x = r_dialog.left;
	p.y = r_dialog.top;
	ScreenToClient(tab,&p);
	new_dialog_size.x = tabx-borderx-p.x;
	new_dialog_size.y = taby-bordery-p.y;
	int resize_flags = SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER;
	SetWindowPos(dialog, 0, 0, 0, new_dialog_size.x, new_dialog_size.y, resize_flags);

return new_dialog_size;
}

void SetSizeOfFunctionVariableList(HWND tab, int tabx, int taby, int borderx, int bordery, int tab_id, int list_id, int last_column)
{
RECT r;
POINT p;

	HWND dialog = GetPrgRes()->GetTabWindow(tab_id);
	HWND list   = GetDlgItem(dialog, list_id);
	int resize_flags = SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER;

	ResizeTabDialog(tab, dialog, tabx, taby, borderx, bordery);

	GetWindowRect(list, &r);
	p.x = r.left;
	p.y = r.top;
	ScreenToClient(tab,&p);
	int newx = tabx - borderx - p.x;
	int newy = taby - bordery - p.y;
	SetWindowPos(list, 0, 0, 0, newx, newy, resize_flags);

	int sum = 0;
	for(int i=0 ; i<last_column ; ++i)
		sum += ListView_GetColumnWidth(list, i);

	if( sum < newx - 30 )
		ListView_SetColumnWidth(list, last_column, newx-sum-25);
}


void SetSizeOfVariablesList(HWND tab, int tabx, int taby, int borderx, int bordery)
{
	SetSizeOfFunctionVariableList(tab, tabx, taby, borderx, bordery, TabWindowFunctions::tab_variables, IDC_VARIABLES_LIST, 1);
}

void SetSizeOfFunctionsList(HWND tab, int tabx, int taby, int borderx, int bordery)
{
	SetSizeOfFunctionVariableList(tab, tabx, taby, borderx, bordery, TabWindowFunctions::tab_functions, IDC_FUNCTIONS_LIST, 2);
}


void SetSizeOfConvertingLists(HWND tab, int tabx, int taby, int borderx, int bordery)
{
RECT r_edit, r;
POINT p;

	HWND dialog = GetPrgRes()->GetTabWindow(tab_convert);
	HWND list1  = GetDlgItem(dialog, IDC_LIST_UNIT_TYPE);
	HWND info_edit = GetDlgItem(dialog, IDC_EDIT_OUTPUT_INFO);
	HWND check_dynamic_output = GetDlgItem(dialog, IDC_COMBO_DYNAMIC_OUTPUT);
	int move_flags = SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER;
	int move_resize_flags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER;

	POINT dialog_new_size = ResizeTabDialog(tab, dialog, tabx, taby, borderx, bordery);

	GetWindowRect(info_edit, &r_edit);

	GetWindowRect(list1, &r);
	p.x = r.left;
	p.y = r.top;
	ScreenToClient(dialog,&p);

	GetWindowRect(dialog, &r);
	int newy = (r.bottom-r.top) - bordery - (r_edit.bottom-r_edit.top) - p.y;
	int list1_x  = p.x;

	int list2a_cx = tabx/7;
	int list2b_cx = tabx/5;
	int list3a_cx = tabx/7;
	int list3b_cx = tabx/5;

	int marginx = 4;
	int list1_cx = tabx - list2a_cx - list2b_cx - list3a_cx - list3b_cx - marginx*4 - borderx - 8;

	SetWindowPos(list1, 0, list1_x, p.y, list1_cx, newy, move_resize_flags);

	HWND list2a  = GetDlgItem(dialog, IDC_LIST_UNIT_INPUT1);
	HWND list2b  = GetDlgItem(dialog, IDC_LIST_UNIT_INPUT2);
	HWND list3a  = GetDlgItem(dialog, IDC_LIST_UNIT_OUTPUT1);
	HWND list3b  = GetDlgItem(dialog, IDC_LIST_UNIT_OUTPUT2);

	int list2a_x = list1_x + list1_cx + marginx;
	int list2b_x = list2a_x + list2a_cx + marginx;
	int list3a_x = list2b_x + list2b_cx + marginx;
	int list3b_x = list3a_x + list3a_cx + marginx;

	SetWindowPos(list2a, 0, list2a_x, p.y, list2a_cx, newy, move_resize_flags);
	SetWindowPos(list2b, 0, list2b_x, p.y, list2b_cx, newy, move_resize_flags);
	SetWindowPos(list3a, 0, list3a_x, p.y, list3a_cx, newy, move_resize_flags);
	SetWindowPos(list3b, 0, list3b_x, p.y, list3b_cx, newy, move_resize_flags);

	newy = dialog_new_size.y -(r_edit.bottom-r_edit.top);
	int info_edit_cx = list1_cx + list2a_cx + list2b_cx + marginx*2;
	SetWindowPos(info_edit, 0, list1_x, newy, info_edit_cx, (r_edit.bottom-r_edit.top), move_resize_flags);

	SetWindowPos(check_dynamic_output, 0, list1_x + info_edit_cx + marginx, newy, 0, 0, move_flags);

	HWND static_type = GetDlgItem(dialog, IDC_STATIC_UNIT_CONVERSION);
	GetWindowRect(static_type, &r);
	p.x = r.left;
	p.y = r.top;
	ScreenToClient(dialog, &p);
	SetWindowPos(static_type, 0, list1_x, p.y, 0, 0, move_flags);

	HWND static_input = GetDlgItem(dialog, IDC_STATIC_UNIT_INPUT);
	SetWindowPos(static_input, 0, list2a_x, p.y, 0, 0, move_flags);

	HWND static_output = GetDlgItem(dialog, IDC_STATIC_UNIT_OUTPUT);
	SetWindowPos(static_output, 0, list3a_x, p.y, 0, 0, move_flags);

	// setting width of colums inside the lists
	int vscroll_cx = GetSystemMetrics(SM_CXVSCROLL) + borderx;
	ListView_SetColumnWidth(list1, 0, list1_cx - vscroll_cx);
	ListView_SetColumnWidth(list2a, 0, list2a_cx - vscroll_cx);
	ListView_SetColumnWidth(list2b, 0, list2b_cx - vscroll_cx);
	ListView_SetColumnWidth(list3a, 0, list3a_cx - vscroll_cx);
	ListView_SetColumnWidth(list3b, 0, list3b_cx - vscroll_cx);
}


void SetSizeOfDialogs()
{
	const int cy_std = 19;
	int cy_caption = GetSystemMetrics(SM_CYCAPTION);

	if( cy_caption <= cy_std )
		return;

	int len = GetPrgRes()->HowManyTabWindows();
	RECT r;

	for(int i = 0 ; i<len ; ++i )
	{
		HWND dialog = GetPrgRes()->GetTabWindow(i);
		GetWindowRect( dialog, &r);
		SetWindowPos( dialog,0, 0,0, r.right-r.left, r.bottom-r.top+(cy_caption-cy_std),
					SWP_NOZORDER |SWP_NOMOVE);

		GetWindowRect( dialog, &r);
	}
}

} // namespace TabWindowFunctions



