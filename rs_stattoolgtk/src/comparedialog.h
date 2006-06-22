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
// comparedialog.h: dialog for comparing users

#ifndef COMPAREDIALOG_H
#define COMPAREDIALOG_H

// necessary includes
#include <gtkmm/dialog.h>
#include <gtkmm/scrolledwindow.h>

// project includes
#include "common.h"

// forward declarations
class Gtk::Label;
class CompareListView;

// dialog with listview for player comparisons
class CompareDialog: public Gtk::Dialog {
	public:
		// constructor
		CompareDialog();
		
		// set players to compare
		void set_players(const PlayerData &p1, const PlayerData &p2);
		
	protected:
		// signal response handler
		virtual void on_response(int id);
		
		// labels
		Gtk::Label *m_TitleLabel;
		Gtk::Label *m_P1Label;
		Gtk::Label *m_P2Label;
		
		// scrolled window
		Gtk::ScrolledWindow *m_SW;
		
		// list view
		CompareListView *m_ListView;
};

#endif
