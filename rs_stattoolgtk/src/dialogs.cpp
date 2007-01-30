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
// dialogs.cpp: implementation of dialogs classes

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/frame.h>
#include <gtkmm/table.h>
#include "dialogs.h"

// constructor
CompareSelectDialog::CompareSelectDialog() {
	set_resizable(false);
	construct();
}

// set players to use for combo boxes
void CompareSelectDialog::set_players(std::vector<Glib::ustring> players) {
	// set the players in the combo boxes
	for (std::vector<Glib::ustring>::iterator it=players.begin(); it!=players.end(); ++it) {
		m_Player1CB->append_text((*it));
		m_Player2CB->append_text((*it));
	}
	
	// enable player-player comparison
	if (players.size()>=2) {
		m_Player1CB->set_active(0);
		m_Player2CB->set_active(1);
		
		// disable file comparison by default
		m_File1CB->set_sensitive(true);
		m_File2CB->set_sensitive(true);
		m_File1CB->set_active(false);
		m_File2CB->set_active(false);
		
		// toggle widgets
		on_file_cb1_toggled();
		on_file_cb2_toggled();
	}
	
	// enable player-file comparison
	else if (players.size()==1) {
		m_Player1CB->set_active(0);
		
		// disable player 1 file comparison
		m_File1CB->set_sensitive(false);
		m_File1CB->set_active(false);
		
		// force file comparison on player 2
		m_File2CB->set_sensitive(false);
		m_File2CB->set_active(true);
		
		// toggle widgets
		on_file_cb1_toggled();
		on_file_cb2_toggled();
	}
	
	// otherwise just enable file-file comparison
	else {
		// enable only file compare for player 1
		m_File1CB->set_sensitive(false);
		m_File1CB->set_active(true);
		
		// enable only file compare for player 2
		m_File2CB->set_sensitive(false);
		m_File2CB->set_active(true);
		
		// toggle widgets
		on_file_cb1_toggled();
		on_file_cb2_toggled();
	}
}

// get a struct containing compare data
struct CompareSelectDialog::CompareData CompareSelectDialog::get_compare_struct() {
	struct CompareData cd;
	
	// set the names
	cd.player1=m_Player1CB->get_active_text();
	cd.player2=m_Player2CB->get_active_text();
	
	// set the file paths
	cd.player1File=m_File1Entry->get_text();
	cd.player2File=m_File2Entry->get_text();
	
	// set compare options
	int size=m_Player1CB->get_model()->children().size();
	if (size>=2) {
		cd.useFile1=(m_File1CB->get_active() && m_File1CB->is_sensitive());
		cd.useFile2=(m_File2CB->get_active() && m_File2CB->is_sensitive());
	}
	
	else if (size==1) {
		cd.useFile1=false;
		cd.useFile2=true;
	}
	
	else {
		cd.useFile1=true;
		cd.useFile2=true;
	}
	
	return cd;
}

