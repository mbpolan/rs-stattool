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
// rsparser.cpp: implementations of RSParser class

// necessary includes
#include <curl/curl.h>
#include <gtkmm/dialog.h>
#include <gtkmm/progressbar.h>

// project includes
#include "rsparser.h"
#include "utilities.h"

// constructor
RSParser::RSParser() {
};

void RSParser::get_player_data(const Glib::ustring &name) {
	// create a new handler thread
	m_Threads.push_back(Glib::Thread::create(
			sigc::bind(sigc::mem_fun(*this, &RSParser::thread_get_player_data), name),
			false));
	
	// connect dispatcher signals
	dispatcher_transfer_start.connect(sigc::mem_fun(*this, &RSParser::on_process_transfer_start));
	dispatcher_data_ready.connect(sigc::mem_fun(*this, &RSParser::on_process_data_ready));
};

// parse html data
PlayerData RSParser::parse_html(char *data, bool *ok) {
	PlayerData player;
	
	// form a ustring from data
	Glib::ustring html=Glib::ustring(data);
	
	// first check if this player is on the high scores
	if (html.find("does not feature in the hiscores.")!=-1) {
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
		"Runecraft", "Construction" };
		
	// get the name
	int npos1=html.find("Personal scores for ");
	html.erase(0, npos1+20);
	npos1=html.find("</b>");
	player.name=html.substr(0, npos1);
	
	// iterate over skills
	for (int i=0; i<SKILL_COUNT; i++) {
		SkillData s;
		
		// find the skill
		int pos;
		if ((pos=html.find(skills[i]))!=-1) {
			// assign name
			s.name=skills[i];
			int skillLength=strlen(skills[i]);
			
			// erase string up to this point
			html.erase(0, pos);
			
			// 28 bytes+skillLength to rank
			html.erase(0, skillLength+28);
			
			// find </td> tag
			int tpos=html.find("</td>");
			
			// grab the rank
			s.rank=html.substr(0, tpos);
			
			// check if this skill is actually on the page
			// FIXME: this is too hacker-ish, gotta improve it :)
			bool nr=false;
			int op=atoi(s.rank.substr(0, 1).c_str());
			if (op==0) {
				s.rank="-";
				nr=true;
			}
			
			// 24 bytes+rank to level
			html.erase(0, tpos+24);
			
			// find </td> tag
			tpos=html.find("</td>");
			
			// grab the level
			s.level=(nr ? "-" : html.substr(0, tpos));
			
			// 24 bytes+level to xp
			html.erase(0, 24+(atoi(s.level.c_str())>=10 ? 2 : 1));
			
			// find </td> tag
			tpos=html.find("</td>");
			
			// grab the xp
			s.xp=(nr ? "-" : html.substr(0, tpos));
			
			// add the skill to the player's list
			player.skills[i]=s;
		}
		
		else
			g_message("Unable to find skill %s\n", skills[i]);
	}
	
	return player;
};

void RSParser::on_process_transfer_start() {
	// emit the actual transfer_start signal
	signal_transfer_start.emit();
};

void RSParser::on_process_data_ready() {
	// emit actual data_ready signal
	signal_data_ready.emit(m_Data.code, (m_Data.data ? m_Data.data : NULL));
};

// get player data thread routine
void RSParser::thread_get_player_data(const Glib::ustring &name) {
	// clear chunk
	m_Chunk.set_data(NULL);
	m_Chunk.set_size(0);
	
	// get curl handle
	CURL *handle=curl_easy_init();
	
	// post fields
	Glib::ustring pfields="user1=";
	pfields+=validate_name(name);
	
	// set options
	curl_easy_setopt(handle, CURLOPT_POSTFIELDS, pfields.c_str());
	curl_easy_setopt(handle, CURLOPT_URL, RS_HI_SCORES_URL);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &RSParser::curl_write_func);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*) &m_Chunk);
	
	// emit transfer start signal
	dispatcher_transfer_start.emit();
	
	// perform curl session
	CURLcode code=curl_easy_perform(handle);
	
	// set transfer stats
	m_Data.code=code;
	m_Data.data=m_Chunk.get_data();
	
	// emit data done signal
	dispatcher_data_ready.emit();
	
	// clean up
	curl_easy_cleanup(handle);
};

size_t RSParser::curl_write_func(void *ptr, size_t size, size_t nmemb, void *userp) {
	// calculate the real size
	size_t rsize=size*nmemb;
	
	// get our memory chunk
	MemChunk *chunk=(MemChunk*) userp;
	char *data=chunk->get_data();
	size_t csize=chunk->get_size();
	
	// lock the mutex
	chunk->lock();
	
	// reallocate the data
	data=(char*) Utils::alloc(data, csize+rsize+1);
	if (data) {
		// copy the new data
		memcpy(&(data[csize]), ptr, rsize);
		
		// increase chunk size
		csize+=rsize;
		
		// pad the last byte with 0
		data[csize]=0;
	}
	
	// set new size
	chunk->set_size(csize);
	chunk->set_data(data);
	
	// free the lock
	chunk->unlock();
	
	// return the actual size
	return rsize;
};

// validate player's name
Glib::ustring RSParser::validate_name(const Glib::ustring &name) {
	Glib::ustring vname;
	const char *cName=name.c_str();
	
	// format the string
	for (int i=0; i<strlen(cName); i++) {
		// lowercase
		char ch=tolower(cName[i]);
		
		// replace space with underscore
		if (ch==' ')
			ch='_';
		
		// append the character
		vname+=ch;
	}
	
	return vname;
};
