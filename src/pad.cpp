/*
 * This file is a part of TTCalc - a mathematical calculator
 * and is distributed under the (new) BSD licence.
 * Author: Tomasz Sowa <t.sowa@ttmath.org>
 */

/* 
 * Copyright (c) 2006-2010, Tomasz Sowa
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

#include <windows.h>
#include <ttmath/ttmath.h>
#include <shlobj.h>
#include <fstream>
#include "programresources.h"
#include "resource.h"
#include "messages.h"
#include "bigtypes.h"
#include "pad.h"


namespace Pad
{
HWND edit;
WNDPROC old_edit_proc;
std::string parse_string;
int precision;
HFONT font;

int  base_input;
int  base_output;
bool always_scientific;
int  when_scientific;
int  rounding;
bool remove_zeroes;
int  angle_deg_rad_grad;

char decimal_point;
char grouping;
char input_comma1;
char input_comma2;
char param_sep;

int grouping_digits;

Languages::Country country;

int last_variables_id;
int last_functions_id;
ttmath::Objects * variables;
ttmath::Objects * functions;

ttmath::ErrorCode code;
bool calculated;

std::string res, res2;
std::string file_name;


#ifndef TTCALC_PORTABLE
	ttmath::Parser<TTMathBig1> parser1;
	ttmath::Parser<TTMathBig2> parser2;
	ttmath::Parser<TTMathBig3> parser3;
#else
	ttmath::Parser<TTMathBig1> parser1;
#endif


void PutChars(const char * str)
{
	SendMessage(edit, EM_REPLACESEL, true, (LPARAM)str);
}


void PutChars(std::string & str)
{
	PutChars(str.c_str());
}


void PutOverflowMsg()
{
	PutChars(GetPrgRes()->GetLanguages()->ErrorMessage(country, ttmath::err_overflow));
	PutChars("\r\n");
}


template<typename ValueType>
void PutResultFromParser(ttmath::Parser<ValueType> & matparser)
{
unsigned int i = 0;

	res.clear();
	res2.clear();

	for(i=0 ; i<matparser.stack.size() ; ++i)
	{
		try
		{
			ValueType result = matparser.stack[i].value;

			ttmath::Conv conv;

			conv.base         = base_output;
			conv.scient       = always_scientific;
			conv.scient_from  = when_scientific;
			conv.round        = rounding;
			conv.trim_zeroes  = remove_zeroes;
			conv.comma        = decimal_point;
			conv.group        = grouping;
			conv.group_digits = grouping_digits;
			
			if( result.ToString(res2, conv) )
				res2 = GetPrgRes()->GetLanguages()->GuiMessage(country, Languages::overflow_during_printing);
		}
		catch(...)
		{
			res2 = GetPrgRes()->GetLanguages()->ErrorMessage(country, ttmath::err_internal_error);
		}

		res += res2;

		if( i < matparser.stack.size()-1 )
			res += "\r\n";
	}

	res += ' ';
	PutChars(res);
}


void PutResult()
{
	#ifndef TTCALC_PORTABLE

		switch( precision )
		{
		case 0:
			PutResultFromParser(parser1);
			break;

		case 1:
			PutResultFromParser(parser2);
			break;

		default:
			PutResultFromParser(parser3);
			break;
		}

	#else

		PutResultFromParser(parser1);

	#endif
}



// line - index of a line -- as you see it on the edit control
// (if the text is wrapped then the line is larger)
void GetParseString(const char * buf, int line)
{
int i;

	i = SendMessage(edit, EM_LINEINDEX, line, 0);
	// first_line_char takes into consideration \r\n characters

	if( i < 0 )
		// something wrong
		return;

	// looking for the beginning of the line (lines can be wrapped)
	for( ; i>0 && buf[i-1]!=10 ; --i);

	// copying the whole line
	for( ; buf[i]!=0 && buf[i]!='\r' && buf[i]!='\n' ; ++i )
		parse_string += buf[i];
}


void GetParseString()
{
int line;

	parse_string.clear();

	// index of a last line to parse (-1 because the ENTER has been put)
	line = SendMessage(edit, EM_LINEFROMCHAR, -1, 0) - 1;

	if( line < 0 )
		// something wrong
		return;


	HLOCAL handle = (HLOCAL)SendMessage(edit, EM_GETHANDLE, 0, 0);

	if( handle == 0 )
		// something wrong
		return;

	const char * buf = (const char*)LocalLock(handle);
	
	if( buf )
		GetParseString(buf, line);

	LocalUnlock(handle);
}


template<class ValueType>
void ParseString(ttmath::Parser<ValueType> & matparser)
{
	matparser.SetBase(base_input);
	matparser.SetDegRadGrad(angle_deg_rad_grad);
	matparser.SetComma(input_comma1, input_comma2);
	matparser.SetGroup(grouping);
	matparser.SetParamSep(param_sep);

	matparser.SetVariables(variables);
	matparser.SetFunctions(functions);

	code = matparser.Parse(parse_string.c_str());

	calculated = matparser.Calculated();
}


void SetParameters()
{
	base_input         = GetPrgRes()->GetBaseInput();
	base_output        = GetPrgRes()->GetBaseOutput();
	always_scientific  = GetPrgRes()->GetDisplayAlwaysScientific();
	when_scientific    = GetPrgRes()->GetDisplayWhenScientific();
	rounding           = GetPrgRes()->GetDisplayRounding();
	precision          = GetPrgRes()->GetPrecision();
	remove_zeroes      = GetPrgRes()->GetRemovingZeroes();
	angle_deg_rad_grad = GetPrgRes()->GetDegRadGrad();
	country            = GetPrgRes()->GetLanguages()->GetCurrentLanguage();
	decimal_point      = GetPrgRes()->GetDecimalPointChar();
	grouping           = GetPrgRes()->GetGroupingChar();
	grouping_digits    = GetPrgRes()->GetGroupingDigits();
	param_sep          = GetPrgRes()->GetParamSepChar();

	GetPrgRes()->GetInputDecimalPointChar(&input_comma1, &input_comma2);

	variables = GetPrgRes()->GetVariables();
	functions = GetPrgRes()->GetFunctions();
}

	
void ParseString()
{
	if( parse_string.empty() )
		return;
	
	SetParameters();

	try
	{
	#ifndef TTCALC_PORTABLE

		switch( precision )
		{
		case 0:
			ParseString(parser1);
			break;

		case 1:
			ParseString(parser2);
			break;

		default:
			ParseString(parser3);
			break;
		}

	#else
		ParseString(parser1);
	#endif
	}
	catch(...)
	{
		code = ttmath::err_internal_error;
	}


	if( code==ttmath::err_ok && calculated )
		PutResult();
	else
	if( code==ttmath::err_overflow )
		PutOverflowMsg();
}


LRESULT EditReturnPressed(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
DWORD sel_start, sel_end;

	SendMessage(edit, EM_GETSEL, (WPARAM)&sel_start, (LPARAM)&sel_end);
		
	// putting the ENTER
	LRESULT res = CallWindowProc(old_edit_proc, hwnd, msg, wParam, lParam);

	if( sel_start != sel_end )
		// something was selected
		return res;

	GetParseString();
	ParseString();

return res;
}



LRESULT EditSubclass(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_CHAR:
		if( wParam == VK_RETURN )
		{
			return EditReturnPressed(hwnd, msg, wParam, lParam);
		}
		break;
	}

return CallWindowProc(old_edit_proc, hwnd, msg, wParam, lParam);
}




inline void SetMenuLanguageItem(HMENU menu, DWORD menu_id, Languages::GuiMsg msg)
{
Languages * pLang = GetPrgRes()->GetLanguages();

	ModifyMenu(menu, menu_id, MF_BYCOMMAND|MF_STRING, menu_id, pLang->GuiMessage(msg) );
}



void SetPadMenuLanguage(HWND hWnd)
{
HMENU menu = GetMenu(hWnd);

	if( !menu )
		return;

	MENUITEMINFO mi;
	mi.cbSize = sizeof(mi);
	mi.fMask = MIIM_TYPE;
	mi.fType = MFT_STRING;

	// on popup menus we're using indexes because they don't want to work correctly
	// with the identifiers 
	mi.dwTypeData = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::pad_menu_file) );
	SetMenuItemInfo(menu, MENUPAD_FILE_INDEX, true, &mi);

	mi.dwTypeData = const_cast<char*>( GetPrgRes()->GetLanguages()->GuiMessage(Languages::pad_menu_edit) );
	SetMenuItemInfo(menu, MENUPAD_EDIT_INDEX, true, &mi);

	SetMenuLanguageItem(menu, MENUPAD_FILE_OPEN,		Languages::pad_menu_file_open);
	SetMenuLanguageItem(menu, MENUPAD_FILE_SAVE,		Languages::pad_menu_file_saveas);
	SetMenuLanguageItem(menu, MENUPAD_FILE_CLOSE,		Languages::pad_menu_file_close);
	SetMenuLanguageItem(menu, MENUPAD_EDIT_UNDO,		Languages::pad_menu_edit_undo);
	SetMenuLanguageItem(menu, MENUPAD_EDIT_CUT,			Languages::pad_menu_edit_cut);
	SetMenuLanguageItem(menu, MENUPAD_EDIT_COPY,		Languages::pad_menu_edit_copy);
	SetMenuLanguageItem(menu, MENUPAD_EDIT_PASTE,		Languages::pad_menu_edit_paste);
	SetMenuLanguageItem(menu, MENUPAD_EDIT_DEL,			Languages::pad_menu_edit_del);
	SetMenuLanguageItem(menu, MENUPAD_EDIT_SELECTALL,	Languages::pad_menu_edit_select_all);

	DrawMenuBar(hWnd);
}


LRESULT PadCreate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	edit = CreateWindow("edit", "",
			WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
			0, 0, 100, 100, hwnd, 0, GetPrgRes()->GetInstance(), 0);

	if( !edit )
		return 0;

	GetPrgRes()->SetPadEdit(edit);

	old_edit_proc = (WNDPROC)SetWindowLong(edit, GWL_WNDPROC, (LONG)EditSubclass);

	// 65535 - 64KB 
	// we're using some kind of messages which operates only on 64KB
	SendMessage(edit, EM_SETLIMITTEXT, 65535 - 5, 0);

	if( font != 0 )
		SendMessage(edit, WM_SETFONT, (WPARAM)font, 0);

	SetPadLanguage();

return 0;
}


LRESULT PadMove(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
RECT r;

	GetWindowRect(hwnd,&r);

	GetPrgRes()->SetPadXPos( r.left );
	GetPrgRes()->SetPadYPos( r.top );

return 0;
}


LRESULT PadSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
// client coordinates
int cx = LOWORD(lParam);
int cy = HIWORD(lParam);
//int fwSizeType = int(wParam);
RECT r;


	//if( fwSizeType == SIZE_MAXIMIZED )
	//	GetPrgRes()->SetPadMaximized(true);
	//else
	//if( fwSizeType == SIZE_RESTORED )
	GetPrgRes()->SetPadMaximized(false);

	MoveWindow(edit, 0, 0, cx, cy, true);

	if( fwSizeType != SIZE_MINIMIZED && fwSizeType != SIZE_MAXIMIZED )
	{
		GetWindowRect(hwnd, &r);
		GetPrgRes()->SetPadXSize( r.right - r.left );
		GetPrgRes()->SetPadYSize( r.bottom - r.top );
	}

return 0;
}



LRESULT PadClose(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	GetPrgRes()->ShowPad( false );

return 0;
}


LRESULT PadFocus(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SetFocus(edit);

return 0;
}

bool OpenDialog(HWND hwnd)
{
OPENFILENAME o;
char buf[MAX_PATH];

	buf[0] = 0;

	o.lStructSize       = sizeof(o);
	o.hwndOwner         = hwnd;
	o.hInstance         = GetPrgRes()->GetInstance();
	o.lpstrFilter       = "*.*\0*.*\0";
	o.lpstrCustomFilter = 0;
	o.nMaxCustFilter    = 0;
	o.nFilterIndex      = 1;
	o.lpstrFile         = buf;
	o.nMaxFile          = MAX_PATH;
	o.lpstrFileTitle    = 0;
	o.nMaxFileTitle     = 0;
	o.lpstrInitialDir   = 0;
	o.lpstrTitle        = 0;
	o.Flags             = OFN_PATHMUSTEXIST;
	o.nFileOffset       = 0;
	o.nFileExtension    = 0;
	o.lpstrDefExt       = 0;
	o.lCustData         = 0;
	o.lpfnHook          = 0;
	o.lpTemplateName    = 0;


	if( GetOpenFileName(&o) )
	{
		file_name = buf;
		return true;
	}

return false;
}


// testing files which have only one 10 at the end of a line (unix mode)
void CorrectUnixMode()
{
bool was_r = false;

	for(size_t i=0 ; i<res.size() ; ++i)
	{
		if( res[i]==10 && !was_r )
		{
			res.insert(res.begin()+i, 13);
			++i;
		}
		else
		{
			was_r = res[i] == 13;
		}
	}
}


void LoadFromFile(HWND hwnd, std::ifstream & file)
{
char buf[64];
std::streamsize size;

	res.clear();

	do
	{
		file.read(buf, sizeof(buf)-1);

		size = file.gcount();
		buf[size] = 0;
		res += buf;
	}
	while( !file.eof() );

	CorrectUnixMode();
	SetWindowText(edit, res.c_str());
}




LRESULT PadOpen(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( !OpenDialog(hwnd) )
		return 0;


	std::ifstream file(file_name.c_str(), std::ios_base::in | std::ios_base::binary);
	Languages * lang = GetPrgRes()->GetLanguages();

	if( !file )
	{
		MessageBox(hwnd, lang->GuiMessage(Languages::cannot_open_file),
						 lang->GuiMessage(Languages::message_box_error_caption), MB_ICONERROR);
		return 0;
	}

	file.seekg(0, std::ios::end);
	std::ifstream::pos_type pos = file.tellg();

	if( pos > 65530 )
	{
		MessageBox(hwnd, lang->GuiMessage(Languages::file_too_long),
						 lang->GuiMessage(Languages::message_box_error_caption), MB_ICONERROR);
		return 0;
	}

	file.seekg(0);
	LoadFromFile(hwnd, file);

	file.close();

return 0;
}




bool SaveDialog(HWND hwnd)
{
OPENFILENAME o;
char buf[MAX_PATH];

	sprintf(buf, "ttcalc.txt");

	o.lStructSize       = sizeof(o);
	o.hwndOwner         = hwnd;
	o.hInstance         = GetPrgRes()->GetInstance();
	o.lpstrFilter       = "*.*\0*.*\0";
	o.lpstrCustomFilter = 0;
	o.nMaxCustFilter    = 0;
	o.nFilterIndex      = 1;
	o.lpstrFile         = buf;
	o.nMaxFile          = MAX_PATH;
	o.lpstrFileTitle    = 0;
	o.nMaxFileTitle     = 0;
	o.lpstrInitialDir   = 0;
	o.lpstrTitle        = 0;
	o.Flags             = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
	o.nFileOffset       = 0;
	o.nFileExtension    = 0;
	o.lpstrDefExt       = 0;
	o.lCustData         = 0;
	o.lpfnHook          = 0;
	o.lpTemplateName    = 0;


	if( GetSaveFileName(&o) )
	{
		file_name = buf;
		return true;
	}

return false;
}


void SaveBuffer(HWND hwnd, const char * buf)
{
	std::ofstream file(file_name.c_str(), std::ios_base::out | std::ios_base::binary);

	if( !file )
	{
		Languages * lang = GetPrgRes()->GetLanguages();
		MessageBox(hwnd, lang->GuiMessage(Languages::cannot_save_file),
						 lang->GuiMessage(Languages::message_box_error_caption), MB_ICONERROR);
		return;
	}

	for( ; *buf ; ++buf )
		file.put(*buf);

	file.close();
}



LRESULT PadSave(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( !SaveDialog(hwnd) )
		return 0;

	Languages * lang = GetPrgRes()->GetLanguages();	
	HLOCAL handle = (HLOCAL)SendMessage(edit, EM_GETHANDLE, 0, 0);

	if( handle == 0 )
	{
		MessageBox(hwnd, lang->GuiMessage(Languages::other_error),
						 lang->GuiMessage(Languages::message_box_error_caption), MB_ICONERROR);
		return 0;
	}

	const char * buf = (const char*)LocalLock(handle);
	
	if( buf )
		SaveBuffer(hwnd, buf);
	else
		MessageBox(hwnd, lang->GuiMessage(Languages::other_error),
						 lang->GuiMessage(Languages::message_box_error_caption), MB_ICONERROR);

	LocalUnlock(handle);

return 0;
}


LRESULT PadNew(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SetWindowText(edit, "");

return 0;
}


LRESULT PadUndo(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SendMessage(edit, WM_UNDO, 0, 0);

return 0;
}


/*
LRESULT PadCut(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SendMessage(edit, WM_CUT, 0, 0);

return 0;
}


LRESULT PadCopy(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SendMessage(edit, WM_COPY, 0, 0);

return 0;
}


LRESULT PadPaste(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SendMessage(edit, WM_PASTE, 0, 0);

return 0;
}
*/

