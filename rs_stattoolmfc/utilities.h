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
// utilities.h: various functions

#ifndef UTILITIES_H
#define UTILITIES_H

#include "io.h"
#include "stdafx.h"

// Utilities namespace
namespace Utils {

// allocate memory
void* Alloc(void *ptr, size_t size);

// convert a CString to an int
int CStringToInt(const CString &str);

// convert a CString to a long
long CStringToLong(const CString &str);

// convert an int to a CString
CString IntToCString(int a);

// translate an IOHandler to string
CString TranslateIOError(IOHandler::IOError error);

// set the windows xp theme for a window
void SetWindowsXPTheme(HWND *hwnd);

}; // namespace Utils

#endif
