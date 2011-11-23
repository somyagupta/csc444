/*
 * This file is a part of TTCalc - a mathematical calculator
 * and is distributed under the (new) BSD licence.
 * Author: Tomasz Sowa <t.sowa@ttmath.org>
 */

/* 
 * Copyright (c) 2006-2010, Tomasz Sowa
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

#include <cstdlib>
#include <shlobj.h>
#include <process.h>

#include "update.h"
#include "programresources.h"
#include "messages.h"
#include "resource.h"
#include "languages.h"
#include "winmain.h"
#include "misc.h"



namespace Update
{
	bool continue_down;
	int level;
	std::string url;
	std::string remote_file_name;
	std::string local_file_name;
	int major;
	int minor;
	int revision;
	int prerelease;

	// this is set by another thread at the beginning
	namespace Background
	{
		std::string url_;
		std::string remote_file_name_;
		int major_;
		int minor_;
		int revision_;
		int prerelease_;
	}
}


/*



*/
DownloadIni::DownloadIni()
{
	file = 0;
	file_name = new char[MAX_PATH];
	file_name[0] = 0;
	continue_download = 0;
}


DownloadIni::~DownloadIni()
{
	if( file )
		fclose(file);

	DeleteTmpFile();

	delete [] file_name;
}


void DownloadIni::DeleteTmpFile()
{
	if( file_name[0] != 0 )
		DeleteFile(file_name);
}

const char * DownloadIni::GetFileName()
{
	return file_name;
}


FILE * DownloadIni::CreateTmpFile()
{
char buf[MAX_PATH];

	file = 0;
	file_name[0] = 0;

	if( !GetTempPath(MAX_PATH, buf) )
		return 0;


	if( !GetTempFileName(buf, "ttcalc_", 0, file_name) )
	{
		file_name[0] = 0;
		return 0;
	}

	file = fopen(file_name, "wb");

return file;
}

void DownloadIni::GetLen(HINTERNET conn)
{
char buf[100];
DWORD buf_len = sizeof(buf);
DWORD index = 0;

	if( HttpQueryInfo(conn, HTTP_QUERY_CONTENT_LENGTH, buf, &buf_len, &index) )
		all_len = atoi(buf);
	else
		all_len = 0;
}



bool DownloadIni::Init()
{
	if( !CreateTmpFile() )
		return false;

return true;
}



bool DownloadIni::Read(char * buffer, size_t size)
{
	if( !file )
		return false;

	fwrite(buffer, size, 1, file);
	CheckMessages();

	if( continue_download && !(*continue_download) )
		return false;

return true;
}

bool DownloadIni::DownloadUrl(const char * url, bool * c)
{
	continue_download = c;
	
return Download::DownloadUrl(url);
}


void DownloadIni::Close()
{
	if( file )
		fclose(file);

	file = 0;
}






/*



*/

DownloadProgram::DownloadProgram()
{
	continue_download = 0;
}


void DownloadProgram::GetLen(HINTERNET conn)
{
char buf[100];
DWORD buf_len = sizeof(buf);
DWORD index = 0;

	if( HttpQueryInfo(conn, HTTP_QUERY_CONTENT_LENGTH, buf, &buf_len, &index) )
		all_len = atoi(buf);
	else
		all_len = 0;
}


bool DownloadProgram::Init()
{
	file.open(Update::local_file_name.c_str(), std::ios_base::out | std::ios_base::binary);

	if( !file )
		return false;

return true;
}



bool DownloadProgram::Read(char * buffer, size_t size)
{
	file.write(buffer, size);
	CheckMessages();

	if( continue_download && !(*continue_download) )
		return false;

return true;
}


bool DownloadProgram::DownloadUrl(const char * url, bool * c)
{
	continue_download = c;

return Download::DownloadUrl(url);
}


void DownloadProgram::Close()
{
	file.close();
}




/*
	we check whether the download url is correct
	this is in the case of someone hacked the server with *.ini file
*/
bool IsServerCorrect(const std::string & url)
{
	if( IsSubStringNoCase("http://downloads.sourceforge.net/", url.c_str()) ||
		IsSubStringNoCase("ftp://ttmath.org/", url.c_str()) )
		return true;

return false;
}



