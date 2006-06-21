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
// rsparser.h: parser for runescape hi scores html

#ifndef RSPARSER_H
#define RSPARSER_H

// necessary includes
#include <curl/curl.h>
#include <glibmm/thread.h>
#include <gtkmm.h>
#include <vector>

// project includes
#include "common.h"

// rs html parser
class RSParser {
	public:
		// constructor
		RSParser();
		
		// get data about a player
		// emits the data_ready signal when done
		void get_player_data(const Glib::ustring &name);
		
		// parse html data
		PlayerData parse_html(char *data, bool *ok);
		
		//////////////////////////////////////////////////
		// signals
		//////////////////////////////////////////////////
		// signal emitted when transfer has begun
		sigc::signal<void> signal_transfer_start;
		
		// signal emitted when progress is reported
		sigc::signal<void, double, double> signal_data_progress;
		
		// signal emitted when player data is recieved
		sigc::signal<void, int, char*> signal_data_ready;
		
	protected:
		// get player data thread funcion
		void thread_get_player_data(const Glib::ustring &name);
		
		// curl progress handler
		static int curl_progess_func(void *userp, double t, double d,
					    double ultotal, double ulnow);
		
		// curl write function
		static size_t curl_write_func(void *ptr, size_t size, size_t nmemb, void *userp);
		
		// validate player's name
		Glib::ustring validate_name(const Glib::ustring &name);
		
		// curl data from previous session
		MemChunk m_Chunk;
		
		// Threads
		std::vector<Glib::Thread*> m_Threads;
};

#endif
