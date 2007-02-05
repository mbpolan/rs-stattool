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
// io.h: I/O functions

#ifndef IO_H
#define IO_H

#include "common.h"

// class for managing I/O operations
class IOHandler {
	public:
		// i/o errors
		enum IOError { IO_NO_ERROR=0, IO_OPEN_FAIL=-1, IO_BAD_HEADER=-2, IO_BAD_VERSION=-3 };
		
		// write a string value
		static void writeString(FILE *f, const CString &path);
		
		// read a string value
		static CString readString(FILE *f);
		
		// save player stats to file
		static bool savePlayerStats(const CString &path, PlayerData pd);
		
		// load player stats from file
		static bool loadPlayerStats(const CString &path, PlayerData &pd);
		
		// global IO error
		static IOError Error;
};

#endif
