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

#ifndef headerfilecalculation
#define headerfilecalculation

/*!
	\file parsermanager.h
    \brief object of type ParserManager we're using during calculating
*/

#include "resource.h"
#include "programresources.h"
#include <windows.h>


/*!
	\brief object of type ParserManager we're using during calculating

	In our program we're using three kind of precisions. First is the smallest
	, and the third is the biggest. Because precision is established during
	compilation (templates) we need three different objects. ParserManager
	helps us to maintain these objects.
*/
class ParserManager
{
public:

	ParserManager();
	ParserManager(const ParserManager & p);
	ParserManager & operator=(const ParserManager & p);
	~ParserManager();


	/*!
		the main method which call parserX.Parse(...)
	*/
	ttmath::ErrorCode Parse();
	

	/*
		it returns the last error code which was during parsing	
	*/
	ttmath::ErrorCode GetLastCode();


	/*!
		we call this method directly after when we have left
		WaitForCalculatingAndBlockForStop() method from the ThreadController

		only in this method we can read variables which can be changed
		by the first thread
	*/
	void MakeCopyOfVariables();


	/*!
		it prepares our three parsers to work, it should be called only once
	*/
	void Init();


	/*!
		this method prints result
		(a correct value if was or an error instead)
	*/
	int PrintResult();


	/*!
		this method prints the info about converting (on the converting tab)
	*/
	void ShowConvertingInfo();


private:

#ifndef TTCALC_PORTABLE
	ttmath::Parser<TTMathBig1> parser1;
	ttmath::Parser<TTMathBig2> parser2;
	ttmath::Parser<TTMathBig3> parser3;
#else
	ttmath::Parser<TTMathBig1> parser1;
#endif

	ttmath::Objects variables, functions;
	int last_variables_id;
	int last_functions_id;

	const unsigned int buffer_len;
	char * buffer;

	int base_input, base_output;
	int conv_type, conv_input_unit, conv_output_unit;

	bool always_scientific;
	int when_scientific;
	int rounding;
	bool remove_zeroes;
	int precision;
	Languages::Country country;
	ttmath::ErrorCode code;
	int angle_deg_rad_grad;

	char decimal_point;
	char grouping;
	char input_comma1;
	char input_comma2;
	char param_sep;

	int grouping_digits;

	/*
		some buffers which we use in some method in the second thread,
		it's better to have those buffers outside those methods -
		if the buffers were in the methods there would be still allocating
		and deallocating memory
	*/
	std::string buffer1, buffer2;
	

	bool CanWeConvert() const
	{
		if( conv_type != 0 )
			if( conv_input_unit!=-1 && conv_output_unit!=-1 &&
				conv_input_unit!=conv_output_unit )
				return true;

	return false;
	}



	void AddOutputSuffix(std::string & result)
	{
		if( CanWeConvert() )
		{
			result += " ";
			result += GetPrgRes()->GetConvert()->GetUnitAbbr(country, conv_output_unit);
		}
	}


	// 1 if carry
	template<class ValueType>
	int PrintResult(ttmath::Parser<ValueType> & matparser)
	{
	unsigned int i = 0;

		buffer1.erase();
		buffer2.erase();

		for(i=0 ; i<matparser.stack.size() ; ++i)
		{
			try
			{
				ValueType result = matparser.stack[i].value;
				
				if( CanWeConvert() )
				{
					if( GetPrgRes()->GetConvert()->Conversion(conv_input_unit, conv_output_unit, result) )
					{
						code = ttmath::err_overflow;
						return 1;
					}
				}

				ttmath::Conv conv;

				conv.base         = base_output;
				conv.scient       = always_scientific;
				conv.scient_from  = when_scientific;
				conv.round        = rounding;
				conv.trim_zeroes  = remove_zeroes;
				conv.comma        = decimal_point;
				conv.group        = grouping;
				conv.group_digits = grouping_digits;

				if( result.ToString(buffer2, conv) )
				{
					// we shouldn't have had this error in the new version of ToStrign(...)
					// (where we're using a bigger type for calculating)
					buffer2 = GetPrgRes()->GetLanguages()->GuiMessage(country, Languages::overflow_during_printing);
				}
			}
			catch(...)
			{
				buffer2 = GetPrgRes()->GetLanguages()->ErrorMessage(country, ttmath::err_internal_error);
			}

			buffer1 += buffer2;
			AddOutputSuffix(buffer1);

			if( i < matparser.stack.size()-1 )
				buffer1 += "  ;  ";
		}

		SetDlgItemText(GetPrgRes()->GetMainWindow(),IDC_OUTPUT_EDIT,buffer1.c_str());

	return 0;
	}

	template<class ValueType>
	void Parse(ttmath::Parser<ValueType> & matparser)
	{
		matparser.SetBase(base_input);
		matparser.SetDegRadGrad(angle_deg_rad_grad);
		matparser.SetComma(input_comma1, input_comma2);
		matparser.SetGroup(grouping);
		matparser.SetParamSep(param_sep);
		code = matparser.Parse(buffer);
	}



};




#endif
