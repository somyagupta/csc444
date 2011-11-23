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

#ifndef headerfileprogramresources
#define headerfileprogramresources

/*!
	\file programresources.h
    \brief various kinds of objects using by the application
*/

#include "compileconfig.h"
#include "iniparser.h"
#include "languages.h"
#include "threadcontroller.h"
#include "convert.h"

#include <ttmath/ttmathobjects.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <windows.h>
#include <ctime>


/*!
	some messages
*/
#define WM_INIT_TAB_VARIABLES	WM_APP
#define WM_INIT_TAB_FUNCTIONS	WM_APP+1
#define WM_INIT_TAB_PRECISION	WM_APP+2
#define WM_INIT_TAB_DISPLAY		WM_APP+3
#define WM_SET_LAST_ERROR		WM_APP+4
#define WM_INIT_TAB_CONVERT		WM_APP+5
#define WM_UPDATE_EXISTS		WM_APP+6


/*!
	you don't have to create an object of this class but instead of it you can
	get the pointer to the object by using 'GetPrgRes()'
	(there'll be only one object of this type)

	you must remember that some of these methods are used by the second thread as well
	and when you want to change a state you must do it between calling 
	StopCalculating() and StartCalculating() in the main thread
	for example if you want to change the precision of displaying you can do as follow:
		(in the main thread)
		GetPrgRes()->GetThreadController()->StopCalculating();
		GetPrgRes()->SetPrecision( new_precision );
		GetPrgRes()->GetThreadController()->StartCalculating();
	
	if you change some variables by using GetVariable() method
	you should call VariablesChanged() in order to inform the second thread that variables have changed
	(variables_id will be increment), the same is for functions (FunctionsChanged() method)

	methods which are used by the second thread:
		GetBuffer(), GetVariables(), GetFunctions(), GetBaseInput(), GetBaseOutput()
		GetDisplayAlwaysScientific(), GetDisplayWhenScientific(), GetDisplayRounding()
		GetCurrentLanguage(), GetPrecision() ...

	and if you're using methods which begin with Set..... you must call it
	between StopCalculating() and StartCalculating()
*/
class ProgramResources
{
public:


	/*!
		types of the main window's view
			view_normal  - standard view with all controls on the dialog
			view_compact - only the input and output edits are shown
	*/
	enum View
	{
		view_normal = 0, view_compact
	};


	/*!
		position of the left top corner of the main window and its size
	*/
	void SetXPos(int x);
	void SetYPos(int y);
	int GetXPos();
	int GetYPos();
	void SetXSize(int x);
	void SetYSize(int y);
	int GetXSize();
	int GetYSize();


	/*!
		position of the left top corner of the pad window and its size
	*/
	void SetPadXPos(int x);
	void SetPadYPos(int y);
	int GetPadXPos();
	int GetPadYPos();
	void SetPadXSize(int x);
	void SetPadYSize(int y);
	int GetPadXSize();
	int GetPadYSize();


	/*!
		size of the main window when 'normal view' is selected
	*/
	void SetYSizeNormal(int y);
	int GetYSizeNormal();


	/*!
		size of the main window when 'compact view' is selected
	*/
	void SetYSizeCompact(int y);
	int GetYSizeCompact();


	/*!
		minimum horizontal size of the main window
	*/
	void SetXSizeMin(int x);
	int GetXSizeMin();


	/*!
		the kind of the view
	*/
	void SetView(View v);
	View GetView();


	/*!
		maximizing of the main window
	*/
	void SetMaximized(bool max);
	bool GetMaximized();


	/*!
		maximizing of the pad window
	*/
	void SetPadMaximized(bool max);
	bool GetPadMaximized();

	/*!
		the main window is always on top or not	
	*/
	void SetAlwaysOnTop(bool always);
	bool GetAlwaysOnTop();


	/*!
		rounding of the calculation's result
	*/
	void SetDisplayRounding(int r);
	int GetDisplayRounding();


	/*!
		displaying always scientific format
	*/
	void SetDisplayAlwaysScientific(bool a);
	bool GetDisplayAlwaysScientific();


	/*!
		when scientific format will be used
		(when the exponent of the result will be greater than w)
	*/
	void SetDisplayWhenScientific(int w);
	int GetDisplayWhenScientific();


