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
#include "dialogs.h"

// constructor
SaveDialog::SaveDialog() {
	set_resizable(false);
	construct();
};

// get save struct
struct SaveDialog::SaveOps SaveDialog::get_save_ops() {
	struct SaveOps sp;
	sp.path=m_FilePathEntry->get_text();
	sp.timestamp=m_TimeStampCB->get_active();
	
	return sp;
};

// build the dialog
void SaveDialog::construct() {
	// get the main vb and configure it
	Gtk::VBox *vb=get_vbox();
	vb->set_spacing(5);
	vb->set_border_width(5);
	
	// frame vbox
	Gtk::VBox *fVBox=manage(new Gtk::VBox);
	fVBox->set_border_width(1);
	
	// allocate container frame
	Gtk::Frame *frame=manage(new Gtk::Frame("Options"));
	
	// allocate labels
	m_TitleLabel=manage(new Gtk::Label("Save Stats"));
	m_FilePathLabel=manage(new Gtk::Label("Path"));
	
	// allocate entries
	m_FilePathEntry=manage(new Gtk::Entry);
	
	// allocate buttons
	m_ShowFSButton=manage(new Gtk::Button("..."));
	m_ShowFSButton->signal_clicked().connect(sigc::mem_fun(*this, &SaveDialog::on_file_button_clicked));
	
	// allocate check boxes
	m_TimeStampCB=manage(new Gtk::CheckButton("Save Timestamp"));
	m_TimeStampCB->set_active(false);
	
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
	
	fVBox->pack_start(*m_TimeStampCB);
	frame->add(*fVBox);
	
	vb->pack_start(*frame);
	
	show_all_children();
};

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
};
