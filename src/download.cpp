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

#include <cstdio>
#include "compileconfig.h"
#include "download.h"


Download::Download()
{
	buffer_len = 128;
	progress   = 0;

	buffer = new char[buffer_len];
}


Download::~Download()
{
	if( buffer )
		delete [] buffer;
}


void Download::SetProgress()
{
	if( !progress )
		return;

	SendMessage(progress, PBM_SETRANGE32, 0, all_len);
	SendMessage(progress, PBM_SETPOS, 0, 0);
}


void Download::ChangeProgress(size_t size)
{
	if( !progress )
		return;

	SendMessage(progress, PBM_SETPOS, size, 0);
}



void Download::ClearProgress()
{
	if( !progress )
		return;

	SendMessage(progress, PBM_SETPOS, 0, 0);
}


bool Download::DownloadLoop(HINTERNET conn)
{
DWORD read;


	while( true )
	{
		bool res = InternetReadFile(conn, buffer, buffer_len, &read);

		if( !res )
			// some problems with downloading
			return false;


		if( read > 0 )
		{
			down_size += read;
			ChangeProgress(down_size);
			
			if( !Read(buffer, read) )
				return false;
		}
		else
		{
			// end
			return true;
		}
	}
}


bool Download::DownloadUrl(const char * url)
{
bool res = true;
char browser[100];

	down_size = 0;
	sprintf(browser, "TTCalc updater %d.%d.%d%s",
		TTCALC_MAJOR_VER,
		TTCALC_MINOR_VER,
		TTCALC_REVISION_VER,
		(TTCALC_PRERELEASE_VER>0)? " prerelease" : "");


	HINTERNET hint = InternetOpen(browser,INTERNET_OPEN_TYPE_DIRECT, 0, 0, 0);
	if( !hint )
		return false;

	HINTERNET conn = InternetOpenUrl(hint, url, 0, 0,
									 INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_UI, 0);

	if( conn )
	{
		GetLen(conn);
		SetProgress();

		if( Init() )
		{
			res = DownloadLoop(conn);
			Close();
			ClearProgress();
		}

		InternetCloseHandle(conn);
	}
	else
	{
		res = false;
	}

	InternetCloseHandle(hint);	
	
return res;
}














