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
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/checkbutton.h>
#include "playerview.h"

// dialog shown before saving
class SaveDialog: public Gtk::Dialog {
	public:
		// save data
		struct SaveOps {
			Glib::ustring path;
			bool timestamp;
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
		
		// save timestamp checkbox
		Gtk::CheckButton *m_TimeStampCB;
};

#endif
