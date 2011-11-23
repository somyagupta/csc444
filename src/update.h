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

#ifndef headerfileupdate
#define headerfileupdate

#include <windows.h>
#include <fstream>
#include <cstdio>
#include "download.h"


void ShowUpdateDialog(HWND hwnd);
void ShowUpdateDialogAfterCheck(HWND hwnd);
void CheckUpdateInBackground();



class DownloadIni : public Download
{
public:
	DownloadIni();
	~DownloadIni();
	void DeleteTmpFile();
	const char * GetFileName();
	bool DownloadUrl(const char * url, bool * continue_download = 0);

private:

	FILE * CreateTmpFile();
	virtual void GetLen(HINTERNET conn);
	virtual bool Init();
	virtual bool Read(char * buffer, size_t size);
	virtual void Close();

	char * file_name;
	FILE * file;
	bool * continue_download;
};





class DownloadProgram : public Download
{
public:
	DownloadProgram();
	bool DownloadUrl(const char * url, bool * continue_download = 0);

private:
	virtual void GetLen(HINTERNET conn);
	virtual bool Init();
	virtual bool Read(char * buffer, size_t size);
	virtual void Close();

	std::ofstream file;
	bool * continue_download;
};



#endif
