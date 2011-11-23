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

#ifndef headerfileiniparser
#define headerfileiniparser

/*!
	\file iniparser.h
    \brief A Parser which we use for parsing 'ini' files
*/

#include <string>
#include <fstream>
#include <map>


/*!
	\brief it implements the parser for reading 'ini' files

	how it works?
	first you have to create an object of this class
		IniParser ini_parser;
	then you must associate a pattern and its value which'll be set after parsing
		ini_parser.Associate( "sec|pattern", &value );
	where 'value' is std::string type

	when the parser meet in the file following sequences:

		---- file.ini -------
		# some commentary
		[sec]
		pattern = some_values with space itd 
		---------------------
	
	then the value 'some_values with space etc.' will be set into the 'value' variable,
	you can use '#' as first character in a line which means the line is only a commentary,
	a pattern and its value have to be in one line.


	you can also read the whole section, for example:
		[test]
		item1 = 10
		item2 = 20
		item3 = 30
		etc.
	by using ...
*/
class IniParser
{
public:

	typedef std::map<std::string, std::string> Section;

	enum Error
	{	
		err_ok = 0,
		err_file_end,
		err_is_section,

		err_cant_open_file,
		err_incorrect_character
	};

	IniParser();

	/*!
		these methods associate pattern and its value
		(look at a description of this class)
	*/
	void Associate(const char * pattern, std::string * result);
	void Associate(const std::string & pattern, std::string * result);

	/*!
		these methods associate pattern to the whole its section
	*/
	void Associate(const char * pattern, std::map<std::string, std::string> * result);
	void Associate(const std::string & pattern, std::map<std::string, std::string> * result);

	/*!
		this method clears patterns and their values
	*/
	void Clear();


	/*!
		when the parser could not read correctly a ini file
		this method returns a number of a bad line
	*/
	int  GetBadLine();


	/*!
		this method attempts to read a file
		
		if the reading was good it returns 'err_ok'
		else it returns either 'err_cant_open_file' or 'err_incorrect_character'

	*/
	Error ReadFromFile(const char * path);



	/*!
		when true the names of sections will be case-sensitive
		e.g.
		when true: '[foo]' is not equal '[Foo]'

		default: false
	*/
	void SectionCaseSensitive(bool sens);


	/*!
		when true the names of patterns will be case-sensitive
		e.g.
		when true: 'foo = 4' is not equal 'Foo = 4'

		default: false
	*/
	void PatternCaseSensitive(bool sens);


	/*!
		if conv=true (default) that the whole characters in a value
		will be changed into small letters
	*/
	void ConvertValueToSmallLetters(bool conv);


private:

	typedef std::map<std::string, std::string *> Table;
	typedef std::map<std::string, Section *> TableWholeSection;

	Table table;
	TableWholeSection table_whole_section;
	std::ifstream file;
	int line;
	bool strip_white_characters_from_value;
	bool convert_value_to_small_letters;
	bool section_case_sensitive;
	bool pattern_case_sensitive;

	Error Read();
	Error ReadSection(std::string & section);
	int   SkipCommentaryAndEmptyLines();
	//Error CheckEndOfLine();
	Error ReadExpression(std::string & pattern, std::string & value);
	void  CheckAndSet(const std::string & section, const std::string & pattern, const std::string & value);
	bool  IsWhiteCharacter(int c);
	void  SkipWhiteCharacters();
	int   ReadCharacter();
	void  ReturnCharacter(int c);
	bool  IsSectionCharacter(int c);
	bool  IsPatternCharacter(int c);
	bool  IsValueCharacter(int c);
	void  SkipLine();
	int   LowerCase(int c);

public:
	static void CheckWhiteCharacters(const char * string, const char ** start, const char ** end);
	static void StripWhiteCharacters(std::string & string);
};


#endif
