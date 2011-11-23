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
#include "programresources.h"



namespace ProgramResourcesGlobal
{
	ProgramResources program_resource;
}


ProgramResources * GetPrgRes()
{
	return &ProgramResourcesGlobal::program_resource;
}






void ProgramResources::SetXPos(int x)
{
	x_pos = x;
}


void ProgramResources::SetYPos(int y)
{
	y_pos = y;
}


int ProgramResources::GetXPos()
{
	return x_pos;
}


int ProgramResources::GetYPos()
{
	return y_pos;
}


void ProgramResources::SetXSize(int x)
{
	x_size = x;
}

void ProgramResources::SetYSize(int y)
{
	y_size = y;
}

int ProgramResources::GetXSize()
{
	return x_size;
}

int ProgramResources::GetYSize()
{
	return y_size;
}


void ProgramResources::SetPadXPos(int x)
{
	pad_x_pos = x;
}

void ProgramResources::SetPadYPos(int y)
{
	pad_y_pos = y;
}

int ProgramResources::GetPadXPos()
{
	return pad_x_pos;
}

int ProgramResources::GetPadYPos()
{
	return pad_y_pos;
}

void ProgramResources::SetPadXSize(int x)
{
	pad_x_size = x;
}

void ProgramResources::SetPadYSize(int y)
{
	pad_y_size = y;
}

int ProgramResources::GetPadXSize()
{
	return pad_x_size;
}

int ProgramResources::GetPadYSize()
{
	return pad_y_size;
}




void ProgramResources::SetYSizeNormal(int y)
{
	y_size_normal = y;
}

int ProgramResources::GetYSizeNormal()
{
	return y_size_normal;
}

void ProgramResources::SetYSizeCompact(int y)
{	
	y_size_compact = y;
}

int ProgramResources::GetYSizeCompact()
{
	return y_size_compact;
}

void ProgramResources::SetXSizeMin(int x)
{
	x_size_min = x;
}

int ProgramResources::GetXSizeMin()
{
	return x_size_min;
}


void ProgramResources::SetView(ProgramResources::View v)
{
	view = v;

	if( v == view_normal)
		MoveWindow(main_window, x_pos, y_pos, x_size, y_size, true);
	else
		MoveWindow(main_window, x_pos, y_pos, x_size, y_size_compact, true);
}


ProgramResources::View ProgramResources::GetView()
{
	return view;
}


void ProgramResources::SetMaximized(bool max)
{
	maximized = max;
}


bool ProgramResources::GetMaximized()
{
	return maximized;
}

void ProgramResources::SetPadMaximized(bool max)
{
	pad_maximized = max;
}


bool ProgramResources::GetPadMaximized()
{
	return pad_maximized;
}


void ProgramResources::ShowPad(bool show)
{
	show_pad = show;

	if( show_pad )
	{
		if( pad_maximized )
			ShowWindow(pad_window, SW_SHOWMAXIMIZED);
		else
			ShowWindow(pad_window, SW_SHOWNORMAL);
	}
	else
	{
		ShowWindow(pad_window, SW_HIDE);
	}
}


bool ProgramResources::IsPadVisible()
{
	return show_pad;
}


