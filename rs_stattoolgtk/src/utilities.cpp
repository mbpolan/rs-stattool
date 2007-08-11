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
// utilities.cpp: implementations of extra functions

// necessary includes
#include <sstream>
#include <stdlib.h>
#include <cmath>

// project includes
#include "common.h"
#include "utilities.h"

// (re)allocate memory
void* Utils::alloc(void *ptr, size_t size) {
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}

// convert a glib::ustring to an int
int Utils::ustring_to_int(const Glib::ustring &str) {
	Glib::ustring tmp;
	
	// check skill
	if (str=="-")
		return -1;
	
	// clear out any unnecessary characters
	for (int i=0; i<str.length(); i++) {
		if (str[i]!=',' && str[i]!=' ')
			tmp+=str[i];
	}
	
	// convert to integer
	return atoi(tmp.c_str());
}

// convert an int to a glib::ustring
Glib::ustring Utils::int_to_ustring(int a) {
	std::stringstream ss;
	ss << a;
	
	Glib::ustring str=ss.str();
	return str;
}

// translate an IOHandler error to string
Glib::ustring Utils::translate_io_error(IOHandler::IOError error) {
	Glib::ustring str;
	switch(error) {
		case IOHandler::IO_BAD_HEADER: str="This is not a supported RSP file."; break;
		case IOHandler::IO_BAD_VERSION: str="This file version is unsupported."; break;
		case IOHandler::IO_OPEN_FAIL: str="Unable to open file."; break;
		default: str="Unknown error."; break;
	}
	
	return str;
}

// format a numeric string with comma separators
Glib::ustring Utils::format_numeric(const Glib::ustring &str) {
	Glib::ustring numStr=str;
	
	// after every triplet, insert a comma
	for (int i=str.size()-3; i>=0; i-=3) {
		numStr.insert(i, ",");
	}
	
	// remove surplus commas
	if (numStr[0]==',')
		numStr.erase(0, 1);
	
	return numStr;
	
}
