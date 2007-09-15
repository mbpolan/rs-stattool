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
// mainwindow.h: the MainWindow of the application

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// gtk includes
#include <gtkmm/aboutdialog.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/window.h>
#include <gtkmm/uimanager.h>
#include "common.h"

// forward declarations
class CompareDialog;
class PlayerView;
class RSParser;

// the core window
class MainWindow: public Gtk::Window {
	public:
		// constructor
		MainWindow();
		
		// destructor
		~MainWindow();
		
		// quit signal
		sigc::signal<void, bool> signal_quit() const { return m_SigQuit; }
		
	protected:
		// signals
		sigc::signal<void, bool> m_SigQuit;
		
		/////////////////////////////
		// signal handlers
		/////////////////////////////
		
		// search button handler
		void on_search_button_clicked();
		
		// refresh player stats button
		void on_refresh_player(const Glib::ustring &name);
		
		// transfer begin handler
		void on_transfer_start();
		
		// player data arrival handler
		void on_data_ready(int code, char *data);
		
		// save player stats handler
		void on_save_player_stats(PlayerData &pd);
		
		// compare slot handler
		void on_compare_players();
		
		// about signal handler
		void on_about();
		
		// open player stats handler
		void on_open();
		
		// quit signal handler
		void on_quit();
		
		// edit preferences handler
		void on_edit_preferences();
		
		/////////////////////////////
		
		// build the ui
		void construct();
		
		// delete event handler
		virtual bool on_delete_event(GdkEventAny *e);
		
		// buttons
		Gtk::Button *m_SearchButton;
		
		// entries
		Gtk::Entry *m_PlayerEntry;
		
		// labels
		Gtk::Label *m_TitleLabel;
		Gtk::Label *m_SearchLabel;
		
		// main player view area
		PlayerView *m_NB;
		
		// menu stuff
		Glib::RefPtr<Gtk::ActionGroup> m_Actions;
		Glib::RefPtr<Gtk::UIManager> m_UI;
		
		// status bar
		Gtk::Statusbar *m_StatusBar;
		
		// about dialog
		Gtk::AboutDialog m_AboutDialog;
		
		// dialogs
		CompareDialog *m_CompareDialog;
		
		// rs parser
		RSParser *m_Parser;
		
		// internal application state
		AppState m_AppState;
};

#endif