void ProgramResources::SetAlwaysOnTop(bool always)
{
	always_on_top = always;

	if( always )
		SetWindowPos(main_window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	else
		SetWindowPos(main_window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
}

bool ProgramResources::GetAlwaysOnTop()
{
	return always_on_top;
}


void ProgramResources::SetDisplayRounding(int r)
{
	if( r < -1 )
		r = -1;

	if( r > 99 )
		r = 99;

	display_rounding = r;
}


int ProgramResources::GetDisplayRounding()
{
	return display_rounding;
}


void ProgramResources::SetRemovingZeroes(bool r)
{
	remove_zeroes = r;
}

bool ProgramResources::GetRemovingZeroes()
{
	return remove_zeroes;
}


void ProgramResources::SetDegRadGrad(int angle)
{
	if( angle < 0 || angle > 2 )
		angle = 1;

	angle_deg_rad_grad = angle;
}



int ProgramResources::GetDegRadGrad()
{
	return angle_deg_rad_grad;
}




void ProgramResources::SetDisplayAlwaysScientific(bool a)
{
	display_always_scientific = a;
}

bool ProgramResources::GetDisplayAlwaysScientific()
{
	return display_always_scientific;
}

void ProgramResources::SetDisplayWhenScientific(int w)
{
	if(w<1)
		w = 1;

	if( w > 99 )
		w = 99;

	display_when_scientific = w;
}

int ProgramResources::GetDisplayWhenScientific()
{
	return display_when_scientific;
}



void ProgramResources::SetBaseInput(int i)
{
	if(i<2)
		i = 2;

	if(i>16)
		i = 16;

base_input = i;
}


int ProgramResources::GetBaseInput()
{
	return base_input;
}


void ProgramResources::SetBaseOutput(int i)
{
	if(i<2)
		i = 2;

	if(i>16)
		i = 16;

base_output = i;
}


int ProgramResources::GetBaseOutput()
{
	return base_output;
}


void ProgramResources::SetDecimalPoint(int decimal)
{
	decimal_point = decimal;

	if( decimal_point<0 || decimal_point>1 )
		decimal_point = 0;
}


int ProgramResources::GetDecimalPoint()
{
	return decimal_point;
}


char ProgramResources::GetDecimalPointChar()
{
char c;

	if( GetDecimalPoint() == 0 )
		c = '.';
	else
		c = ',';

return c;
}


char ProgramResources::GetGroupingChar()
{
char c;

	// grouping 
	static char group_table[] = "\0 `'.,"; // 5 items

	int g = GetGrouping();
	
	if( g>=0 && g<=5 )
		c = group_table[g];
	else
		c = 0;

return c;
}


void ProgramResources::GetInputDecimalPointChar(char * comma1, char * comma2)
{
	// input decimal point
	switch( GetInputDecimalPoint() )
	{
	case 0:
		*comma1 = '.';
		*comma2 = ',';
		break;

	case 1:
		*comma1 = '.';
		*comma2 = 0;
		break;

	case 2:
	default:
		*comma1 = ',';
		*comma2 = 0;
		break;
	}
}


char ProgramResources::GetParamSepChar()
{
char c;

	// additional param separator
	static char param_table[] = ";.,"; // 3 items

	int sep = GetParamSep();
	
	if( sep>=0 && sep<=3 )
		c = param_table[sep];
	else
		c = ';';

return c;
}


ttmath::Objects * ProgramResources::GetVariables()
{
	return &variables;
}

ttmath::Objects * ProgramResources::GetFunctions()
{
	return &functions;
}


void ProgramResources::VariablesChanged()
{
	++variables_id;
}


void ProgramResources::FunctionsChanged()
{
	++functions_id;
}


int ProgramResources::GetVariablesId()
{
	return variables_id;
}


int ProgramResources::GetFunctionsId()
{
	return functions_id;
}


Languages * ProgramResources::GetLanguages()
{
	return &languages;
}


Convert * ProgramResources::GetConvert()
{
	return &convert;
}


void ProgramResources::SetInstance(HINSTANCE h)
{
	hInstance = h;
}

HINSTANCE ProgramResources::GetInstance()
{
	return hInstance;
}

void ProgramResources::SetMainWindow(HWND h)
{
	main_window = h;
}

HWND ProgramResources::GetMainWindow()
{
	return main_window;
}

void ProgramResources::SetPadWindow(HWND h)
{
	pad_window = h;
}

HWND ProgramResources::GetPadWindow()
{
	return pad_window;
}

void ProgramResources::SetPadEdit(HWND h)
{
	pad_edit = h;
}

HWND ProgramResources::GetPadEdit()
{
	return pad_edit;
}

void ProgramResources::SetTabWindow(unsigned int id, HWND h)
{
	if( id >= sizeof(tab_window) / sizeof(HWND) )
		return;

tab_window[id] = h;
}

HWND ProgramResources::GetTabWindow(unsigned int id)
{
	if( id >= sizeof(tab_window) / sizeof(HWND) )
		return 0;
	
return tab_window[id];
}

unsigned int ProgramResources::HowManyTabWindows()
{
	return sizeof(tab_window) / sizeof(HWND);
}

char * ProgramResources::GetBuffer()
{
	return buffer;
}

char * ProgramResources::GetBufferTemp()
{
	return buffer_temp;
}

unsigned int ProgramResources::GetBufferSize()
{
	return buffer_size;
}

volatile ThreadController * ProgramResources::GetThreadController()
{
	return &thread_controller;
}

void ProgramResources::SetPrecision(int p)
{
	if( p < 0 )
		p = 0;

	if( p > 2 )
		p = 2;

precision = p;
}

int ProgramResources::GetPrecision()
{
	return precision;
}


void ProgramResources::SetGrouping(int g)
{
	grouping = g;

	if( grouping<0 || grouping>5 )
		g = 0;
}


int ProgramResources::GetGrouping()
{
	return grouping;
}


void ProgramResources::SetGroupingDigits(int digits)
{
	grouping_digits = digits;

	if( grouping_digits < 1 || grouping_digits > 9 )
		grouping_digits = 3; // 3 by default
}


int ProgramResources::GetGroupingDigits()
{
	return grouping_digits;
}



void ProgramResources::SetInputDecimalPoint(int p)
{
	input_decimal_point = p;

	if( input_decimal_point<0 || input_decimal_point>2 )
		decimal_point = 0;
}


int ProgramResources::GetInputDecimalPoint()
{
	return input_decimal_point;
}


void ProgramResources::SetParamSep(int s)
{
	param_sep = s;

	if( param_sep<0 || param_sep>2 )
		param_sep = 0;
}


int ProgramResources::GetParamSep()
{
	return param_sep;
}


void ProgramResources::SetCheckUpdateStartup(bool s)
{
	check_update_startup = s;
}


bool ProgramResources::GetCheckUpdateStartup()
{
	return check_update_startup;
}


void ProgramResources::SetUpdateExists(bool e)
{
	update_exists = e;
}


bool ProgramResources::GetUpdateExists()
{
	return update_exists;
}


void ProgramResources::SetLastUpdate(time_t t)
{
	last_update = t;
}


time_t ProgramResources::GetLastUpdate()
{
	return last_update;
}



ProgramResources::ProgramResources()
{
	// if you want to change the input buffer you have to also change
	// ParserManager::ParserManager() : buffer_len(20480) in parsermanager.cpp
	buffer_size = 20480;

	buffer = new char[buffer_size];
	buffer[0] = 0;

	buffer_temp = new char[buffer_size];
	buffer_temp[0] = 0;

	always_on_top = false;
	view = view_normal;

	maximized = false;

	precision = 0;

	hInstance = 0;
	main_window = 0;
	pad_window = 0;
	pad_edit = 0;

	base_input = 10;
	base_output = 10;

	display_always_scientific = false;
	display_when_scientific   = 8;
	display_rounding          = -1;
	remove_zeroes             = true;

	for(int i=HowManyTabWindows()-1 ; i!=-1 ; --i)
		tab_window[i] = 0;

	x_pos  = 0;
	y_pos  = 0;
	x_size = 100;
	y_size = 100;

	pad_x_pos  = 30;
	pad_y_pos  = 30;
	pad_x_size = 600;
	pad_y_size = 500;
	pad_maximized = false;

	y_size_normal  = 0;
	y_size_compact = 0;
	x_size_min     = 0;
	
	// !!! put into Convert constructor
	#ifdef TTCALC_CONVERT
	convert.Init();
	#endif


	decimal_point = 0; // output decimal point
	angle_deg_rad_grad = 1; // rad is default
	grouping = 0;
	grouping_digits = 3;
	
	input_decimal_point = 0; // dot or comma
	param_sep = 0;

	variables_id = 0;
	functions_id = 0;

	show_pad = false;
	check_update_startup = true;
	update_exists = false; // for dynamic menu

	last_update = 0;
}


ProgramResources::~ProgramResources()
{
	delete [] buffer;
	delete [] buffer_temp;
}



bool ProgramResources::IsWhiteCharacter(int c)
{
	if( c==' ' || c=='\t' || c==13 || c=='\n' )
		return true;

return false;
}

const char * ProgramResources::SkipWhiteCharacters(const char * string)
{
	while( IsWhiteCharacter(*string) )
		++string;

return string;
}

bool ProgramResources::IsDecDigit(int c)
{
	if( c>='0' && c<='9' )
		return true;

return false;
}

bool ProgramResources::SplitFunction(const std::string & input, const char * * name, int * param)
{
const char * pchar = input.c_str();
const int buffer_len = 20;
char buffer[buffer_len];
int i;

	pchar = SkipWhiteCharacters(pchar);

	for(i=0 ; i<buffer_len-1 && IsDecDigit(*pchar) ; ++i, ++pchar)
		buffer[i] = *pchar;

	buffer[i] = 0;

	*param = atoi(buffer);
	if(*param < 0)
		*param = 0;
	else
	if(*param > 9)
		*param = 9;


	//
	while( IsDecDigit(*pchar) );

	pchar = SkipWhiteCharacters(pchar);
	if( *pchar != '|' )
		return false;

	pchar = SkipWhiteCharacters(pchar+1);

	*name = pchar;

return true;
}


const std::string & ProgramResources::GetHelpFile()
{
	return help_file;
}


bool ProgramResources::ReadTextValueFromRegistry(HKEY main_key, const char * sub_key, const char * value, std::string & result)
{
HKEY reg_key;

	if( RegOpenKeyEx( main_key, sub_key, 0, KEY_QUERY_VALUE, &reg_key ) != ERROR_SUCCESS )
		return false;

	bool  status = false;
	DWORD result_type;
	char  result_buffer[300];
	DWORD buffer_size = sizeof(result_buffer) / sizeof(char);

	if( RegQueryValueEx( reg_key, value, 0, &result_type, reinterpret_cast<BYTE*>(result_buffer), &buffer_size ) == ERROR_SUCCESS )
	{
		if( result_type == REG_SZ )
		{
			result = result_buffer;
			status = true;
		}
	}

	RegCloseKey(reg_key);

return status;
}


/*
	setting names of:
	- configuration file
	- help file
*/
void ProgramResources::SetNameOfFiles()
{
char buffer[_MAX_PATH];
static const char conf_name[] = "ttcalc.ini";
static const char help_name[] = "ttcalc.chm";

	// if there'll be an error we assume that the current directory will be used
	configuration_file  = ".\\";
	configuration_file += conf_name;
	help_file           = ".\\";
	help_file          += help_name;

	if( !GetCurrentDirectory(_MAX_PATH, buffer) )
		return;

	help_file  = buffer;
	help_file += '\\';
	help_file += help_name;
	
#ifdef TTCALC_PORTABLE

	configuration_file  = buffer;
	configuration_file += '\\';
	configuration_file += conf_name;

#else

	static const char dir_name[] = "TTCalc";
	std::string app_data;

	// we're trying to read the value "AppData" from registry
	// which can be, for instance, as:
	// "C:\Documents and Settings\user\data application" on WinNT
	// or "C:\Windows\Data Application\" on Win9x
	
	// we don't use SHGetFolderLocation() because it is not available on Win9x

	if( ReadTextValueFromRegistry(
		HKEY_CURRENT_USER,
		"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
		"AppData",
		app_data) )
	{	
		std::string app_dir = app_data + '\\' + dir_name;

		if( CreateDirIfNotExists(app_dir) )
			configuration_file = app_dir + '\\' + conf_name;
	}

#endif
}



/*
	it is used when 'createconfig' parameter has been passed to the ttcalc program	
*/
void ProgramResources::SetConfigName()
{
	configuration_file = "ttcalc.ini";
}




bool ProgramResources::CreateDirIfNotExists(const std::string & app_dir)
{
bool success = false;

	if( SetCurrentDirectory( app_dir.c_str() ) )
	{
		success = true;
	}
	else
	if( CreateDirectory(app_dir.c_str(), 0) )
	{
		success = true;
	}

return success;
}



void ProgramResources::CheckCoordinates()
{
	if( x_pos < -50 )
		x_pos = 0;

	if( y_pos < -50 )
		y_pos = 0;

	if( x_size < 100 )
		x_size = 100;

	if( y_size < 30 )
		y_size = 30;

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	if( x_size > cx+10 )
		x_size = cx;

	if( y_size > cy+10 )
		y_size = cy;

	if( x_pos > cx-50 )
		x_pos = cx - x_size;

	if( y_pos > cy-50 )
		y_pos = cy - y_size;
}

void ProgramResources::CheckPadCoordinates()
{
	if( pad_x_pos < -50 )
		pad_x_pos = 0;

	if( pad_y_pos < -50 )
		pad_y_pos = 0;

	if( pad_x_size < 30 )
		pad_x_size = 600;

	if( pad_y_size < 30 )
		pad_y_size = 500;

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	if( pad_x_size > cx+10 )
		pad_x_size = cx;

	if( pad_y_size > cy+10 )
		pad_y_size = cy;

	if( pad_x_pos > cx-50 )
		pad_x_pos = cx - pad_x_size;

	if( pad_y_pos > cy-50 )
		pad_y_pos = cy - pad_y_size;
}


void ProgramResources::AddVariablesFunctions( IniParser::Section & temp_variables,
											  IniParser::Section & temp_functions,
											  bool if_not_exist)
{
IniParser::Section::iterator ic;

	// we're adding variables

	if( !if_not_exist )
	{
		variables.Clear();
		VariablesChanged();
	}

	for( ic = temp_variables.begin() ; ic!=temp_variables.end() ; ++ic )
		if( !if_not_exist || !variables.IsDefined(ic->first) )
		{
			variables.Add(ic->first, ic->second);
			VariablesChanged();
		}


	// we're adding functions

	if( !if_not_exist )
	{
		functions.Clear();
		FunctionsChanged();
	}

	for( ic = temp_functions.begin() ; ic!=temp_functions.end() ; ++ic )
	{
		const char * name;
		int param;

		if( !if_not_exist || !functions.IsDefined(ic->first) )
			if( SplitFunction(ic->second, &name, &param) )
			{
				functions.Add(ic->first, name, param);
				FunctionsChanged();
			}
	}
}


IniParser::Error ProgramResources::ReadVariablesFunctionsFromFile()
{
IniParser iparser;
IniParser::Section temp_variables, temp_functions;

	iparser.ConvertValueToSmallLetters(false);
	iparser.SectionCaseSensitive(false);
	
	// we have variables and functions case-sensitive
	iparser.PatternCaseSensitive(true);

	iparser.Associate( "variables", &temp_variables );
	iparser.Associate( "functions", &temp_functions );

	bad_line = -1;
	IniParser::Error err = iparser.ReadFromFile( configuration_file.c_str() );

	if( err == IniParser::err_cant_open_file )
		return err;

	AddVariablesFunctions(temp_variables, temp_functions, true);

	if( err != IniParser::err_ok )
		bad_line = iparser.GetBadLine();

return err;
}




IniParser::Error ProgramResources::ReadFromFile()
{
IniParser iparser;
IniParser::Section temp_variables, temp_functions;
IniParser::Section::iterator ic;
std::string ini_value[30];
std::string language_setup;

	iparser.ConvertValueToSmallLetters(false);
	iparser.SectionCaseSensitive(false);
	
	// we have variables and functions case-sensitive
	iparser.PatternCaseSensitive(true);

	iparser.Associate( "global|language.setup",			&language_setup );
	iparser.Associate( "global|always.on.top",			&ini_value[0] );
	iparser.Associate( "global|view",					&ini_value[1] );
	iparser.Associate( "global|maximized",				&ini_value[2] );
	iparser.Associate( "global|x",						&ini_value[3] );
	iparser.Associate( "global|y",						&ini_value[4] );
	iparser.Associate( "global|size.x",					&ini_value[5] );
	iparser.Associate( "global|precision",				&ini_value[6] );
	iparser.Associate( "global|disp.input",				&ini_value[7] );
	iparser.Associate( "global|disp.output",			&ini_value[8] );
	iparser.Associate( "global|disp.alw.scientific",	&ini_value[9] );
	iparser.Associate( "global|disp.when.scientific",	&ini_value[10] );
	iparser.Associate( "global|disp.rounding",			&ini_value[11] );
	iparser.Associate( "global|size.y",					&ini_value[12] );
	iparser.Associate( "global|language",				&ini_value[13] );
	iparser.Associate( "global|disp.output_comma",		&ini_value[14] );
	iparser.Associate( "global|disp.remove.zeroes",		&ini_value[15] );
	iparser.Associate( "global|disp.deg_rad_grad",		&ini_value[16] );
	iparser.Associate( "global|pad.x",					&ini_value[17] );
	iparser.Associate( "global|pad.y",					&ini_value[18] );
	iparser.Associate( "global|pad.size.x",				&ini_value[19] );
	iparser.Associate( "global|pad.size.y",				&ini_value[20] );
	iparser.Associate( "global|pad.maximized",			&ini_value[21] );
	iparser.Associate( "global|pad",					&ini_value[22] );
	iparser.Associate( "global|disp.grouping",			&ini_value[23] );
	iparser.Associate( "global|disp.input_comma",		&ini_value[24] );
	iparser.Associate( "global|disp.param_sep",			&ini_value[25] );
	iparser.Associate( "global|update.onstartup",		&ini_value[26] );
	iparser.Associate( "global|update.last",			&ini_value[27] );
	iparser.Associate( "global|disp.grouping.digits",	&ini_value[28] );

	iparser.Associate( "variables", &temp_variables );
	iparser.Associate( "functions", &temp_functions );

	bad_line = -1;
	IniParser::Error err = iparser.ReadFromFile( configuration_file.c_str() );

	if( err == IniParser::err_cant_open_file )
		return err;

	if( err != IniParser::err_ok )
		bad_line = iparser.GetBadLine();

	AddVariablesFunctions(temp_variables, temp_functions, false);

	// the first file is created by the installer and has only language.setup option
	// if the rest options are not set then we are not calling ReadGlobalSection()
	if( IsGlobalSectionSet(ini_value, sizeof(ini_value)/sizeof(std::string)) )
		ReadGlobalSection(ini_value);

	if( !language_setup.empty() )
		languages.SelectCurrentLanguage(language_setup); // we don't have to remember this variable

return err;
}


int ProgramResources::Int(const std::string & text)
{
	return atoi(text.c_str());
}


bool ProgramResources::IsGlobalSectionSet(std::string * ini_value, size_t len)
{
	for(size_t i=0 ; i<len ; ++i)
		if( !ini_value[i].empty() )
			return true;

return false;
}



void ProgramResources::ReadGlobalSection(std::string * ini_value)
{
	always_on_top	= Int(ini_value[0]) == 1;
	view			= Int(ini_value[1]) == 0 ? view_normal : view_compact;
	maximized		= Int(ini_value[2]) == 1;
	
	x_pos			= Int( ini_value[3] );
	y_pos			= Int( ini_value[4] );
	x_size			= Int( ini_value[5] );
	y_size			= Int( ini_value[12] );
	CheckCoordinates();

	SetPrecision(	Int(ini_value[6]) );
	SetBaseInput(	Int(ini_value[7]) );
	SetBaseOutput(	Int(ini_value[8]) );

	SetDisplayAlwaysScientific(	Int(ini_value[9]) == 1 );
	SetDisplayWhenScientific(	Int(ini_value[10]) );
	SetDisplayRounding(			Int(ini_value[11]) );

	languages.SetCurrentLanguage((Languages::Country)Int(ini_value[13]));
	SetDecimalPoint( Int(ini_value[14]) );

	// remove zeroes - if there is not in the config then will be true by default
	// (from the constructor)
	if( !ini_value[15].empty() )
		SetRemovingZeroes( Int(ini_value[15]) == 1 );

	SetDegRadGrad( Int(ini_value[16]) );

	pad_x_pos	= Int( ini_value[17] );
	pad_y_pos	= Int( ini_value[18] );
	pad_x_size	= Int( ini_value[19] );
	pad_y_size	= Int( ini_value[20] );
	pad_maximized = Int(ini_value[21]) == 1;
	show_pad      = Int(ini_value[22]) == 1;
	CheckPadCoordinates();

	SetGrouping(          Int(ini_value[23]) );
	SetInputDecimalPoint( Int(ini_value[24]) );
	SetParamSep(          Int(ini_value[25]) );

	// check update once a three day - true by default (if there is not in the config)
	// (default from the constructor)
	if( !ini_value[26].empty() ) 
		check_update_startup = Int(ini_value[26]) == 1;

	last_update = (time_t)atol(ini_value[27].c_str());
	SetGroupingDigits( Int(ini_value[28]) );
}





void ProgramResources::SaveToFile()
{
std::ofstream file( configuration_file.c_str() );

	if( !file )
		return;

	file << "# the configuration file of the program ttcalc\n\n";
	file << "[GLOBAL]\n";

	file << "always.on.top = " << (int)always_on_top	<< std::endl;
	file << "view          = " << (int)view				<< std::endl;
	file << "language      = " << (int)languages.GetCurrentLanguage()
														<< std::endl;
	file << "x             = " << x_pos					<< std::endl;
	file << "y             = " << y_pos					<< std::endl;
	file << "size.x        = " << x_size				<< std::endl;
	file << "size.y        = " << y_size				<< std::endl;
	file << "maximized     = " << (int)maximized		<< std::endl;
	file << "update.onstartup  = " << (int)check_update_startup << std::endl;
	file << "update.last   = " << (long)last_update << std::endl;
	file << "pad           = " << (int)show_pad			<< std::endl;
	file << "pad.x         = " << pad_x_pos				<< std::endl;
	file << "pad.y         = " << pad_y_pos				<< std::endl;
	file << "pad.size.x    = " << pad_x_size			<< std::endl;
	file << "pad.size.y    = " << pad_y_size			<< std::endl;
	file << "pad.maximized = " << (int)pad_maximized	<< std::endl;
	file << "precision     = " << precision				<< std::endl;
	file << "disp.input    = " << base_input			<< std::endl;
	file << "disp.output   = " << base_output			<< std::endl;

	file << "disp.alw.scientific  = " << (int)display_always_scientific	<< std::endl;
	file << "disp.when.scientific = " << display_when_scientific		<< std::endl;
	file << "disp.rounding        = " << display_rounding				<< std::endl;
	file << "disp.remove.zeroes   = " << (int)remove_zeroes				<< std::endl;
	file << "disp.input_comma     = " << input_decimal_point			<< std::endl;
	file << "disp.output_comma    = " << decimal_point					<< std::endl;
	file << "disp.deg_rad_grad    = " << angle_deg_rad_grad				<< std::endl;
	file << "disp.grouping        = " << grouping						<< std::endl;
	file << "disp.grouping.digits = " << grouping_digits				<< std::endl;
	file << "disp.param_sep       = " << param_sep						<< std::endl;


	file << "\n[variables]\n";

	ttmath::Objects::CIterator iv = variables.Begin();

	for( ; iv != variables.End() ; ++iv )
		file << iv->first.c_str() << " = " << iv->second.value.c_str() << std::endl;

	file << "\n[functions]\n";

	iv = functions.Begin();
	for( ; iv != functions.End() ; ++iv )
		file << iv->first.c_str() << " = " << iv->second.param <<
				" | " << iv->second.value.c_str() << std::endl;

}


int ProgramResources::GetBadLine()
{
	return bad_line;
}

