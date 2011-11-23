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

#include "convert.h"



Convert::Convert()
{
	Init();
	current_conv_type = 0;
	CheckCurrentUnits();
}


void Convert::AddType(const char * name)
{
	if( type_tab.empty() )
		return;

	Type temp;
	temp.name = name;

	type_tab.back().push_back(temp);
}


void Convert::AddUnit(const char * name, const char * abbr, int numerator, int denominator)
{
	if( type_tab.empty() || type_tab.back().empty() || unit_tab.empty() )
		return;

	Unit temp;
	temp.name = name;

	if( abbr != 0 )
		temp.abbr = abbr;


	temp.type = int(type_tab.back().size()-1);


	temp.value_numerator.FromInt(numerator);

	if( denominator != 1 )
	{
		// value_denominator is set into one (default)
		temp.value_denominator.FromInt(denominator);
	}

	unit_tab.back().push_back(temp);
}




void Convert::AddUnit(const char * name, const char * abbr,  const char * v)
{
	if( type_tab.empty() || type_tab.back().empty() || unit_tab.empty() )
		return;

	Unit temp;
	temp.name = name;
	temp.abbr = abbr;
	temp.type = int(type_tab.back().size()-1);
	temp.value_numerator.FromString(v, 16);

	unit_tab.back().push_back(temp);
}


void Convert::Init()
{
	// en
	type_tab.push_back( std::vector<Type>() );
	unit_tab.push_back( std::vector<Unit>() );

	AddType("none");

	/*
		yard = 0.9144 metre
		foot = 1/3 yard = 0.3048 metre
		inch = 1/36 yard = 1/12 foot = 0.0254 metre

		In 1958 the United States and countries of the Commonwealth of Nations
		defined the length of the international yard to be precisely 0.9144 metres.
		
		1 point (Didot's system) = 1 / 2660 m
		1 point (pica - USA and UK) = 1 / 72 inch
	*/
	AddType("length"); // base: 10000 metre
	AddUnit("inch", "in", 254);
	AddUnit("foot", "ft", 3048);
	AddUnit("yard", "yd", 9144);
	AddUnit("metre", "m", 10000);
	AddUnit("mile (land)", "mi", 16093440); // 1 land mile =  1609.344 metres
	AddUnit("mile (nautical)", "nmi", 18520000); // 1 nautical mile = 1852 metres
	AddUnit("point (Didot)", "p", 10000, 2660);
	AddUnit("point (pica)",  "p", 18288, 5184);
	AddUnit("light year", "ly", "520F0AA65AAEBAC00"); // 1 ly = 9 460 730 472 580 800 metres
	// 520F0AA65AAEBAC00 (hex) = 94607304725808000000 (dec)
	


	/*
		yard^2 = 0.83612736 metre^2
		foot^2 = 1/9 yard^2 = 0.09290304 metre^2
		inch^2 = 1/1296 yard^2 = 1/144 foot^ = 0.00064516 metre^2

		ar = 100 metre^2
		hectere = 10000 metre^2
	*/
	
	// !!! give a smaller base here
	AddType("area");  // base: 100 000 000 metre^2
	AddUnit("inch^2", "in^2", 64516);
	AddUnit("foot^2", "ft^2", 9290304);
	AddUnit("yard^2", "yd^2", 83612736);
	AddUnit("metre^2", "m^2", 100000000);
	AddUnit("are", "a", "2540BE400"); // 10000000000 (dec)
	AddUnit("hectare", "ha", "E8D4A51000"); // 1000000000000 (dec)
	AddUnit("mile (land)^2", "mi^2", "EB8EDC210000" ); // 258998811033600 (dec), 1 land mile^2 =  2589988.110336 metres^2
	AddUnit("mile (nautical)^2", "nmi^2", "137F2ACF41000"); // 342990400000000 (dec), 1 nautical mile^2 = 3429904 metres^2
	AddUnit("light year^2", "ly^2", "1A4DA3B4D207FB4C43D08391653B900000"); // 1 ly = 89505421074818927300612528640000 metres^2
	// 1A4DA3B4D207FB4C43D08391653B900000 (hex) =  8950542107481892730061252864000000000000 (dec)


	AddType("time");  // base: second
	AddUnit("second", "s", 1);
	AddUnit("minute", "m", 60);
	AddUnit("hour", "h", 3600);
	AddUnit("day", "d", 86400);
	AddUnit("week", "w", 604800);
	AddUnit("fortnight", "fn", 1209600);
	AddUnit("month", "m", 2592000);
	AddUnit("year", "y", 31536000);
	AddUnit("year (mean)", "my", 31557600);
	AddUnit("year (leap)", "ly", 31622400);
	AddUnit("decade", "dc", 315360000);
	


	// pl
	// the 'value' param we're using only from the english
	type_tab.push_back( std::vector<Type>() );
	unit_tab.push_back( std::vector<Unit>() );

	AddType("brak");

	AddType("d³ugoœæ");
	AddUnit("cal");
	AddUnit("stopa");
	AddUnit("jard");
	AddUnit("metr");
	AddUnit("mila (angielska)");
	AddUnit("mila (morska)");
	AddUnit("punkt (Didot)");
	AddUnit("punkt (pica)");
	AddUnit("rok œwietlny");

	AddType("powierzchnia");
	AddUnit("cal^2");
	AddUnit("stopa^2");
	AddUnit("jard^2");
	AddUnit("metr^2");
	AddUnit("ar", "");
	AddUnit("hektar");
	AddUnit("mila (angielska)^2");
	AddUnit("mila (morska)^2");
	AddUnit("rok œwietlny^2");


	AddType("czas");
	AddUnit("sekunda");
	AddUnit("minuta");
	AddUnit("godzina");
	AddUnit("dzieñ");
	AddUnit("tydzieñ");
	AddUnit("dwa tygodnie");
	AddUnit("miesi¹c");
	AddUnit("rok");
	AddUnit("rok (œredni)");
	AddUnit("rok (przestêpny)");
	AddUnit("dekada");

	CheckAfterInit();
}