/*
	this method is called from the other thread too
	make sure you don't write directly to any global variables (from Update namespace etc)
*/
bool CheckUpdateFromIni(HWND hwnd, std::string & url_, std::string & remote_file_name_, 
						int & major_, int & minor_, int & revision_, int & prerelease_,
						bool * continue_download_)
{
DownloadIni down_ini;

	if( hwnd )
		down_ini.AttachProgress(GetDlgItem(hwnd, IDC_PROGRESS));

	if( down_ini.DownloadUrl("http://update.ttmath.org/ttcalc_update.ini", continue_download_) )
	{
		IniParser iparser;
		std::string ini_value[4];

		iparser.SectionCaseSensitive(false);

		#ifndef TTCALC_PORTABLE
		iparser.Associate("normal|url",					&url_ );
		iparser.Associate("normal|filename",			&remote_file_name_ );
		iparser.Associate("normal|version.major",		&ini_value[0] );
		iparser.Associate("normal|version.minor",		&ini_value[1] );
		iparser.Associate("normal|version.revision",	&ini_value[2] );
		iparser.Associate("normal|version.prerelease",	&ini_value[3] );
		
		#else
		iparser.Associate("portable|url",				&url_ );
		iparser.Associate("portable|filename",			&remote_file_name_ );
		iparser.Associate("portable|version.major",		&ini_value[0] );
		iparser.Associate("portable|version.minor",		&ini_value[1] );
		iparser.Associate("portable|version.revision",	&ini_value[2] );
		iparser.Associate("portable|version.prerelease",&ini_value[3] );
		#endif

		IniParser::Error err = iparser.ReadFromFile( down_ini.GetFileName() );

		if( err == IniParser::err_ok && IsServerCorrect(url_) )
		{
			major_    = atoi(ini_value[0].c_str());
			minor_    = atoi(ini_value[1].c_str());
			revision_ = atoi(ini_value[2].c_str());
			prerelease_ = atoi(ini_value[3].c_str());

			if( major_ > TTCALC_MAJOR_VER )
				return true;
			else
			if( major_ == TTCALC_MAJOR_VER && minor_ > TTCALC_MINOR_VER )
				return true;
			else
			if( major_ == TTCALC_MAJOR_VER && minor_ == TTCALC_MINOR_VER && revision_ > TTCALC_REVISION_VER )
				return true;
			else
			if( major_ == TTCALC_MAJOR_VER && minor_ == TTCALC_MINOR_VER && revision_ == TTCALC_REVISION_VER && 
				prerelease_ < TTCALC_PRERELEASE_VER )
				return true;
		}
	}

return false;
}




namespace Update
{
	


void CheckUpdate(HWND hwnd)
{
char buffer[200];

	level = -1; // nothing to do when Next is pressed
	continue_down = true;
	Languages * pl = GetPrgRes()->GetLanguages();
	EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_NEXT), false);
	EnableWindow(GetDlgItem(hwnd, IDC_UPDATE_INFO1), false);
	ShowWindow(GetDlgItem(hwnd, IDC_PROGRESS), SW_SHOW);
	CheckMessages();

	if( CheckUpdateFromIni(hwnd, url, remote_file_name, major, minor, revision, prerelease, &continue_down) )
	{
		level = 1;
		sprintf(buffer, pl->GuiMessage(Languages::update_is_new_version), major, minor, revision);
		SetDlgItemText(hwnd, IDC_UPDATE_INFO1, buffer);
	}
	else
	{
		level = 100;
		SetDlgItemText(hwnd, IDC_UPDATE_INFO1, pl->GuiMessage(Languages::update_no_new_version1));
		SetDlgItemText(hwnd, IDC_UPDATE_INFO2, pl->GuiMessage(Languages::update_no_new_version2));
		SetDlgItemText(hwnd, IDC_BUTTON_NEXT, pl->GuiMessage(Languages::update_button_finish));
		ShowWindow(GetDlgItem(hwnd, IDC_PROGRESS), SW_HIDE);
		EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_CANCEL), false);
	}

	EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_NEXT), true);
	EnableWindow(GetDlgItem(hwnd, IDC_UPDATE_INFO1), true);
	ShowWindow(GetDlgItem(hwnd, IDC_PROGRESS), SW_HIDE);
	SetFocus(GetDlgItem(hwnd, IDC_BUTTON_NEXT));
}


