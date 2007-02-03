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
// io.cpp: implementation of IOHandler class

#include <glibmm.h>
#include <cstdio>
#include <ctime>
#include "io.h"

IOHandler::IOError IOHandler::Error=IOHandler::IO_NO_ERROR;

// write a string value
void IOHandler::write_string(FILE *f, const Glib::ustring &str) {
	// first write the length
	int len=str.size();
	fwrite((int*) &len, sizeof(int), 1, f);
	
	// write the string itself
	for (int i=0; i<len; i++)
		fputc(str[i], f);
};

// read a string value
Glib::ustring IOHandler::read_string(FILE *f) {
	Glib::ustring str="";
	
	// read the length
	int len;
	fread((int*) &len, sizeof(int), 1, f);
	
	// read in the string
	for (int i=0; i<len; i++)
		str+=(char) fgetc(f);
	
	return str;
};

// save player stats to file
bool IOHandler::save_player_stats(const Glib::ustring &path, PlayerData pd) {
	// open the file
	FILE *f=fopen(path.c_str(), "wb");
	if (!f) {
		IOHandler::Error=IO_OPEN_FAIL;
		return false;
	}
	
	// initialize header variables
	char mn[4]=RSP_FILE_HEADER;
	short version=RSP_FILE_VERSION;
	
	// write header
	fwrite((char*) mn, sizeof(char), 3, f);
	fwrite((short*) &version, sizeof(short), 1, f);
	
	// write the timestamp
	time_t curTime=time(NULL);
	pd.timestamp=ctime(&curTime);
	write_string(f, pd.timestamp);
	
	// write player name length
	write_string(f, pd.name);
	
	// write rank, level, and exp
	write_string(f, pd.overallRank);
	write_string(f, pd.overallLvl);
	write_string(f, pd.overallExp);
	
	// iterate over skills
	for (int i=0; i<SKILL_COUNT; i++) {
		SkillData s=pd.skills[i];
		
		// write skill data
		write_string(f, s.name);
		write_string(f, s.rank);
		write_string(f, s.level);
		write_string(f, s.xp);
	}
	
	// wrap up
	fclose(f);
	IOHandler::Error=IOHandler::IO_NO_ERROR;
	return true;
};

// load player stats from file
bool IOHandler::load_player_stats(const Glib::ustring &path, PlayerData &pd) {
	// open the file
	FILE *f=fopen(path.c_str(), "rb");
	if (!f) {
		IOHandler::Error=IOHandler::IO_OPEN_FAIL;
		return false;
	}
	
	// read in file header
	char mn[3];
	short version;
	fread((char*) mn, sizeof(char), 3, f);
	fread((short*) &version, sizeof(short), 1, f);
	
	// check the header
	if (strncmp(mn, RSP_FILE_HEADER, 3)!=0) {
		fclose(f);
		IOHandler::Error=IOHandler::IO_BAD_HEADER;
		return false;
	}
	
	// check the version
	if (version!=RSP_FILE_VERSION) {
		fclose(f);
		IOHandler::Error=IOHandler::IO_BAD_VERSION;
		return false;
	}
	
	// read the timestamp
	pd.timestamp=read_string(f);
	
	// read name
	pd.name=read_string(f);
	
	// read totals: rank, level, exp
	pd.overallRank=read_string(f);
	pd.overallLvl=read_string(f);
	pd.overallExp=read_string(f);
	
	// read in skills
	for (int i=0; i<SKILL_COUNT; i++) {
		SkillData s;
		
		// read skill data
		s.name=read_string(f);
		s.rank=read_string(f);
		s.level=read_string(f);
		s.xp=read_string(f);
		
		pd.skills[i]=s;
	}
	
	// wrap up
	fclose(f);
	IOHandler::Error=IOHandler::IO_NO_ERROR;
	return true;
};