LRESULT PadDel(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SendMessage(edit, EM_REPLACESEL, true, (LPARAM)"");

return 0;
}


LRESULT PadSelectAll(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SendMessage(edit, EM_SETSEL, 0, -1);

return 0;
}


void PadInitMenuEdit(HMENU menu)
{
DWORD sel_start, sel_end;
bool can_undo = SendMessage(edit, EM_CANUNDO, 0 ,0);

	if( can_undo )
		EnableMenuItem(menu,MENUPAD_EDIT_UNDO,MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(menu,MENUPAD_EDIT_UNDO,MF_BYCOMMAND | MF_GRAYED);


	SendMessage(edit, EM_GETSEL, (WPARAM)&sel_start, (LPARAM)&sel_end);

	if( sel_start == sel_end )
	{
		EnableMenuItem(menu,MENUPAD_EDIT_CUT, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(menu,MENUPAD_EDIT_COPY,MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(menu,MENUPAD_EDIT_DEL, MF_BYCOMMAND | MF_GRAYED);
	}
	else
	{
		EnableMenuItem(menu,MENUPAD_EDIT_CUT, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(menu,MENUPAD_EDIT_COPY,MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(menu,MENUPAD_EDIT_DEL, MF_BYCOMMAND | MF_ENABLED);
	}

	if( IsClipboardFormatAvailable(CF_TEXT) )
		EnableMenuItem(menu,MENUPAD_EDIT_PASTE,MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(menu,MENUPAD_EDIT_PASTE,MF_BYCOMMAND | MF_GRAYED);
}


LRESULT PadInitMenuPopUp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
HMENU menu = (HMENU) wParam;
int index  = LOWORD(lParam);

	if( index == MENUPAD_EDIT_INDEX )
		PadInitMenuEdit(menu);

return 0;
}



void CreatePadCommandTable(Messages<LRESULT> & messages)
{
	messages.Associate(MENUPAD_FILE_NEW,	PadNew);
	messages.Associate(MENUPAD_FILE_OPEN,	PadOpen);
	messages.Associate(MENUPAD_FILE_SAVE,	PadSave);
	messages.Associate(MENUPAD_FILE_CLOSE,	PadClose);
	messages.Associate(MENUPAD_EDIT_UNDO,   PadUndo);
	messages.Associate(MENUPAD_EDIT_CUT,    PadCut);
	messages.Associate(MENUPAD_EDIT_COPY,   PadCopy);
	messages.Associate(MENUPAD_EDIT_PASTE,  PadPaste);
	messages.Associate(MENUPAD_EDIT_DEL,  PadDel);
	messages.Associate(MENUPAD_EDIT_SELECTALL, PadSelectAll);
}



LRESULT PadCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
static Messages<LRESULT> messages;
bool method_exists;
LRESULT res;

	if( messages.Empty() )
		// initiation
		CreatePadCommandTable(messages);

	res = messages.Call(LOWORD(wParam), hwnd, msg, wParam, lParam, &method_exists);

	if( !method_exists )
		return DefWindowProc(hwnd, msg, wParam, lParam);

return res;
}


void CreatePadMessagesTable(Messages<LRESULT> & messages)
{
	messages.Associate(WM_CREATE,   PadCreate);
	messages.Associate(WM_MOVE,		PadMove);
	messages.Associate(WM_SIZE,     PadSize);
	messages.Associate(WM_CLOSE,    PadClose);
	messages.Associate(WM_SETFOCUS, PadFocus);
	messages.Associate(WM_COMMAND,  PadCommand);
	messages.Associate(WM_INITMENUPOPUP, PadInitMenuPopUp);
}


LRESULT CALLBACK WindowPadProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
static Messages<LRESULT> messages;
bool method_exists;
LRESULT res;

	if( messages.Empty() )
		// initiation
		CreatePadMessagesTable(messages);

	res = messages.Call(msg, hwnd, msg, wParam, lParam, &method_exists);

	if( !method_exists )
		return DefWindowProc(hwnd, msg, wParam, lParam);

return res;
}



ATOM RegisterPadClass(const char * ttcalc_pad_class_name)
{
WNDCLASS w;

	w.style         = 0;
	w.lpfnWndProc   = WindowPadProc;
	w.cbClsExtra    = 0;
	w.cbWndExtra    = 0;
	w.hInstance     = GetPrgRes()->GetInstance();
	w.hIcon         = LoadIcon(GetPrgRes()->GetInstance(), MAKEINTRESOURCE(IDI_ICON2));
	w.hCursor       = 0;
	w.hbrBackground = (HBRUSH)(COLOR_BACKGROUND+1);
	w.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU2);
	w.lpszClassName = ttcalc_pad_class_name;

return RegisterClass(&w);
}


} // namespace Pad





bool CreatePadWindow()
{
using namespace Pad;

static char ttcalc_pad_class_name[] = "TTCalcPadWindow";
ATOM a = RegisterPadClass(ttcalc_pad_class_name);

	if( a == 0 )
		return false;

	bool pad_maximized_from_file = GetPrgRes()->GetPadMaximized();
	// CreateWindow() would change maximizing

	Languages * pl = GetPrgRes()->GetLanguages();

	font = CreateFont(0, 0, 0, 0, FW_NORMAL, false, false, false,
					  ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					  DEFAULT_QUALITY, DEFAULT_PITCH, "Ms Shell Dlg");


	HWND pad = CreateWindowEx(WS_EX_CLIENTEDGE, ttcalc_pad_class_name,
				pl->GuiMessage(Languages::pad_title),
				WS_OVERLAPPEDWINDOW | WS_POPUPWINDOW ,
				GetPrgRes()->GetPadXPos(), GetPrgRes()->GetPadYPos(),
				GetPrgRes()->GetPadXSize(), GetPrgRes()->GetPadYSize(),
				0 /*GetPrgRes()->GetMainWindow()*/, 0, GetPrgRes()->GetInstance(), 0);

	GetPrgRes()->SetPadWindow(pad);
	SetPadLanguage();
	GetPrgRes()->SetPadMaximized( pad_maximized_from_file );
	GetPrgRes()->ShowPad( GetPrgRes()->IsPadVisible() );

return pad!=0 && edit!=0;
}


void DestroyPadWindow()
{
using namespace Pad;

	DeleteObject(font);
}


void SetPadLanguage()
{
using namespace Pad;

	HWND pad = GetPrgRes()->GetPadWindow();
	Languages * pl = GetPrgRes()->GetLanguages();

	SetPadMenuLanguage(pad);
	SetWindowText(pad, pl->GuiMessage(Languages::pad_title));
}

