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
// playerview.cpp: implementations of PlayerView widget

// necessary includes
#include <gtkmm/menu.h>
#include <gtkmm/scrolledwindow.h>

// project includes
#include "infodialog.h"
#include "playerview.h"
#include "utilities.h"

// constructor
PlayerView::PlayerView() {
	// set up menu
	m_PopupMenu=new Gtk::Menu;
	
	// get item list
	Gtk::Menu::MenuList &list=m_PopupMenu->items();
	
	// add items
	list.push_back(Gtk::Menu_Helpers::MenuElem("_View Info",
		       sigc::mem_fun(*this, &PlayerView::on_list_info)));
	list.push_back(Gtk::Menu_Helpers::MenuElem("_Save Stats",
		       sigc::mem_fun(*this, &PlayerView::on_list_save_stats)));
	list.push_back(Gtk::Menu_Helpers::MenuElem("_Refresh", 
		       sigc::mem_fun(*this, &PlayerView::on_list_refresh)));
	list.push_back(Gtk::Menu_Helpers::MenuElem("_Close",
		       sigc::mem_fun(*this, &PlayerView::on_list_close)));
	
	// accelerate menu
	m_PopupMenu->accelerate(*this);
	
	// allocate dialog
	m_InfoDialog=new InfoDialog;
}

// destructor
PlayerView::~PlayerView() {
	delete m_PopupMenu;
	delete m_InfoDialog;
}

// add a player tab
void PlayerView::add_player_tab(const Glib::ustring &name, const PlayerData &data) {
	// player has tab
	if (m_PlayerMap.find(name)!=m_PlayerMap.end()) {
		// update list view
		CListView *list=m_PlayerMap[name];
		list->clear();
		list->populate(data);
	}
	
	// make a new tab
	else {
		// scrolled window
		Gtk::ScrolledWindow *sw=manage(new Gtk::ScrolledWindow);
		sw->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_ALWAYS);
		
		// allocate new list
		CListView *list=manage(new CListView);
		
		// populate it
		list->populate(data);
		
		// add list to the window
		sw->add(*list);
		
		// add a new tab
		append_page(*sw, name);
		
		// add player to map
		m_PlayerMap[name]=list;
		
		// refresh the view
		show_all_children();
	}
}

// get player data struct
PlayerData PlayerView::get_player_data(const Glib::ustring &name) {
	// player data struct
	PlayerData player;
	player.name=name;
	
	// first get the right tab
	Gtk::ScrolledWindow *sw;
	Gtk::Notebook::PageList list=pages();
	for (Gtk::Notebook::PageList::iterator it=list.begin(); it!=list.end(); ++it) {
		Gtk::Widget *w=(*it)->get_child();
		if (w) {
			// get label
			Glib::ustring l=get_tab_label_text(*w);
			if (l==name) {
				sw=(Gtk::ScrolledWindow*) w;
				break;
			}
		}
	}
	
	// see if we found the tab
	if (sw) {
		// get list view
		CListView *list=(CListView*) sw->get_child();
		Gtk::TreeModel::Children lrows=list->get_rows();
		Gtk::TreeModel::Children::iterator it=lrows.begin();
		
		// get overall data
		list->get_overall_data(player.overallLvl, player.overallExp, player.overallRank);
		
		// iterate over rows
		int c=0;
		for (; it!=lrows.end(); ++it) {
			// skill struct
			SkillData skill;
			
			// get name
			skill.name=(*it)[list->m_Columns.m_Skill];
			
			// get level
			Glib::ustring lvl=(*it)[list->m_Columns.m_Level];
			skill.level=lvl;
			
			// convert to integer
			int ilvl=Utils::ustring_to_int(lvl);
			
			// get exp
			Glib::ustring exp=(*it)[list->m_Columns.m_Exp];
			skill.xp=exp;
			
			// convert to integer
			int iexp=Utils::ustring_to_int(exp);
			
			// get rank
			skill.rank=(*it)[list->m_Columns.m_Rank];
			
			// add skill
			player.skills[c++]=skill;
		}
		
		// no timestamp
		player.timestamp="-";
	}
	
	return player;
}

// show player info
void PlayerView::on_list_info() {
	// check current page
	if (!current_page_is_player())
		return;
	
	// get name of player
	int index=get_current_page();
	Gtk::Widget *w=get_nth_page(index);
	Glib::ustring name=get_tab_label_text(*w);
	
	// get player struct
	PlayerData player=get_player_data(name);
	
	// display dialog
	m_InfoDialog->set_player(player);
	m_InfoDialog->run();
}

// save player stats
void PlayerView::on_list_save_stats() {
	if (!current_page_is_player())
		return;
	
	// get the current player
	int index=get_current_page();
	Gtk::Widget *w=get_nth_page(index);
	Glib::ustring name=get_tab_label_text(*w);
	
	// get player struct
	PlayerData player=get_player_data(name);
	
	// emit save signal
	signal_save_stats_request.emit(player);
}

// refresh the view
void PlayerView::on_list_refresh() {
	// get current player
	Gtk::Widget *w=get_nth_page(get_current_page());
	if (w) {
		// get the player's name
		Glib::ustring name=get_tab_label_text(*w);
		
		// check the name; must be a player
		if (name!="RS Stat Tool") {
			// emit refresh signal
			signal_refresh_player.emit(name);
		}
	}
}

// close the tab
void PlayerView::on_list_close() {
	// first check if a player page is open
	Gtk::Widget *w=get_nth_page(get_current_page());
	if (w) {
		// get tab name
		Glib::ustring name=get_tab_label_text(*w);
		
		// check name
		if (name!="RS Stat Tool") {
			// close the page
			remove_page(*w);
			
			// remove from map
			m_PlayerMap.erase(name);
		}
	}
}

// button press event handler
bool PlayerView::on_button_press_event(GdkEventButton *e) {
	// call base class handler
	bool ret=Gtk::Notebook::on_button_press_event(e);
	
	// check button
	if (e->type==GDK_BUTTON_PRESS && e->button==3) {
		// enable/disable options
		if (!current_page_is_player())
			toggle_menu_items(false);
		else
			toggle_menu_items(true);
		
		// popup menu
		m_PopupMenu->popup(e->button, e->time);
	}
}

// check if the current page consists of a player
bool PlayerView::current_page_is_player() {
	// first check if a player page is open
	Gtk::Widget *w=get_nth_page(get_current_page());
	if (w) {
		// get tab name
		Glib::ustring name=get_tab_label_text(*w);
		
		// check name
		return (name!="RS Stat Tool");
	}
	
	return false;
}

// enable or disable all popup menu items
void PlayerView::toggle_menu_items(bool enable) {
	// get items
	Gtk::Menu::MenuList &list=m_PopupMenu->items();
			
	// toggle
	for (Gtk::Menu::MenuList::iterator it=list.begin(); it!=list.end(); ++it) {
		(*it).set_sensitive(enable);
	}
}
