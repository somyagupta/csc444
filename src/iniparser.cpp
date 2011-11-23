/*
 * This file is a part of TTCalc - a mathematical calculator
 * and is distributed under the (new) BSD licence.
 * Author: Tomasz Sowa <t.sowa@ttmath.org>
 */

/* 
 * Copyright (c) 2006-2008, Tomasz Sowa
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

#include "iniparser.h"


IniParser::IniParser()
{
	strip_white_characters_from_value = true;
	convert_value_to_small_letters    = true;
	section_case_sensitive = false;
	pattern_case_sensitive = false;
}


void IniParser::ConvertValueToSmallLetters(bool conv)
{
	convert_value_to_small_letters = conv;
}


void IniParser::SectionCaseSensitive(bool sens)
{
	section_case_sensitive = sens;
}


void IniParser::PatternCaseSensitive(bool sens)
{
	pattern_case_sensitive = sens;
}


void IniParser::Associate(const char * pattern, std::string * result)
{
	table.insert( std::make_pair(std::string(pattern), result) );
}


void IniParser::Associate(const std::string & pattern, std::string * result)
{
	table.insert( std::make_pair(pattern, result) );
}

void IniParser::Associate(const char * pattern, std::map<std::string, std::string> * result)
{
	table_whole_section.insert( std::make_pair(std::string(pattern), result) );
}

void IniParser::Associate(const std::string & pattern, std::map<std::string, std::string> * result)
{
	table_whole_section.insert( std::make_pair(pattern, result) );
}

void IniParser::Clear()
{
	table.clear();
	table_whole_section.clear();
}


IniParser::Error IniParser::ReadFromFile(const char * path)
{
	file.open(path);

	if(!file)
		return err_cant_open_file;

	Error err = Read();

return err;
}


int IniParser::GetBadLine()
{
	return line;
}



IniParser::Error IniParser::Read()
{
std::string section, pattern, value;
Error err;

	line = 1;

	do
	{
		err = ReadSection(section);
		if( err != err_ok )
			break;

		err = ReadExpression(pattern, value);

		while( err == err_ok )
		{
			if( strip_white_characters_from_value )
				StripWhiteCharacters(value);

			CheckAndSet( section, pattern, value );

			err = ReadExpression(pattern, value);
		}

	}
	while( err == err_is_section );


	if( err == err_file_end )
		err = err_ok;

return err;
}



IniParser::Error IniParser::ReadSection(std::string & section)
{
	section.erase();
	
	int c = SkipCommentaryAndEmptyLines();
	if( c == EOF )
		return err_file_end;

	if( c != '[' )
	{
		// this line is not for a section (empty section?)
		ReturnCharacter(c);
		return err_ok;
	}
	
	while( IsSectionCharacter( (c = ReadCharacter()) ) )
		if( section_case_sensitive )
			section += c;
		else
			section += LowerCase(c);

	if( c != ']' )
	{
		ReturnCharacter(c);
		return err_incorrect_character;	
	}

return err_ok;
}



int IniParser::SkipCommentaryAndEmptyLines()
{
int c;

	do
	{
		SkipWhiteCharacters();
		c = ReadCharacter();

		if( c == '#' )
			SkipLine();
		else
		if( c == 10 )
			c='#';
	}
	while( c == '#' );

return c;
}


/*
IniParser::Error IniParser::CheckEndOfLine()
{
	SkipWhiteCharacters();
	int c = ReadCharacter();

	if( c == EOF )
		return err_file_end;

	if( c != 10 )
	{
		ReturnCharacter(c);
		return err_incorrect_character;	
	}

return err_ok;
}
*/



IniParser::Error IniParser::ReadExpression(std::string & pattern, std::string & value)
{
	pattern.erase();
	value.erase();

	int c = SkipCommentaryAndEmptyLines();
	if( c == EOF )
		return err_file_end;

	if( c == '[' )
	{
		ReturnCharacter(c);
		return err_is_section;
	}

	while( IsPatternCharacter(c) )
	{
		if( pattern_case_sensitive )
			pattern += c;
		else
			pattern += LowerCase(c);

		c = ReadCharacter();
	}

	if( IsWhiteCharacter(c) )
	{
		SkipWhiteCharacters();
		c = ReadCharacter();
	}

	if( c != '=' )
	{
		ReturnCharacter(c);
		return err_incorrect_character;
	}
	
	SkipWhiteCharacters();

	while( IsValueCharacter( (c = ReadCharacter()) ) )
		if( convert_value_to_small_letters )
			value += LowerCase(c);
		else
			value += c;

	ReturnCharacter(c);

//return CheckEndOfLine();
return err_ok;
}



void IniParser::CheckAndSet(const std::string & section, const std::string & pattern, const std::string & value)
{
	TableWholeSection::iterator iw = table_whole_section.find( section );

	if( iw != table_whole_section.end() )
	{
		(*iw->second).insert( std::make_pair( pattern, value ) );
		return;
	}


	Table::iterator i = table.find( section + '|' + pattern );

	if( i == table.end() )
		return;

	*i->second = value;
}



bool IniParser::IsWhiteCharacter(int c)
{
	// 13 is in 'dos and windows' systems at the end of a text file (13 and 10 exactly)
	if( c==' ' || c=='\t' || c==13 )
		return true;

return false;
}



void IniParser::SkipWhiteCharacters()
{
int c;

	while( IsWhiteCharacter( (c=ReadCharacter()) ) );

	ReturnCharacter(c);
}


int IniParser::ReadCharacter()
{
	int c = file.get();

	if( !file )
		c = EOF;
	else
	if( c == 10 )
		++line;

return c;
}



void IniParser::ReturnCharacter(int c)
{
	file.unget();

	if( c == 10 )
		--line;
}


bool IniParser::IsSectionCharacter(int c)
{
	if( (c>='a' && c<='z') ||
		(c>='A' && c<='Z') ||
		(c>='0' && c<='9') ||
		c=='_' || c=='.' )

		return true;

return false;
}


bool IniParser::IsPatternCharacter(int c)
{
	return IsSectionCharacter(c);
}



bool IniParser::IsValueCharacter(int c)
{	
	if( c!=10 && c!=EOF )
		return true;

return false;
}



void IniParser::SkipLine()
{
	while( ReadCharacter() != 10 );
}


int IniParser::LowerCase(int c)
{
	if( c>='A' && c<='Z')
		return c - 'A' + 'a';

return c;
}



/*!
	this function sets '*start' and '*end' to point to the middle part of the string
	without any whitespaces at the end and at the beginning

	'*start' will be the first correct character
	'*end'   will be after the last correct character
*/
void IniParser::CheckWhiteCharacters(const char * string, const char ** start, const char ** end)
{
const char * p;

	// we're setting 'p' at the end of the string (at '\0' character)
	for( p = string ; *p ; ++p );

	if( p == string )
	{
		// an empty string
		*start = *end = string;
		return;
	}

	// we're looking for a last character which is different than a white character
	for( --p ; p != string && (*p==' ' || *p=='\t') ; --p );

	// 'end' will be after the last character
	*end = p+1;

	// we're looking for a first character which is different than a white character
	for( p = string ; *p==' ' || *p=='\t' ; ++p );
	
	*start = p;
}




void IniParser::StripWhiteCharacters(std::string & string)
{
const char *s, * start, * end;

	s = string.c_str();
	CheckWhiteCharacters(s, &start, &end);


	std::string::size_type istart, iend;

	istart = start - s;
	iend  = end - s;

	string.erase(iend, string.length() - iend);
	string.erase(0, istart);
}
