// build the dialog
void CompareSelectDialog::construct() {
	// get vbox
	Gtk::VBox *vb=get_vbox();
	vb->set_border_width(5);
	vb->set_spacing(5);
	
	// allocate tables
	Gtk::Table *mtable=manage(new Gtk::Table);
	mtable->set_spacings(5);
	
	// for player frames
	Gtk::Table *ftable1=manage(new Gtk::Table);
	ftable1->set_border_width(5);
	ftable1->set_spacings(5);
	
	Gtk::Table *ftable2=manage(new Gtk::Table);
	ftable2->set_border_width(5);
	ftable2->set_spacings(5);
	
	// allocate frames
	m_Player1Frame=manage(new Gtk::Frame);
	m_Player2Frame=manage(new Gtk::Frame);
	
	// allocate labels
	m_TitleLabel=manage(new Gtk::Label("Compare Players"));
	m_Player1Label=manage(new Gtk::Label("Player 1"));
	m_Player2Label=manage(new Gtk::Label("Player 2"));
	m_Player1FileLabel=manage(new Gtk::Label("Path to Save File"));
	m_Player2FileLabel=manage(new Gtk::Label("Path to Save File"));
	
	// allocate buttons
	m_File1CB=manage(new Gtk::CheckButton("Compare with file"));
	m_File2CB=manage(new Gtk::CheckButton("Compare with file"));
	m_ShowFSButton1=manage(new Gtk::Button("..."));
	m_ShowFSButton2=manage(new Gtk::Button("..."));
	
	// allocate entries
	m_File1Entry=manage(new Gtk::Entry);
	m_File2Entry=manage(new Gtk::Entry);
	
	// allocate combo boxes
	m_Player1CB=manage(new Gtk::ComboBoxText);
	m_Player2CB=manage(new Gtk::ComboBoxText);
	
	// connect signals
	m_File1CB->signal_toggled().connect(sigc::mem_fun(*this, &CompareSelectDialog::on_file_cb1_toggled));
	m_File2CB->signal_toggled().connect(sigc::mem_fun(*this, &CompareSelectDialog::on_file_cb2_toggled));
	m_ShowFSButton1->signal_clicked().connect(sigc::mem_fun(*this, &CompareSelectDialog::on_show_fs1_clicked));
	m_ShowFSButton2->signal_clicked().connect(sigc::mem_fun(*this, &CompareSelectDialog::on_show_fs2_clicked));
	
	// just in case there are no players, force file comparison
	
	// disable non-file widgets by default
	m_Player1Label->set_sensitive(false);
	m_Player2Label->set_sensitive(false);
	m_Player1CB->set_sensitive(false);
	m_Player2CB->set_sensitive(false);
	
	// enable files by default
	m_File1CB->set_active(true);
	m_File2CB->set_active(true);
	m_File1CB->set_sensitive(false);
	m_File2CB->set_sensitive(false);
	
	// add buttons
	add_button("OK", 1);
	add_button("Cancel", 0);
	
	// place widgets
	ftable1->attach(*m_Player1Label, 0, 1, 0, 1);
	ftable1->attach(*m_Player1CB, 1, 2, 0, 1);
	ftable1->attach(*m_Player1FileLabel, 0, 1, 1, 2);
	ftable1->attach(*m_File1Entry, 1, 2, 1, 2);
	ftable1->attach(*m_ShowFSButton1, 2, 3, 1, 2);
	ftable1->attach(*m_File1CB, 0, 3, 2, 3, Gtk::SHRINK | Gtk::SHRINK);
	
	ftable2->attach(*m_Player2Label, 0, 1, 0, 1);
	ftable2->attach(*m_Player2CB, 1, 2, 0, 1);
	ftable2->attach(*m_Player2FileLabel, 0, 1, 1, 2);
	ftable2->attach(*m_File2Entry, 1, 2, 1, 2);
	ftable2->attach(*m_ShowFSButton2, 2, 3, 1, 2);
	ftable2->attach(*m_File2CB, 0, 3, 2, 3, Gtk::SHRINK | Gtk::SHRINK);
	
	m_Player1Frame->add(*ftable1);
	m_Player2Frame->add(*ftable2);
	
	mtable->attach(*m_TitleLabel, 0, 1, 0, 1);
	mtable->attach(*m_Player1Frame, 0, 1, 1, 2);
	mtable->attach(*m_Player2Frame, 0, 1, 2, 3);
	
	vb->pack_start(*mtable);
	
	show_all_children();
}

// checkbox toggle 1 handler
void CompareSelectDialog::on_file_cb1_toggled() {
	// modify the sensitivity of player 1 file widgets
	bool t=m_File1CB->get_active();
	
	m_Player1Label->set_sensitive(!t);
	m_Player1CB->set_sensitive(!t);
	
	m_File1Entry->set_sensitive(t);
	m_Player1FileLabel->set_sensitive(t);
	m_ShowFSButton1->set_sensitive(t);
}

