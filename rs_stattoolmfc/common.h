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
// common.h: common definitions and classes

#ifndef COMMON_H
#define COMMON_H

// includes
#include "stdafx.h"
#include <iostream>

// custom messages for Win32 thread com
#define WM_RSTHREAD_STARTED (WM_USER+0x101)
#define WM_RSTHREAD_FINISHED (WM_USER+0x102)

// url to hi score server
#define RS_HI_SCORES_URL "http://hiscore.runescape.com/lang/en/aff/runescape/hiscorepersonal.ws"

// maximum characters allowed in player's name
#define RS_MAX_PLAYER_NAME_LENGTH	12

// total amount of skills
#define SKILL_COUNT	23

// save file magic number and version
#define RSP_FILE_HEADER		"RSP"
#define RSP_FILE_VERSION	11

// compare dialog dimensions
#define COMPARE_DIALOG_WIDTH	520
#define COMPARE_DIALOG_HEIGHT	460

// skill data
typedef struct {
	// name
	CString name;
	
	// level
	CString level;
	
	// rank
	CString rank;
	
	// xp
	CString xp;
} SkillData;

// player data
typedef struct {
	// player name
	CString name;
	
	// overall total level
	CString overallLvl;
	
	// overall Exp
	CString overallExp;
	
	// overall rank
	CString overallRank;
	
	// skills
	SkillData skills[SKILL_COUNT];

	// time stamp
	CString timestamp;
} PlayerData;

// transfer data from parser
typedef struct {
	// the data recieved
	char *data;
	
	// curl transfer code, if any
	int code;
} TransferData;

/////////////////////////////////////////////
// memory management
/////////////////////////////////////////////

// memory chunk
class MemChunk {
	public:
		// constructor
		MemChunk() {
			m_Data=NULL;
			m_Size=0;
		};
		
		// get the data buffer
		char* GetData() { return m_Data; };
		
		// get the data size
		size_t GetSize() { return m_Size; };
		
		// set the data
		void SetData(char *nData) { m_Data=nData; };
		
		// set the size
		void SetSize(size_t nSize) { m_Size=nSize; };
		
	protected:
		// data buffer
		char *m_Data;
		
		// size of data
		size_t m_Size;
};

#endif
