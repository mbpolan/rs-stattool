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
// rsparser.h: the core HTML and Network I/O class

#ifndef RSPARSER_H
#define RSPARSER_H

#include <curl/curl.h>
#include "common.h"
#include "playernotebook.h"

// rs html parser
class RSParser {
	public:
		// struct used in thread func arguement
		struct CurlStruct {
			HWND diag;
			RSParser *parser; // this
			CString pname;
		};

		// constructor
		RSParser();

		// get the player's high scores data
		void getPlayerData(const CString &name, CDialog *md);

		// validate a player's name
		static CString validateName(const CString &name);

		// parse html data
		static PlayerData parseHTML(char *data, bool *ok);

		// curl data from previous session
		MemChunk m_Chunk;
		TransferData m_Data;
		
	protected:
		// thread function for network i/o
		static UINT __cdecl threadGetPlayerData(LPVOID pParam);

		// writing function for curl write-data callback
		static size_t curlWriteFunc(void *ptr, size_t size, size_t nmemb, void *userp);

		// the worker thread
		CWinThread *m_Thread;
};

#endif