// checkbox toggle 2 handler
void CompareSelectDialog::on_file_cb2_toggled() {
	// modify the sensitivity of player 2 file widgets
	bool t=m_File2CB->get_active();
	
	m_Player2Label->set_sensitive(!t);
	m_Player2CB->set_sensitive(!t);
	
	m_File2Entry->set_sensitive(t);
	m_Player2FileLabel->set_sensitive(t);
	m_ShowFSButton2->set_sensitive(t);
}

// show fs buttons click handler
void CompareSelectDialog::on_show_fs1_clicked() {
	// run file chooser
	Gtk::FileChooserDialog fcd(*this, "Open Player File", Gtk::FILE_CHOOSER_ACTION_OPEN);
	fcd.add_button("Open", 1);
	fcd.add_button("Cancel", 0);
	
	// add a filter
	Gtk::FileFilter filter;
	filter.set_name("Player Stat Files (*.rsp)");
	filter.add_pattern("*.rsp");
	fcd.set_filter(filter);
	
	// set the file text if ok
	if (fcd.run()==1)
		m_File1Entry->set_text(fcd.get_filename());
	fcd.hide();
}

// show fs button 2 click handler
void CompareSelectDialog::on_show_fs2_clicked() {
	// run file chooser
	Gtk::FileChooserDialog fcd(*this, "Open Player File", Gtk::FILE_CHOOSER_ACTION_OPEN);
	fcd.add_button("Open", 1);
	fcd.add_button("Cancel", 0);
	
	// add a filter
	Gtk::FileFilter filter;
	filter.set_name("Player Stat Files (*.rsp)");
	filter.add_pattern("*.rsp");
	fcd.set_filter(filter);
	
	// set the file text if ok
	if (fcd.run()==1)
		m_File2Entry->set_text(fcd.get_filename());
	fcd.hide();
}

/******************************************************************************************/

// constructor
SaveDialog::SaveDialog() {
	set_resizable(false);
	construct();
}

// get save struct
struct SaveDialog::SaveOps SaveDialog::get_save_ops() {
	struct SaveOps sp;
	sp.path=m_FilePathEntry->get_text();
	
	return sp;
}

// build the dialog
void SaveDialog::construct() {
	// get the main vb and configure it
	Gtk::VBox *vb=get_vbox();
	vb->set_spacing(5);
	vb->set_border_width(5);
	
	// allocate labels
	m_TitleLabel=manage(new Gtk::Label("Save Stats"));
	m_FilePathLabel=manage(new Gtk::Label("Path"));
	
	// allocate entries
	m_FilePathEntry=manage(new Gtk::Entry);
	
	// allocate buttons
	m_ShowFSButton=manage(new Gtk::Button("..."));
	m_ShowFSButton->signal_clicked().connect(sigc::mem_fun(*this, &SaveDialog::on_file_button_clicked));
	
	// allocate hboxes
	Gtk::HBox *fileHB=manage(new Gtk::HBox);
	fileHB->set_spacing(5);
	
	// add buttons
	add_button("OK", 1);
	add_button("Cancel", 0);
	
	// pack widgets
	fileHB->pack_start(*m_FilePathLabel);
	fileHB->pack_start(*m_FilePathEntry);
	fileHB->pack_start(*m_ShowFSButton, Gtk::PACK_SHRINK);
	
	vb->pack_start(*m_TitleLabel);
	vb->pack_start(*fileHB);
	
	show_all_children();
}

// file dialog button handler
void SaveDialog::on_file_button_clicked() {
	// run file chooser dialog
	Gtk::FileChooserDialog fcd(*this, "Save Stats", Gtk::FILE_CHOOSER_ACTION_SAVE);
	fcd.add_button("Save", 1);
	fcd.add_button("Cancel", 0);
	if (fcd.run()) {
		// update the entry
		m_FilePathEntry->set_text(fcd.get_filename());
	}
}
