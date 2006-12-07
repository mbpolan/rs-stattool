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

// gtk includes
#include <glibmm/thread.h>
#include <glibmm/ustring.h>

// url to hi score server
#define RS_HI_SCORES_URL "http://hiscore.runescape.com/lang/en/aff/runescape/hiscorepersonal.ws"

// maximum characters allowed in player's name
#define RS_MAX_PLAYER_NAME_LENGTH	12

// total amount of skills
#define SKILL_COUNT	23

// tree view colors
#define COMPARE_COLOR_LOW	"#de4040"
#define COMPARE_COLOR_HI	"#5ade40"
#define COMPARE_COLOR_EVEN	"#e8e773"
#define COMPARE_COLOR_GRAY	"#778899"

// compare dialog dimensions
#define COMPARE_DIALOG_WIDTH	520
#define COMPARE_DIALOG_HEIGHT	460

// skill data
typedef struct {
	// name
	Glib::ustring name;
	
	// level
	Glib::ustring level;
	
	// rank
	Glib::ustring rank;
	
	// xp
	Glib::ustring xp;
} SkillData;

// player data
typedef struct {
	// player name
	Glib::ustring name;
	
	// overall total level
	Glib::ustring overallLvl;
	
	// overall Exp
	Glib::ustring overallExp;
	
	// overall rank
	Glib::ustring overallRank;
	
	// skills
	SkillData skills[SKILL_COUNT];
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
		
		// lock the mutex
		void lock() { m_Mutex.lock(); };
		
		// unlock the mutex
		void unlock() { m_Mutex.unlock(); };
		
		// get the data buffer
		char* get_data() { return m_Data; };
		
		// get the data size
		size_t get_size() { return m_Size; };
		
		// set the data
		void set_data(char *nData) { m_Data=nData; };
		
		// set the size
		void set_size(size_t nSize) { m_Size=nSize; };
		
	protected:
		// mutex
		Glib::Mutex m_Mutex;
		
		// data buffer
		char *m_Data;
		
		// size of data
		size_t m_Size;
};

#endif
