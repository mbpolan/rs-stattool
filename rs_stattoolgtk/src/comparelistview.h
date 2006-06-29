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
// comparelistview.h: the CompareListView class

#ifndef COMPARELISTVIEW_H
#define COMPARELISTVIEW_H

// necessary includes
#include <gtkmm/cellrenderer.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>

// project includes
#include "common.h"

// list view for player comparisons
class CompareListView: public Gtk::TreeView {
	public:
		// constructor
		CompareListView();
		
		// set comparison data
		void set_compare_data(const PlayerData &p1, const PlayerData &p2);
		
		// clear the list
		void clear() { m_Model->clear(); };
		
	protected:
		// color the rows based on each players' rank
		void color_rows();
		
		// column record
		class ColumnRec: public Gtk::TreeModelColumnRecord {
			public:
				// constructor
				ColumnRec() {
					add(m_Skill);
					
					add(m_P1Rank);
					add(m_P1Level);
					add(m_P1Exp);
					
					add(m_VSep);
					
					add(m_P2Rank);
					add(m_P2Level);
					add(m_P2Exp);
					
					add(m_Color);
				};
				
				// columns
				Gtk::TreeModelColumn<Glib::ustring> m_Skill;
				
				Gtk::TreeModelColumn<Glib::ustring> m_P1Rank;
				Gtk::TreeModelColumn<Glib::ustring> m_P1Level;
				Gtk::TreeModelColumn<Glib::ustring> m_P1Exp;
				
				Gtk::TreeModelColumn<Glib::ustring> m_VSep;
				
				Gtk::TreeModelColumn<Glib::ustring> m_P2Rank;
				Gtk::TreeModelColumn<Glib::ustring> m_P2Level;
				Gtk::TreeModelColumn<Glib::ustring> m_P2Exp;
				
				Gtk::TreeModelColumn<Glib::ustring> m_Color;
		};
		
		// column record instance
		ColumnRec m_Columns;
		
		// list store model
		Glib::RefPtr<Gtk::ListStore> m_Model;
};

// cellrenderer for separator column
class SeparatorCellRenderer: public Gtk::CellRenderer {
	public:
		// constructor
		SeparatorCellRenderer();
		
		// destructor
		virtual ~SeparatorCellRenderer() { };
		
	protected:
		// get renderer's cell size
		virtual void get_size_vfunc(Gtk::Widget&, const Gdk::Rectangle* cell_area,
					    int* x_offset, int* y_offset, 
					    int* width, int* height) const;
		
		// render the cell
		virtual void render_vfunc(const Glib::RefPtr<Gdk::Drawable>& window,
					  Gtk::Widget& widget,
					  const Gdk::Rectangle& background_area,
					  const Gdk::Rectangle& cell_area,
					  const Gdk::Rectangle& expose_area,
					  Gtk::CellRendererState flags);
};

#endif
