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
// playerview.h: custom Gtk::Notebook for displaying players

#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

// gtk includes
#include <gtkmm/notebook.h>

// stl includes
#include <map>

// project includes
#include "clistview.h"
#include "common.h"

// forward declarations
class Gtk::Menu;
class InfoDialog;

// custom player notebook widget
class PlayerView: public Gtk::Notebook {
	public:
		// constructor
		PlayerView();
		
		// destructor
		~PlayerView();
		
		// add a player tab
		void add_player_tab(const Glib::ustring &name, const PlayerData &data);
		
		// get player data struct
		PlayerData get_player_data(const Glib::ustring &name);
		
		// get a player's list
		CListView* get_player_list(const Glib::ustring &name);
		
		// see if a player has a tab
		bool player_has_tab(const Glib::ustring &name);
		
		// return player map
		typedef std::map<Glib::ustring, CListView*> playerMap;
		playerMap get_map() const { return m_PlayerMap; };
		
		///////////////////////////////////////
		// signals
		///////////////////////////////////////
		
		// refresh player signal
		sigc::signal<void, const Glib::ustring&> signal_refresh_player;
		
		// save stats request signal
		sigc::signal<void, PlayerData&> signal_save_stats_request;
		
		// show player info
		void on_list_info();
		
		// save player stats
		void on_list_save_stats();
		
		// refresh the view
		void on_list_refresh();
		
		// close the tab
		void on_list_close();
	
	protected:
		// button press event handler
		virtual bool on_button_press_event(GdkEventButton*);
		
		// check if the current page consists of a player
		bool current_page_is_player();
		
		// enable or disable all popup menu items
		void toggle_menu_items(bool enable);
		
		// player map
		playerMap m_PlayerMap;
		
		// popup menu
		Gtk::Menu *m_PopupMenu;
		
		// player info dialog
		InfoDialog *m_InfoDialog;
};

#endif
