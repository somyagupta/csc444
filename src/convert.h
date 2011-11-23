/*
 * This file is a part of TTCalc - a mathematical calculator
 * and is distributed under the (new) BSD licence.
 * Author: Tomasz Sowa <t.sowa@ttmath.org>
 */

/* 
 * Copyright (c) 2006-2009, Tomasz Sowa
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

#ifndef convertheader
#define convertheader

#include <windows.h>
#include <string>
#include "compileconfig.h"
#include "bigtypes.h"



class Convert
{

public:

	Convert();


	/*
		none, length, area, time, 
	*/
	struct Type
	{
		std::string name;
	};



	struct Unit
	{
		std::string name, abbr;
		int type;
		TTMathBigMax value_numerator, value_denominator;

		Unit()
		{
			type = -1;
			value_numerator.SetZero();
			value_denominator.SetOne();
		}
	};


	std::vector<std::vector<Type> > type_tab;
	std::vector<std::vector<Unit> > unit_tab;




	void AddType(const char * name);
	void AddUnit(const char * name, const char * abbr = 0, int numerator = 0, int denominator = 1);
	void AddUnit(const char * name, const char * abbr, const char * v);

	void Init();


	const char * GetTypeName(int cid, int id);

	const char * GetUnitName(int cid, int id);
	const char * GetUnitAbbr(int cid, int id);
	int          GetUnitType(int id);

	int GetTypeSize();
	int GetUnitSize();


	/*
		the return value is the same for all languages
		(in fact we're taking it from english)
	*/
	bool GetUnitValue(int id, TTMathBigMax & numerator, TTMathBigMax & denominator);

	void ErrorMessage();
	void CheckAfterInit();


	template<class ValueType>
	int Conversion(int conv_input_unit, int conv_output_unit, ValueType & result)
	{
		int c = 0;

		ValueType v1_n, v1_d; // input
		ValueType v2_n, v2_d; // output

		TTMathBigMax v1_n_, v1_d_; // input
		TTMathBigMax v2_n_, v2_d_; // output

		if( !GetUnitValue(conv_input_unit, v1_n_, v1_d_) )
			return 1;

		if( !GetUnitValue(conv_output_unit, v2_n_, v2_d_) )
			return 1;

		v1_n.FromBig( v1_n_ );
		v1_d.FromBig( v1_d_ );
		v2_n.FromBig( v2_n_ );
		v2_d.FromBig( v2_d_ );

		// result = result * (v1_n * v2_d) / (v1_d * v2_n)
		c += result.Mul(v1_n);
		c += result.Mul(v2_d);
		c += v1_d.Mul(v2_n);
		c += result.Div(v1_d);

	return (c==0)? 0 : 1;
	}



	/*
		for the interface
	*/
	
	int current_conv_type; // selected type of converting: none, length, area, ...

	struct CurrentConvUnit
	{
		int input, output;
		CurrentConvUnit() { input = output = -1; }
	};
	
	std::vector<CurrentConvUnit> current_conv_unit;



	void SetCurrentType(int c);
	int  GetCurrentType();
	void CheckUnitIndex(int type);
	void CheckCurrentUnits();
	void SetCurrentUnit(int type, int input, int output);
	bool GetCurrentUnit(int type, int & input, int & output);

	// end part of gui


};


#endif
