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

#include <cstdio>
#include <ctime>
#include "stdafx.h"
#include "io.h"

IOHandler::IOError IOHandler::Error=IOHandler::IO_NO_ERROR;

// write a string value
void IOHandler::WriteString(FILE *f, const CString &str) {
	if (!f)
		return;

	// first write the length
	int len=str.GetLength();
	fwrite((int*) &len, sizeof(int), 1, f);
	
	// write the string itself
	for (int i=0; i<len; i++)
		fputc(str[i], f);
}

// read a string value
CString IOHandler::ReadString(FILE *f) {
	if (!f)
		return "";

	CString str="";
	
	// read the length
	int len;
	fread((int*) &len, sizeof(int), 1, f);
	
	// read in the string
	for (int i=0; i<len; i++)
		str+=(char) fgetc(f);
	
	return str;
}

// save player stats to file
bool IOHandler::SavePlayerStats(const CString &path, PlayerData pd) {
	// open the file
	FILE *f=fopen(path, "wb");
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
	time_t ts=time(NULL);
	CString cstrTime=ctime(&ts);
	WriteString(f, cstrTime);
	
	// write player name length
	WriteString(f, pd.name);
	
	// write rank, level, and exp
	WriteString(f, pd.overallRank);
	WriteString(f, pd.overallLvl);
	WriteString(f, pd.overallExp);
	
	// iterate over skills
	for (int i=0; i<SKILL_COUNT; i++) {
		SkillData s=pd.skills[i];
		
		// write skill data
		WriteString(f, s.name);
		WriteString(f, s.rank);
		WriteString(f, s.level);
		WriteString(f, s.xp);
	}
	
	// wrap up
	fclose(f);
	IOHandler::Error=IOHandler::IO_NO_ERROR;
	return true;
}

// load player stats from file
bool IOHandler::LoadPlayerStats(const CString &path, PlayerData &pd) {
	// open the file
	FILE *f=fopen(path, "rb");
	if (!f) {
		IOHandler::Error=IOHandler::IO_OPEN_FAIL;
		return false;
	}
	
	// read in file header
	char mn[4];
	short version;
	fread((char*) mn, sizeof(char), 3, f);
	fread((short*) &version, sizeof(short), 1, f);
	
	// check the header
	if (strncmp(mn, RSP_FILE_HEADER, 3)!=0) {
		fclose(f);
		IOHandler::Error=IOHandler::IO_BAD_HEADER;
		return false;
	}
	
	// check version
	if(version!=RSP_FILE_VERSION) {
		fclose(f);
		IOHandler::Error=IOHandler::IO_BAD_VERSION;
		return false;
	 }
	
	// read the timestamp
	pd.timestamp=ReadString(f);
	
	// read name
	pd.name=ReadString(f);
	
	// read totals: rank, level, exp
	pd.overallRank=ReadString(f);
	pd.overallLvl=ReadString(f);
	pd.overallExp=ReadString(f);
	
	// read in skills
	for (int i=0; i<SKILL_COUNT; i++) {
		SkillData s;
		
		// read skill data
		s.name=ReadString(f);
		s.rank=ReadString(f);
		s.level=ReadString(f);
		s.xp=ReadString(f);
		
		pd.skills[i]=s;
	}
	
	// wrap up
	fclose(f);
	IOHandler::Error=IOHandler::IO_NO_ERROR;
	return true;
}
