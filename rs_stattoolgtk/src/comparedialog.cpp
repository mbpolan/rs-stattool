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
// comparedialog.cpp: implementation of CompareDialog class

// necessary includes
#include <gtkmm/label.h>
#include <gtkmm/table.h>
#include <sstream>

// project includes
#include "common.h"
#include "comparedialog.h"
#include "comparelistview.h"

// CompareDialog implementation
// constructor
CompareDialog::CompareDialog(): Gtk::Dialog() {
	// set default size
	set_size_request(COMPARE_DIALOG_WIDTH, COMPARE_DIALOG_HEIGHT);
	
	// get the vbox
	Gtk::VBox *vb=get_vbox();
	
	// allocate table
	Gtk::Table *table=manage(new Gtk::Table);
	
	// allocate labels
	m_TitleLabel=manage(new Gtk::Label);
	m_TitleLabel->set_markup("<b>Player Comparison</b>");
	
	m_P1Label=manage(new Gtk::Label);
	m_P2Label=manage(new Gtk::Label);
	m_P1DateLabel=manage(new Gtk::Label);
	m_P2DateLabel=manage(new Gtk::Label);
	
	// allocate scrolled window and list view
	m_SW=manage(new Gtk::ScrolledWindow);
	m_ListView=manage(new CompareListView);
	m_SW->add(*m_ListView);
	
	// pack widgets
	vb->pack_start(*table);
	
	table->attach(*m_TitleLabel, 0, 2, 0, 1, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
	table->attach(*m_P1Label, 0, 1, 1, 2, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
	table->attach(*m_P2Label, 1, 2, 1, 2, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
	table->attach(*m_P1DateLabel, 0, 1, 2, 3, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
	table->attach(*m_P2DateLabel, 1, 2, 2, 3, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
	table->attach(*m_SW, 0, 2, 3, 4);
	
	// add buttons
	add_button("OK", 1);
	
	// show children
	show_all_children();
}

// set players to compare
void CompareDialog::set_players(const PlayerData &p1, const PlayerData &p2) {
	// create formatted strings
	std::stringstream ss;
	for (int i=0; i<2; i++) {
		Glib::ustring col, name;
		Gtk::Label *label, *tslabel;
		time_t ts;
		if (i==0) {
			col=COMPARE_COLOR_HI;
			name=p1.name;
			label=m_P1Label;
			
			ts=p1.timestamp;
			tslabel=m_P1DateLabel;
			
		}
		else {
			col=COMPARE_COLOR_LOW;
			name=p2.name;
			label=m_P2Label;
			
			ts=p2.timestamp;
			tslabel=m_P2DateLabel;
		}
		
		// format the string
		ss << "<span background=\"" << col << 
				"\"><b>" << name << "</b></span>";
		
		// set the string
		label->set_markup(ss.str());
		
		// set the date by timestamp
		tslabel->set_text((ts==0 ? "-" : ctime(&ts)));
		
		// clear
		ss.str("");
	}
	
	// populate the list view
	m_ListView->set_compare_data(p1, p2);
}

// signal response handler
void CompareDialog::on_response(int id) {
	if (id==1) {
		// clear the old tree view data
		m_ListView->clear();
		
		// close this dialog
		hide();
	}
}
