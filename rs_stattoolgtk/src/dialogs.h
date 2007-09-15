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
// dialogs.h: various dialog classes

#ifndef DIALOGS_H
#define DIALOGS_H

#include <gtkmm/button.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/frame.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/notebook.h>
#include <gtkmm/checkbutton.h>

#include "common.h"
#include "playerview.h"

// dialog for choosing players to compare
class CompareSelectDialog: public Gtk::Dialog {
	public:
		// data for comparison
		struct CompareData {
			// player names
			Glib::ustring player1;
			Glib::ustring player2;
			
			// file paths
			Glib::ustring player1File;
			Glib::ustring player2File;
			
			// comparison types
			bool useFile1;
			bool useFile2;
		};
		
		// constructor
		CompareSelectDialog();
		
		// set players to use for combo boxes
		void set_players(std::vector<Glib::ustring> players);
		
		// get a struct containing compare data
		struct CompareData get_compare_struct();
		
	protected:
		// build the dialog
		void construct();
		
		// checkbox toggle handlers
		void on_file_cb1_toggled();
		void on_file_cb2_toggled();
		
		// show fs buttons click handlers
		void on_show_fs1_clicked();
		void on_show_fs2_clicked();
		
		// labels
		Gtk::Label *m_TitleLabel;
		Gtk::Label *m_Player1Label;
		Gtk::Label *m_Player2Label;
		Gtk::Label *m_Player1FileLabel;
		Gtk::Label *m_Player2FileLabel;
		
		// check buttons
		Gtk::CheckButton *m_File1CB;
		Gtk::CheckButton *m_File2CB;
		
		// combo boxes
		Gtk::ComboBoxText *m_Player1CB;
		Gtk::ComboBoxText *m_Player2CB;
		
		// frames
		Gtk::Frame *m_Player1Frame;
		Gtk::Frame *m_Player2Frame;
		
		// entries
		Gtk::Entry *m_File1Entry;
		Gtk::Entry *m_File2Entry;
		
		// buttons
		Gtk::Button *m_OKButton;
		Gtk::Button *m_CancelButton;
		Gtk::Button *m_ShowFSButton1;
		Gtk::Button *m_ShowFSButton2;
};

/***************************************************************************/

// dialog shown before saving
class SaveDialog: public Gtk::Dialog {
	public:
		// save data
		struct SaveOps {
			Glib::ustring path;
		};
		
		// constructor
		SaveDialog();
		
		// get save struct
		struct SaveOps get_save_ops();
		
	protected:
		// build the dialog
		void construct();
		
		// file dialog button handler
		void on_file_button_clicked();
		
		// labels
		Gtk::Label *m_TitleLabel;
		Gtk::Label *m_FilePathLabel;
		
		// buttons
		Gtk::Button *m_ShowFSButton;
		
		// entry for path name
		Gtk::Entry *m_FilePathEntry;
};

/***************************************************************************/

// dialog used to changed preferences
class PreferencesDialog: public Gtk::Dialog {
	public:
		// constructor
		PreferencesDialog();
		
		// set preferences based on AppState
		void set_preferences(const AppState &state);
		
		// get the preferences state in the form of an AppState struct
		AppState get_preferences();
	
	protected:
		// build the ui
		void construct();
		
		// notebook
		Gtk::Notebook *m_Notebook;
		
		// check buttons
		Gtk::CheckButton *m_CloseXCB;
};

#endif
