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

#ifndef headerfilemessages
#define headerfilemessages

#include <map>
#include <windows.h>



template<typename ProcReturnType>
class Messages
{
public:
	typedef	ProcReturnType (*MessageFunction)(HWND, UINT, WPARAM, LPARAM);
	typedef	std::map<UINT, MessageFunction> MessageTable;


	void Associate(UINT message, MessageFunction message_function)
	{
		message_table.insert( std::make_pair(message, message_function) );
	}
	

	void Clear()
	{
		message_table.clear();
	}


	ProcReturnType Call(UINT key, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool * method_exists = 0)
	{
		typename MessageTable::iterator i = message_table.find(key);

		if( i == message_table.end() )
		{
			if( method_exists )
				*method_exists = false;

			return false;
		}

		if( method_exists )
			*method_exists = true;

	return i->second(hWnd, message, wParam, lParam);
	}


	bool IsMessage(UINT key)
	{
		typename MessageTable::iterator i = message_table.find(key);

		if( i == message_table.end() )
			return false;

	return true;
	}


	bool Empty() const
	{
		return message_table.empty();
	}


private:

	MessageTable message_table;

};


#endif