	/*!
		they sets the base of input and output values (2-16)
	*/
	void SetBaseInput(int i);
	int GetBaseInput();
	void SetBaseOutput(int i);
	int GetBaseOutput();


	/*!
		the instance of the application
		(the same as that one which was passed to the WinMain(...))
	*/
	void SetInstance(HINSTANCE h);
	HINSTANCE GetInstance();


	/*!
		the main window of the application
	*/
	void SetMainWindow(HWND h);
	HWND GetMainWindow();


	void SetPadWindow(HWND h);
	HWND GetPadWindow();

	void SetPadEdit(HWND h);
	HWND GetPadEdit();

	/*!
		handles to the dialogs which are placed on the tab control
	*/
	void SetTabWindow(unsigned int id, HWND h);
	HWND GetTabWindow(unsigned int id);
	unsigned int HowManyTabWindows();


	/*!
		the precision of our calculations (0-2)
			0 - small
			1 - medium
			2 - big
	*/
	void SetPrecision(int p);
	int GetPrecision();


	/*!
		the object which is used for synchronizing threads
	*/
	volatile ThreadController * GetThreadController();


	/*!
		if you change some variables by using GetVariable() method
		you should call this method in order to inform the second thread that variables have changed
		(variables_id will be increment)
	*/
	void VariablesChanged();


	/*!
		if you change some functions by using GetFunction() method
		you should call this method in order to inform the second thread that functions have changed
		(functions_id will be increment)
	*/
	void FunctionsChanged();


	/*!
		returning the state of variables_id
		(used by the second thread)
	*/
	int GetVariablesId();


	/*!
		returning the state of functions_id
		(used by the second thread)
	*/
	int GetFunctionsId();


	/*!
		pointers to variables' table, functions' table, the languages' object, convert object
		and to the characters' buffer
	*/
	ttmath::Objects * GetVariables();
	ttmath::Objects * GetFunctions();
	Languages * GetLanguages();
	Convert * GetConvert();
	char * GetBuffer();
	char * GetBufferTemp(); // this buffer can be used for temporary tasks
	unsigned int GetBufferSize();


	/*!
		the constructor and destructor
	*/
	ProgramResources();
	~ProgramResources();


	/*!
		it returns true if we're using windows nt/2000/xp etc.
	*/
	bool IsWindowsNt();


	/*!
		we're reading a particular text-value from windows' registry
		if there'll be an error we don't change the output string 'result' and return 'false'
	*/
	bool ReadTextValueFromRegistry(HKEY main_key, const char * sub_key, const char * value, std::string & result);


	/*!
		we're trying to set the name of the configuration file of the program

		if we run the application on windows 9x the file will be in the same 
		directory in which the program has been run

		when we're using windows nt/2000/xp etc. the configuration file
		will be in a special directory for a particular user, for example:
		"C:\Documents and Settings\user\applitacion data"
	*/
	void SetNameOfFiles();


	/*!
		it is used when 'createconfig' parameter has been passed to the ttcalc program	
	*/
	void SetConfigName();

	
	/*!
		returning the name of the help file
		SetNameOfFiles() should be used first
	*/
	const std::string & GetHelpFile();


	/*!
		returning true if such a dir exists
		if not the method is trying to create it and returns true if it is created
	*/
	bool CreateDirIfNotExists(const std::string & app_dir);

	/*!
		reading and writing to a file

		ReadVariablesFunctionsFromFile() reads variables/functions only when they are not exist (in this instance of TTCalc)
	*/
	IniParser::Error ReadVariablesFunctionsFromFile();
	IniParser::Error ReadFromFile();
	void SaveToFile();


	/*!
		it returns a number of a line where there was an error
		during last parsing
	*/
	int GetBadLine();


	/*!
		setting and returning the state of removing trailing zeroes
		(used during printing values)
	*/
	void SetRemovingZeroes(bool r);
	bool GetRemovingZeroes();


	/*!
		setting and returning the unit of angle in which sin/cos/tan/ctg (arc sin...) operate
		0 - deg
		1 - red
		2 - grad
	*/
	void SetDegRadGrad(int angle);
	int  GetDegRadGrad();


	/*!
		is the pad window is visible then the method hides it (and vice versa)	
	*/
	void ShowPad(bool show);