void Convert::ErrorMessage()
{
	MessageBox(0, "Internal error: tables for converting have incorrect values",
		"Internal error", MB_ICONERROR);

	exit(0);
}


void Convert::CheckAfterInit()
{
unsigned int i, len;

	if( type_tab.size() != unit_tab.size() )
		return ErrorMessage();

	if( type_tab.empty() )
		return;

	len = (unsigned int)type_tab[0].size();
	for(i=1 ; i<type_tab.size() ; ++i)
	{
		if( len != type_tab[i].size() )
			return ErrorMessage();
	}

	len = (unsigned int)unit_tab[0].size();
	for(i=1 ; i<unit_tab.size() ; ++i)
	{
		if( len != unit_tab[i].size() )
			return ErrorMessage();
	}

}



/*!
	cid - country id

*/
const char * Convert::GetTypeName(int cid, int id)
{
const char * unk_err = "unknown";
	
	if( type_tab.empty() )
		return unk_err;

	if( cid >= (int)type_tab.size() )
		cid = 0;

	if( id< 0 || id >= int(type_tab[cid].size()) )
		return unk_err;

return type_tab[cid][id].name.c_str();
}




const char * Convert::GetUnitName(int cid, int id)
{
const char * unk_err = "unknown";

	if( unit_tab.empty() )
		return unk_err;

	if( cid >= (int)unit_tab.size() )
		cid = 0;

	if( id < 0 || id >= int(unit_tab[cid].size()) )
		return unk_err;

return unit_tab[cid][id].name.c_str();
}




const char * Convert::GetUnitAbbr(int cid, int id)
{
const char * unk_err = "unknown";

	if( unit_tab.empty() )
		return unk_err;

	if( cid >= (int)unit_tab.size() )
		cid = 0;

	if( id < 0 || id >= int(unit_tab[cid].size()) )
		return unk_err;

	if( !unit_tab[cid][id].abbr.empty() )
		return unit_tab[cid][id].abbr.c_str();

	/*
		if there is no defined an abbreviation in the specific 
		language, we return the abbreviation from english
	*/

return unit_tab[0][id].abbr.c_str();
}




int Convert::GetTypeSize()
{
	if( type_tab.empty() )
		return 0;

	return int(type_tab[0].size());
}


int Convert::GetUnitSize()
{
	if( unit_tab.empty() )
		return 0;

	return int(unit_tab[0].size());
}


int Convert::GetUnitType(int id)
{
	if( unit_tab.empty() )
		return -1;

	if( id >= int(unit_tab[0].size()) )
		return -1;

return unit_tab[0][id].type;
}


// value is taken only from english
bool Convert::GetUnitValue(int id, TTMathBigMax & numerator, TTMathBigMax & denominator)
{
	if( unit_tab.empty() )
		return false;

	if( id >= int(unit_tab[0].size()) )
		return false;

	numerator   = unit_tab[0][id].value_numerator;
	denominator = unit_tab[0][id].value_denominator;

return true;
}




//  gui

void Convert::SetCurrentType(int c)
{
	current_conv_type = c;
}


int Convert::GetCurrentType()
{
	return current_conv_type;
}



void Convert::CheckUnitIndex(int type)
{
	if( current_conv_unit[type].input < -1 || current_conv_unit[type].input >= GetUnitSize() )
		current_conv_unit[type].input = 0; // maybe better to -1 ?

	if( current_conv_unit[type].output < -1 || current_conv_unit[type].output >= GetUnitSize() )
		current_conv_unit[type].output = 0; // the same
}


void Convert::CheckCurrentUnits()
{
	int len = GetTypeSize();
	
	if( static_cast<int>( current_conv_unit.size() ) != len )
		current_conv_unit.resize(len);

	for(int i = 0 ; i<len ; ++i)
		CheckUnitIndex(i);
}


void Convert::SetCurrentUnit(int type, int input, int output)
{
	int len = GetTypeSize();
	
	if( type >= len )
		return;
	
	current_conv_unit[type].input  = input;
	current_conv_unit[type].output = output;
	CheckUnitIndex(type);
}


bool Convert::GetCurrentUnit(int type, int & input, int & output)
{
	int len = GetTypeSize();
	
	if( type >= len )
	{
		input = output = -1;
		return false;
	}
	
	input  = current_conv_unit[type].input;
	output = current_conv_unit[type].output;
	
return true;
}

// 
