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
// rsparser.cpp: implementation of RSParser class

#include "stdafx.h"
#include "rsparser.h"
#include "utilities.h"

struct tf {
	MemChunk chunk;
	TransferData data;
};

// constructor
RSParser::RSParser() {
}

// get the player's high scores data
void RSParser::GetPlayerData(const CString &name, CDialog *md) {
	struct CurlStruct *cs=new struct CurlStruct;
	cs->diag=md->GetSafeHwnd();
	cs->parser=this;
	cs->pname=name;

	// create a new thread
	m_Thread=AfxBeginThread(ThreadGetPlayerData, (LPVOID) cs);
}

// calculate a player's total exp
int RSParser::CalculateTotalExp(const PlayerData &pd) {
	int totalExp=0;

	// go over each skill
	for (int i=0; i<SKILL_COUNT; i++) {
		SkillData s=pd.skills[i];

		// convert the string exp to an integer
		int texp=Utils::CStringToInt(s.xp);
		totalExp+=texp;
	}

	return totalExp;
}

// calculate a player's total level
int RSParser::CalculateTotalLevel(const PlayerData &pd) {
	int totalLvl=0;

	// go over each skill
	for (int i=0; i<SKILL_COUNT; i++) {
		SkillData s=pd.skills[i];

		// convert the string exp to an integer
		int tlvl=Utils::CStringToInt(s.level);
		totalLvl+=tlvl;
	}

	return totalLvl;
}

// validate a player's name
CString RSParser::ValidateName(const CString &name) {
	CString ret=name;

	// first, bring the string to lowercase
	ret=ret.MakeLower();

	// replace all instances of ' ' with underscores
	ret.Replace(" ", "_");

	return ret;
}

// parse html data
PlayerData RSParser::ParseHTML(char *data, bool *ok) {
	PlayerData player;

	// make sure the string is valid
	if (!data) {
		AfxMessageBox(_T("Internal error: HTML string is not valid!"), MB_OK);
		*ok=false;
		return player;
	}
	
	// form a ustring from data
	CString html=CString(data);
	
	// first check if this player is on the high scores
	if (html.Find("does not feature in the hiscores.")!=-1) {
		*ok=false;
		return player;
	}
	
	else
		*ok=true;
	
	// string names of skills
	char *skills[] = { "Attack", "Defence", "Strength", "Hitpoints", "Ranged",
		"Prayer", "Magic", "Cooking", "Woodcutting", "Fletching",
		"Fishing", "Firemaking", "Crafting", "Smithing", "Mining",
		"Herblore", "Agility", "Thieving", "Slayer", "Farming",
		"Runecraft", "Hunter", "Construction" };
		
	// get the name
	int npos1=html.Find("Personal scores for ");
	html.Delete(0, npos1+20);
	npos1=html.Find("</b>");
	player.name=CString(html.Mid(0, npos1));

	// iterate over skills
	for (int i=0; i<SKILL_COUNT; i++) {
		SkillData s;
		
		// find the skill
		int pos;
		if ((pos=html.Find(skills[i]))!=-1) {
			// assign name
			s.name=skills[i];
			int skillLength=(int) strlen(skills[i]);
			
			// erase string up to this point
			html.Delete(0, pos);
			
			// 29 bytes+skillLength to rank
			html.Delete(0, skillLength+29);
			
			// find </td> tag
			int tpos=html.Find("</td>");
			
			// grab the rank
			s.rank=html.Mid(0, tpos);
			
			// check if this skill is actually on the page
			// FIXME: this is too hacker-ish, gotta improve it :)
			bool nr=false;
			int op=atoi(s.rank.Mid(0, 1));
			if (op==0) {
				s.rank="-";
				nr=true;
			}
			
			// 24 bytes+rank to level
			html.Delete(0, tpos+24);
			
			// find </td> tag
			if ((tpos=html.Find("</td>"))<0)
				continue;
			
			// grab the level
			s.level=(nr ? "-" : html.Mid(0, tpos));
			
			// 24 bytes+level to xp
			html.Delete(0, 24+(atoi((const char*) s.level)>=10 ? 2 : 1));
			
			// find </td> tag
			if ((tpos=html.Find("</td>"))<0)
				continue;
			
			// grab the xp
			s.xp=(nr ? "-" : html.Mid(0, tpos));
			
			// add the skill to the player's list
			player.skills[i]=s;
		}
		
		else {
			// couldn't find a skill?
			char buf[256];
			sprintf(buf, "Unable to find skill %s in data!", skills[i]);
			AfxMessageBox(buf, MB_OK);
		}
	}
	
	return player;
}

// thread function for network i/o
UINT RSParser::ThreadGetPlayerData(LPVOID pParam) {
	// cast the parameter to the PlayerNotebook widget
	struct CurlStruct *cs=static_cast<struct CurlStruct*> (pParam);

	// cast the parser object
	RSParser *p=cs->parser;
	
	// get curl handle
	CURL *handle=curl_easy_init();

	// reset the data
	p->m_Chunk.SetData(NULL);
	p->m_Chunk.SetSize(0);
	p->m_Data.code=-1;
	
	// post fields
	CString pfields="user1=";
	pfields+=ValidateName(cs->pname);
	
	// set options
	curl_easy_setopt(handle, CURLOPT_POSTFIELDS, (const char*) pfields);
	curl_easy_setopt(handle, CURLOPT_URL, RS_HI_SCORES_URL);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &RSParser::CurlWriteFunc);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*) &p->m_Chunk);

	// alert the application that data transfer has started
	PostMessage(cs->diag, WM_RSTHREAD_STARTED, 0, 0);
	
	// perform curl session
	CURLcode code=curl_easy_perform(handle);
	
	// set transfer stats
	p->m_Data.code=code;
	p->m_Data.data=p->m_Chunk.GetData();
	
	// clean up
	curl_easy_cleanup(handle);

	// alert the application
	PostMessage(cs->diag, WM_RSTHREAD_FINISHED, 0, 0);

	// make sure the free the memory used in the data struct
	delete cs;

	return 0;
}

// writing function for curl write-data callback
size_t RSParser::CurlWriteFunc(void *ptr, size_t size, size_t nmemb, void *userp) {
	// calculate the real size
	size_t rsize=size*nmemb;
	
	// get our memory chunk
	MemChunk *chunk=(MemChunk*) userp;
	char *data=chunk->GetData();
	size_t csize=chunk->GetSize();
	
	// reallocate the data
	data=(char*) Utils::Alloc(data, csize+rsize+1);
	if (data) {
		// copy the new data
		memcpy(&(data[csize]), ptr, rsize);
		
		// increase chunk size
		csize+=rsize;
		
		// pad the last byte with 0
		data[csize]=0;
	}
	
	// set new size
	chunk->SetSize(csize);
	chunk->SetData(data);
	
	// return the actual size
	return rsize;
}
