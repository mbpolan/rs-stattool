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
// clistview.h: custom list view for players

#ifndef CLISTVIEW_H
#define CLISTVIEW_H

// gtk includes
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>

// project includes
#include "common.h"

// player list view
class CListView: public Gtk::TreeView {
	public:
		// constructor
		CListView();
		
		// clear out all data
		void clear();
		
		// populate the list
		void populate(const PlayerData &data);
		
		// return list of rows
		Gtk::TreeModel::Children get_rows() const { return m_Model->children(); };
		
		// get the overall data
		void get_overall_data(Glib::ustring &level, Glib::ustring &exp, Glib::ustring &rank);

	protected:
		// build the widget
		void construct();
		
		// tree model
		class ColumnRec: public Gtk::TreeModelColumnRecord {
			public: 
				// constructor
				ColumnRec() {
					// add the columns
					add(m_Skill);
					add(m_Rank);
					add(m_Level);
					add(m_Exp);
				};
				
				// columns
				Gtk::TreeModelColumn<Glib::ustring> m_Skill;
				Gtk::TreeModelColumn<Glib::ustring> m_Rank;
				Gtk::TreeModelColumn<Glib::ustring> m_Level;
				Gtk::TreeModelColumn<Glib::ustring> m_Exp;
		};
		
		
		// model instance
		Glib::RefPtr<Gtk::ListStore> m_Model;
		
		// store overall data
		Glib::ustring m_PlayerTotalLevel;
		Glib::ustring m_PlayerTotalExp;
		Glib::ustring m_PlayerTotalRank;
		
	public:
		// column record
		ColumnRec m_Columns;
};

#endif
