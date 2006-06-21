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
// infodialog.h: dialog that displays info about a player

#ifndef INFODIALOG_H
#define INFODIALOG_H

// necessary includes
#include <gtkmm/dialog.h>

// project includes
#include "common.h"

// forward declarations
class Gtk::Label;

// info dialog class
class InfoDialog: public Gtk::Dialog {
	public:
		// constructor
		InfoDialog();
		
		// set player to display
		void set_player(const PlayerData &player);
		
	protected:
		// response handler
		virtual void on_response(int id);
		
		// labels
		Gtk::Label *m_NameLabel;
		Gtk::Label *m_TLvlLabel;
		Gtk::Label *m_TExpLabel;
		Gtk::Label *m_RankLabel;
};

#endif
