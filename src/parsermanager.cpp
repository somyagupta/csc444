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
#include "parsermanager.h"
#include "tabs.h"




ParserManager::ParserManager() : buffer_len(20480)
{
	buffer = 0;
	last_variables_id = 0;
	last_functions_id = 0;
}


ParserManager::ParserManager(const ParserManager & p) : buffer_len(p.buffer_len)
{
	buffer = 0;
}


ParserManager & ParserManager::operator=(const ParserManager &)
{
	buffer = 0;

return *this;
}


ParserManager::~ParserManager()
{
	delete [] buffer;
}


ttmath::ErrorCode ParserManager::GetLastCode()
{
	return code;
}




ttmath::ErrorCode ParserManager::Parse()
{
	try
	{
	#ifndef TTCALC_PORTABLE

		switch( precision )
		{
		case 0:
			Parse(parser1);
			break;

		case 1:
			Parse(parser2);
			break;

		default:
			Parse(parser3);
			break;
		}

	#else

		Parse(parser1);

	#endif
	}
	catch(...)
	{
		// we are in a thread, we shouldn't go up
		code = ttmath::err_internal_error;
	}

return code;
}


void ParserManager::MakeCopyOfVariables()
{
unsigned int i;

	char * pchar = (char*)GetPrgRes()->GetBuffer();
	for(i = 0 ; i<buffer_len-1 && pchar[i]!=0 ; ++i)
		buffer[i] = pchar[i];

	buffer[i] = 0;

	//
	
	if( GetPrgRes()->GetVariablesId() != last_variables_id )
	{
		variables = *GetPrgRes()->GetVariables();
		last_variables_id = GetPrgRes()->GetVariablesId();
	}

	if( GetPrgRes()->GetFunctionsId() != last_functions_id )
	{
		functions = *GetPrgRes()->GetFunctions();
		last_functions_id = GetPrgRes()->GetFunctionsId();
	}


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

	conv_type = GetPrgRes()->GetConvert()->GetCurrentType();
	GetPrgRes()->GetConvert()->GetCurrentUnit(conv_type, conv_input_unit, conv_output_unit);
}


void ParserManager::Init()
{
	buffer = new char[buffer_len];
	buffer[0] = 0;

	parser1.SetStopObject( GetPrgRes()->GetThreadController()->GetStopObject() );
	parser1.SetVariables( &variables );
	parser1.SetFunctions( &functions );

	#ifndef TTCALC_PORTABLE

	parser2.SetStopObject( GetPrgRes()->GetThreadController()->GetStopObject() );
	parser2.SetVariables( &variables );
	parser2.SetFunctions( &functions );

	parser3.SetStopObject( GetPrgRes()->GetThreadController()->GetStopObject() );
	parser3.SetVariables( &variables );
	parser3.SetFunctions( &functions );

	#endif
}


int ParserManager::PrintResult()
{
	if( code == ttmath::err_ok )
	{
	#ifndef TTCALC_PORTABLE

		switch( precision )
		{
		case 0:
			return PrintResult(parser1);

		case 1:
			return PrintResult(parser2);

		default:
			return PrintResult(parser3);
		}

	#else
		return PrintResult(parser1);
	#endif
	}

return 0;
}	


void ParserManager::ShowConvertingInfo()
{
HWND conv_tab = GetPrgRes()->GetTabWindow(TabWindowFunctions::tab_convert);

	if( conv_type==0 || conv_input_unit==-1 || conv_output_unit==-1 ||
		conv_input_unit == conv_output_unit )
	{
		SetDlgItemText(conv_tab, IDC_EDIT_OUTPUT_INFO, "");
		return;
	}

	Convert * pconv = GetPrgRes()->GetConvert();

	// the first unit to the second

	ttmath::Big<1,1> result;
	result.SetOne();
	std::string buffer1 = "1 ";
	buffer1 += pconv->GetUnitAbbr(country, conv_input_unit);
	buffer1 += " = ";

	if(	pconv->Conversion(conv_input_unit, conv_output_unit, result) )
	{
		SetDlgItemText(conv_tab, IDC_EDIT_OUTPUT_INFO, "overflow" );
		return;
	}

	result.ToString(buffer2, 10, false, 3, -1, true);

	buffer1 += buffer2;
	buffer1 += " "; 
	buffer1 += pconv->GetUnitAbbr(country, conv_output_unit);


	// the second unit to the first

	buffer1 += "   1 ";
	buffer1 += pconv->GetUnitAbbr(country, conv_output_unit);
	buffer1 += " = ";
	
	result.SetOne();
	if(	pconv->Conversion(conv_output_unit, conv_input_unit, result) )
	{
		SetDlgItemText(conv_tab, IDC_EDIT_OUTPUT_INFO, "overflow" );
		return;
	}

	
	result.ToString(buffer2, 10, false, 3, -1, true);

	buffer1 += buffer2;
	buffer1 += " "; 
	buffer1 += pconv->GetUnitAbbr(country, conv_input_unit);
	
	SetDlgItemText(conv_tab, IDC_EDIT_OUTPUT_INFO, buffer1.c_str() );
}
