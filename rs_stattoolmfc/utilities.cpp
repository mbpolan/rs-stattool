/***************************************************************************
 *   Copyright (C) 2006 by Mike Polan                                      *
 *   kanadakid@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
// utilities.cpp: implementation of Utils namespace

#include "stdafx.h"

#include <sstream>
#include <cstdlib>
#include "utilities.h"

// allocate memory
void* Utils::Alloc(void *ptr, size_t size) {
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}

// convert a CString to an int
int Utils::CStringToInt(const CString &str) {
	CString tmp;
	
	// check skill
	if (str=="-")
		return -1;
	
	// clear out any unnecessary characters
	for (int i=0; i<str.GetLength(); i++) {
		if (str[i]!=',' && str[i]!=' ')
			tmp+=str[i];
	}
	
	// convert to integer
	return atoi((const char*) tmp);
}

// convert a CString to a long
long Utils::CStringToLong(const CString &str) {
	CString tmp;

	if (str=="-")
		return -1;

	// clear out any unnecessary characters
	for (int i=0; i<str.GetLength(); i++) {
		if (str[i]!=',' && str[i]!=' ')
			tmp+=str[i];
	}

	// convert to long
	return atol((const char*) tmp);
}

// convert an int to a CString
CString Utils::IntToCString(int a) {
	std::stringstream ss;
	ss << a;
	
	CString str=CString(ss.str().c_str());
	return str;
}

// translate an IOHandler to string
CString Utils::TranslateIOError(IOHandler::IOError error) {
	CString str;
	switch(error) {
		case IOHandler::IO_BAD_HEADER: str="This is not a supported RSP file."; break;
		case IOHandler::IO_BAD_VERSION: str="This file version is unsupported."; break;
		case IOHandler::IO_OPEN_FAIL: str="Unable to open file."; break;
		default: str="Unknown error."; break;
	}
	
	return str;
}

// set the Windows XP theme for a window
void Utils::SetWindowsXPTheme(HWND *hwnd) {
	// first of all, see if we are on windows xp
	HINSTANCE themeDllHandle = LoadLibrary( _T("UxTheme.dll") );
	if(themeDllHandle) {
		// get the address of the dialog texture function
		EnableThemeDialogTextureFn* pEnableThemeDialogTexture=
			reinterpret_cast<EnableThemeDialogTextureFn*>(GetProcAddress(
			themeDllHandle, "EnableThemeDialogTexture"));
		
		// call it, if the pointer is valid
        if(pEnableThemeDialogTexture)
			pEnableThemeDialogTexture(*hwnd, ETDT_ENABLETAB);
		
		FreeLibrary(themeDllHandle);
	}
}
