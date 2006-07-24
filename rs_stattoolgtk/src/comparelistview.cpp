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

// necessary includes
#include <vector>

// project includes
#include "comparelistview.h"
#include "utilities.h"

// constructor
CompareListView::CompareListView() {
	// allocate model
	m_Model=Gtk::ListStore::create(m_Columns);
	set_model(m_Model);
	
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
	// grab skills and add them in
	for (int i=0; i<SKILL_COUNT; i++) {
		Gtk::TreeModel::Row row=*(m_Model->append());
		
		// skill name
		row[m_Columns.m_Skill]=p1.skills[i].name;
		
		// player 1 data
		row[m_Columns.m_P1Rank]=p1.skills[i].rank;
		row[m_Columns.m_P1Level]=p1.skills[i].level;
		row[m_Columns.m_P1Exp]=p1.skills[i].xp;
		
		// player 2 data
		row[m_Columns.m_P2Rank]=p2.skills[i].rank;
		row[m_Columns.m_P2Level]=p2.skills[i].level;
		row[m_Columns.m_P2Exp]=p2.skills[i].xp;
	}
	
	// color the rows based on player rank
	color_rows();
};

void CompareListView::color_rows() {
	// get list of columns
	std::vector<Gtk::TreeViewColumn*> columns=get_columns();
	Gtk::TreeModel::Children children=get_model()->children();
	
	// iterate over rows and columns
	for (int i=0; i<children.size(); i++) {
		Gtk::TreeModel::Row row=*(children[i]);
		
		// get attributes, converted to ints
		int p1Rank=Utils::ustring_to_int(row[m_Columns.m_P1Rank]);
		int p2Rank=Utils::ustring_to_int(row[m_Columns.m_P2Rank]);
		
		// first, check if one of the players' skills is not ranked
		if (row[m_Columns.m_P1Rank]=="-" || 
		    row[m_Columns.m_P2Rank]=="-")
			row[m_Columns.m_Color]=COMPARE_COLOR_GRAY;
		
		// compare player 1 to player 2
		// the lower the rank, the better the player
		else if (p1Rank<p2Rank)
			row[m_Columns.m_Color]=COMPARE_COLOR_HI;
		else if (p1Rank==p2Rank)
			row[m_Columns.m_Color]=COMPARE_COLOR_EVEN;
		else
			row[m_Columns.m_Color]=COMPARE_COLOR_LOW;
		
		// iterate over columns
		for (int y=0; y<columns.size(); y++) {
			Gtk::TreeView::Column *col=get_column(y);
			
			// get the renderer and set its color property
			Gtk::CellRenderer *renderer=col->get_first_cell_renderer();
			col->add_attribute(renderer->property_cell_background(),
					   m_Columns.m_Color);
		}
	}
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
	
	// allocate color by system colormap
	Gdk::Color shaded("DarkSlateGray");
	Glib::RefPtr<Gdk::GC> gc=Gdk::GC::create(win);
	Glib::RefPtr<Gdk::Colormap> cm=Gdk::Colormap::get_system();
	cm->alloc_color(shaded);
	
	// draw the cell
	win->draw_rectangle(gc, true, x_offset, y_offset, w, h);
	
	// draw the inactive background
	/*widget.get_style()->paint_box(win, Gtk::STATE_INSENSITIVE, Gtk::SHADOW_NONE,
				      cell_area, widget, "0", 
				      x_offset, y_offset,
				      w, h);
	*/
};
