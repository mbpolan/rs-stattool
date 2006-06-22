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
// comparelistview.cpp: implementation of CompareListView classes

// project includes
#include "comparelistview.h"

// constructor
CompareListView::CompareListView() {
	// set columns
	append_column("Skill", m_Columns.m_Skill);
	
	append_column("P1 Rank", m_Columns.m_P1Rank);
	append_column("P1 Level", m_Columns.m_P1Level);
	append_column("P1 Exp", m_Columns.m_P1Exp);
	
	SeparatorCellRenderer *renderer=new SeparatorCellRenderer();
	int sepId=append_column("", *renderer);
	
	append_column("P2 Rank", m_Columns.m_P2Rank);
	append_column("P2 Level", m_Columns.m_P2Level);
	append_column("P2 Exp", m_Columns.m_P2Exp);
	
	// configure separator column
	Gtk::TreeViewColumn *col=get_column(sepId);
	
	// set size
	col->set_fixed_width(3);
	col->set_resizable(false);
};

// set comparison data
void CompareListView::set_compare_data(const PlayerData &p1, const PlayerData &p2) {
};

///////////////////////////////////////////////////////////////////////////////////////
// SeparatorCellRenderer implementation

// constructor
SeparatorCellRenderer::SeparatorCellRenderer():
		Glib::ObjectBase(typeid(SeparatorCellRenderer)) {
};

// get the renderer's cell size
void SeparatorCellRenderer::get_size_vfunc(Gtk::Widget&, const Gdk::Rectangle* cell_area,
					   int* x_offset, int* y_offset, int* width, int* height) const {
	// get properties
	const unsigned int xpad=(unsigned int) g_object_get_data(G_OBJECT(gobj()), "xpad");
	const unsigned int ypad=(unsigned int) g_object_get_data(G_OBJECT(gobj()), "ypad");
	const unsigned int xalign=(unsigned int) g_object_get_data(G_OBJECT(gobj()), "xalign");
	const unsigned int yalign=(unsigned int) g_object_get_data(G_OBJECT(gobj()), "yalign");
	
	// calculate width and height
	int w=xpad*2;
	int h=ypad*2;
	
	if (width)
		*width=w;
	if (height)
		*height=h;
	
	// calculate offsets
	if (cell_area) {
		// offset on x axis
		if (x_offset) {
			*x_offset=int(xalign-(cell_area->get_width()-w));
			*x_offset=std::max(0, *x_offset);
		}
		
		// offset on y axis
		if (y_offset) {
			*y_offset=int(yalign-(cell_area->get_height()-h));
			*y_offset=std::max(0, *y_offset);
		}
	}
};

// render the cell
void SeparatorCellRenderer::render_vfunc(const Glib::RefPtr<Gdk::Drawable>& window, Gtk::Widget& widget,
		  			 const Gdk::Rectangle& background_area,
					 const Gdk::Rectangle& cell_area,
		  			 const Gdk::Rectangle& expose_area,
					 Gtk::CellRendererState flags) {
	// get cell padding
	const unsigned int xpad=(unsigned int) g_object_get_data(G_OBJECT(gobj()), "xpad");
	const unsigned int ypad=(unsigned int) g_object_get_data(G_OBJECT(gobj()), "ypad");
	
	// calculate box dimensions
	int x_offset=0, y_offset=0, w=0, h=0;
	get_size(widget, cell_area, x_offset, y_offset, w, h);
	
	// cast the window
	Glib::RefPtr<Gdk::Window> win=Glib::RefPtr<Gdk::Window>::cast_dynamic<>(window);
	
	// draw the inactive background
	widget.get_style()->paint_box(win, Gtk::STATE_INSENSITIVE, Gtk::SHADOW_NONE,
				      cell_area, widget, "0", 
				      x_offset, y_offset,
				      w, h);
};
