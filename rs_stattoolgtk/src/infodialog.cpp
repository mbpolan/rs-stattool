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
// infodialog.cpp: implementation of InfoDialog class

// necessary includes
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/separator.h>
#include <gtkmm/table.h>

// project includes
#include "infodialog.h"

// constructor
InfoDialog::InfoDialog() {
	set_resizable(false);
	
	// allocate labels
	Gtk::Label *title=manage(new Gtk::Label);
	title->set_markup("<b>Player Info</b>");
	
	// stats labels
	m_NameLabel=manage(new Gtk::Label);
	m_TLvlLabel=manage(new Gtk::Label);
	m_TExpLabel=manage(new Gtk::Label);
	m_RankLabel=manage(new Gtk::Label);
	
	// static descriptor labels
	Gtk::Label *nameLabel=manage(new Gtk::Label("Player Name:"));
	Gtk::Label *totalLevel=manage(new Gtk::Label("Total Level:"));
	Gtk::Label *totalExp=manage(new Gtk::Label("Total Exp:"));
	
	// configure vbox
	Gtk::VBox *vb=get_vbox();
	vb->set_border_width(5);
	
	// allocate table
	Gtk::Table *table=manage(new Gtk::Table);
	table->set_col_spacings(7);
	table->set_row_spacings(5);
	vb->add(*table);
	
	// pack widgets
	table->attach(*title, 0, 2, 0, 1, Gtk::FILL|Gtk::EXPAND, Gtk::FILL|Gtk::SHRINK);
	
	table->attach(*nameLabel,  0, 1, 1, 2, Gtk::FILL|Gtk::EXPAND, Gtk::FILL|Gtk::SHRINK);
	table->attach(*totalLevel, 0, 1, 2, 3, Gtk::FILL|Gtk::EXPAND, Gtk::FILL|Gtk::SHRINK);
	table->attach(*totalExp,   0, 1, 3, 4, Gtk::FILL|Gtk::EXPAND, Gtk::FILL|Gtk::SHRINK);
	
	table->attach(*m_NameLabel, 1, 2, 1, 2, Gtk::FILL|Gtk::EXPAND, Gtk::FILL|Gtk::SHRINK);
	table->attach(*m_TLvlLabel, 1, 2, 2, 3, Gtk::FILL|Gtk::EXPAND, Gtk::FILL|Gtk::SHRINK);
	table->attach(*m_TExpLabel, 1, 2, 3, 4, Gtk::FILL|Gtk::EXPAND, Gtk::FILL|Gtk::SHRINK);
	
	// set buttons
	Gtk::Button *button=add_button("OK", 0);
	
	show_all_children();
}

// set player data
void InfoDialog::set_player(const PlayerData &player) {
	// set text
	m_NameLabel->set_text(player.name);
	m_TLvlLabel->set_text(player.overallLvl);
	m_TExpLabel->set_text(player.overallExp);
	m_RankLabel->set_text(player.overallRank);
	
	show_all_children();
}

// response handler
void InfoDialog::on_response(int id) {
	Gtk::Dialog::on_response(id);
	
	// hide the dialog
	if (id==0)
		hide();
}