bool SaveDialog(HWND hwnd)
{
OPENFILENAME o;
char buf[MAX_PATH], desktop[MAX_PATH];

	if( !remote_file_name.empty() )
		sprintf(buf, remote_file_name.c_str());
	else
	{
		#ifndef TTCALC_PORTABLE
			sprintf(buf, "ttcalc-%d.%d.%d-setup.exe", major, minor, revision);
		#else
				sprintf(buf, "ttcalc-portable-%d.%d.%d-bin.tar.gz", major, minor, revision);
		#endif
	}

	if( !SHGetSpecialFolderPath(hwnd, desktop, CSIDL_DESKTOPDIRECTORY, 0) )
		desktop[0] = 0;

	o.lStructSize       = sizeof(o);
	o.hwndOwner         = hwnd;
	o.hInstance         = GetPrgRes()->GetInstance();
	o.lpstrFilter       = "*.*\0*.*\0";
	o.lpstrCustomFilter = 0;
	o.nMaxCustFilter    = 0;
	o.nFilterIndex      = 1;
	o.lpstrFile         = buf;
	o.nMaxFile          = MAX_PATH;
	o.lpstrFileTitle    = 0;
	o.nMaxFileTitle     = 0;
	o.lpstrInitialDir   = (desktop[0]) ? desktop : 0;
	o.lpstrTitle        = 0;
	o.Flags             = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
	o.nFileOffset       = 0;
	o.nFileExtension    = 0;
	o.lpstrDefExt       = 0;
	o.lCustData         = 0;
	o.lpfnHook          = 0;
	o.lpTemplateName    = 0;


	if( GetSaveFileName(&o) )
	{
		local_file_name = buf;
		return true;
	}

return false;
}



bool DownloadUpdate(HWND hwnd)
{
DownloadProgram down;

	level = -1; // nothing to do when Next is pressed
	continue_down = true;
	EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_NEXT), false);
	Languages * pl = GetPrgRes()->GetLanguages();

	if( SaveDialog(hwnd) )
	{
		SetDlgItemText(hwnd, IDC_UPDATE_INFO1, pl->GuiMessage(Languages::update_download_from));
		SetDlgItemText(hwnd, IDC_UPDATE_INFO2, url.c_str());
		ShowWindow(GetDlgItem(hwnd, IDC_PROGRESS), SW_SHOW);

		down.AttachProgress(GetDlgItem(hwnd, IDC_PROGRESS));
		CheckMessages();

		if( down.DownloadUrl(url.c_str(), &continue_down) )
		{
			SetDlgItemText(hwnd, IDC_UPDATE_INFO1, pl->GuiMessage(Languages::update_downloaded_info1));
			SetDlgItemText(hwnd, IDC_UPDATE_INFO2, pl->GuiMessage(Languages::update_downloaded_info2));

		#ifndef TTCALC_PORTABLE
			level = 2;
		#else
			level = 100;
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_CANCEL), false);
		#endif
		}
		else
		{
			level = 100;
			DeleteFile(local_file_name.c_str());
			SetDlgItemText(hwnd, IDC_UPDATE_INFO1, pl->GuiMessage(Languages::update_download_error));
			SetDlgItemText(hwnd, IDC_UPDATE_INFO2, "");
		}

		ShowWindow(GetDlgItem(hwnd, IDC_PROGRESS), SW_HIDE);
		SetDlgItemText(hwnd, IDC_BUTTON_NEXT, pl->GuiMessage(Languages::update_button_finish));
	}
	else
	{
		level = 100;
		EndDialog(hwnd, 0);
	}

	EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_NEXT), true);
	SetFocus(GetDlgItem(hwnd, IDC_BUTTON_NEXT));

return false;
}








