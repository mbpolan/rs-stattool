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
#include <gtkmm/actiongroup.h>
#include <gtkmm/window.h>
#include <gtkmm/uimanager.h>

// forward declarations
class Gtk::AboutDialog;
class Gtk::Button;
class Gtk::Entry;
class Gtk::Label;
class Gtk::Statusbar;
class PlayerView;
class RSParser;

// the core window
class MainWindow: public Gtk::Window {
	public:
		// constructor
		MainWindow();
		
		// destructor
		~MainWindow();
		
		/////////////////////////////
		// signal handlers
		/////////////////////////////
		
		// search button handler
		void on_search_button_clicked();
		
		// transfer begin handler
		void on_transfer_start();
		
		// data progress handler
		void on_data_progress(double t, double d);
		
		// player data arrival handler
		void on_data_ready(int code, char *data);
		
		// about signal handler
		void on_about();
		
		// quit signal handler
		void on_quit();
		
	protected:
		// build the ui
		void construct();
		
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
		Gtk::AboutDialog *m_AboutDialog;
		
		// rs parser
		RSParser *m_Parser;
};

#endif