	/*!
		returning true if the pad window is visible
	*/
	bool IsPadVisible();


	/*!
		setting/getting the group character
		0 - none
		1 - space
		2 - '`'
		3 - '\''
		4 - '.'
		5 - ','
	*/
	void SetGrouping(int g);
	int  GetGrouping();


	/*!
		setting/getting how many digits should be groupped
		digits can be from 1 to 9
	*/
	void SetGroupingDigits(int digits);
	int  GetGroupingDigits();


	/*!
		returning the grouping character
	*/
	char GetGroupingChar();


	/*!
		setting/getting the input decimal point
		0 - dot or comma, 1 - dot, 2 - comma
	*/
	void SetInputDecimalPoint(int p);
	int  GetInputDecimalPoint();


	/*!
		returning the input decimal point characters
	*/
	void GetInputDecimalPointChar(char * comma1, char * comma2);

	/*!
		setting and returning the decimal point used during printig values
		0 - '.'
		1 - ','
	*/
	void SetDecimalPoint(int decimal);
	int GetDecimalPoint();


	/*!
		returning the decimal point char (used during printig values)
	*/
	char GetDecimalPointChar();


	/*!
		setting/getting the separator of function arguments
		0 - semicolon, 1 - semicolon or dot, 2 - semicolon or comma
	*/
	void SetParamSep(int s);
	int  GetParamSep();


	/*!
		returning the character of param separator
		either a semicolon, dot or comma
	*/
	char GetParamSepChar();


	/*!
		setting/getting the test which is made when the program starts
		(testing for an update)
	*/
	void SetCheckUpdateStartup(bool s);
	bool GetCheckUpdateStartup();


	/*!
		true if there is an update available
		(used only with the dynamic menu)
	*/
	void SetUpdateExists(bool e);
	bool GetUpdateExists();


	/*!
		setting/getting the time of the last update
	*/
	void   SetLastUpdate(time_t t);
	time_t GetLastUpdate();


private:

	bool IsWhiteCharacter(int c);
	const char * SkipWhiteCharacters(const char * string);
	bool IsDecDigit(int c);
	bool SplitFunction(const std::string & input, const char * * name, int * param);
	void CheckCoordinates();
	void CheckPadCoordinates();
	void AddVariablesFunctions(IniParser::Section & temp_variables, IniParser::Section & temp_functions, bool if_not_exist);
	void ReadGlobalSection(std::string * ini_value);
	int  Int(const std::string & text);
	bool IsGlobalSectionSet(std::string * ini_value, size_t len);

	ttmath::Objects variables;
	ttmath::Objects functions;
	Languages languages;
	Convert convert;
	volatile ThreadController thread_controller;

	int variables_id;
	int functions_id;

	unsigned int buffer_size;
	char * buffer;
	char * buffer_temp;

	HINSTANCE hInstance;
	HWND main_window;
	HWND pad_window;
	HWND pad_edit; // edit control on the pad window
	HWND tab_window[6];

	int precision;
	bool always_on_top;
	bool maximized;
	View view;

	std::string configuration_file;
	std::string help_file;

	int y_size_normal;
	int y_size_compact;
	int x_size_min;

	int x_pos;
	int y_pos;
	int x_size;
	int y_size;

	int pad_x_pos;
	int pad_y_pos;
	int pad_x_size;
	int pad_y_size;
	bool pad_maximized;

	int base_input;
	int base_output;

	int decimal_point;

	bool display_always_scientific;
	int display_when_scientific;
	int display_rounding;
	bool remove_zeroes;
	int angle_deg_rad_grad;
	int grouping;				// 0 - none, 1 - space, 2 - '`', 3 - '\'', 4 - '.', 5 - ','
	int grouping_digits;        // from 1 to 9
	int input_decimal_point;	// 0 - dot or comma, 1 - dot, 2 - comma
	int param_sep;				// parameters separator: 0 - semicolon, 1 - dot, 2 - comma

	int bad_line;
	bool show_pad;
	
	bool check_update_startup;
	bool update_exists;
	time_t last_update;

};


/*!
	a global function which returns the pointer to the object of ProgramResource's type
	(there's only one object of this type)
*/
ProgramResources * GetPrgRes();


#endif