BOOL UpdateInit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Languages * pl = GetPrgRes()->GetLanguages();

	if( level == 0 )
	{
		SetDlgItemText(hwnd, IDC_UPDATE_INFO1, pl->GuiMessage(Languages::update_check_for_info));
	}
	else
	if( level == 1 )
	{
		char buffer[200];

		sprintf(buffer, pl->GuiMessage(Languages::update_is_new_version), major, minor, revision);
		SetDlgItemText(hwnd, IDC_UPDATE_INFO1, buffer);
	}

	SetDlgItemText(hwnd, IDC_UPDATE_INFO2, "");
	SetDlgItemText(hwnd, IDC_BUTTON_NEXT, pl->GuiMessage(Languages::update_button_next));
	SetDlgItemText(hwnd, IDC_BUTTON_CANCEL, pl->GuiMessage(Languages::update_button_cancel));
	SetWindowText(hwnd, pl->GuiMessage(Languages::update_title));
	SetDlgItemText(hwnd, IDC_CHECK_UPDATE_STURTUP, pl->GuiMessage(Languages::update_check_at_startup));

	if( GetPrgRes()->GetCheckUpdateStartup() )
		CheckDlgButton(hwnd, IDC_CHECK_UPDATE_STURTUP, BST_CHECKED);

	ShowWindow(GetDlgItem(hwnd, IDC_PROGRESS), SW_HIDE);

return true;
}


BOOL UpdateCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( LOWORD(wParam)==IDCANCEL || LOWORD(wParam)==IDC_BUTTON_CANCEL )
	{
		continue_down = false;
		EndDialog(hwnd, 0);
		return 0;
	}

	if( LOWORD(wParam) == IDC_CHECK_UPDATE_STURTUP )
	{
		GetPrgRes()->SetCheckUpdateStartup( IsDlgButtonChecked(hwnd, IDC_CHECK_UPDATE_STURTUP)==BST_CHECKED );
	}
	else
	if( LOWORD(wParam) == IDC_BUTTON_NEXT )
	{
		switch(level)
		{
			case 0:
			CheckUpdate(hwnd);
			break;

			case 1:
			DownloadUpdate(hwnd);
			break;

			case 2:
			ShellExecute(0, "open", local_file_name.c_str(), "", "", SW_SHOWNORMAL);
			EndDialog(hwnd, 0);
			PostQuitMessage(0);
			break;

			case 100:
			EndDialog(hwnd, 0);
			break;
		}
		return 0;
	}

	return 1;
}


void CreateUpdateMessagesTable(Messages<BOOL> & messages)
{
	messages.Associate(WM_INITDIALOG,   UpdateInit);
	messages.Associate(WM_COMMAND,		UpdateCommand);
}


BOOL CALLBACK UpdateProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
static Messages<BOOL> messages;

	if( messages.Empty() )
		// initiation
		CreateUpdateMessagesTable(messages);

return messages.Call(msg, hwnd, msg, wParam, lParam);
}




unsigned __stdcall UpdateBackgroundProc(void *)
{
using namespace Background;

	try
	{
		if( CheckUpdateFromIni(0, url_, remote_file_name_, major_, minor_, revision_, prerelease_, 0) )
		{
			HWND main = GetPrgRes()->GetMainWindow();
			PostMessage(main, WM_UPDATE_EXISTS, 0, 0);
		}
	}
	catch(...)
	{
	}

_endthreadex(0);
return 0;
}


} // namespace








void ShowUpdateDialog(HWND hwnd)
{
using namespace Update;

	level = 0;
	local_file_name.clear();
	remote_file_name.clear();
	url.clear();

	DialogBox(GetPrgRes()->GetInstance(), MAKEINTRESOURCE(IDD_UPDATE_DIALOG), hwnd, UpdateProc);
}


void ShowUpdateDialogAfterCheck(HWND hwnd)
{
using namespace Update;

	level = 1;
	local_file_name.clear();

	// make sure that there is a memory barrier before reading next objects (those from Background::)
	// objects from Background:: are set by an another thread at the beginning
	// when we are here then the thread is not writing (it exited)
	remote_file_name = Background::remote_file_name_;
	url      = Background::url_;
	major    = Background::major_;
	minor    = Background::minor_;
	revision = Background::revision_;

	DialogBox(GetPrgRes()->GetInstance(), MAKEINTRESOURCE(IDD_UPDATE_DIALOG), hwnd, UpdateProc);
}


void CheckUpdateInBackground()
{
	time_t last = GetPrgRes()->GetLastUpdate();
	time_t now  = time(0);

	// testing only once a three days (259200 = 60*60*24 * 3)
	if( GetPrgRes()->GetCheckUpdateStartup() && last+259200 < now )
	{
		// this setting can be added also when a user selects manually the test for an update
		GetPrgRes()->SetLastUpdate(now);

		unsigned int thread_id;
		_beginthreadex(0, 0, Update::UpdateBackgroundProc, 0, 0, &thread_id);
	}
}


