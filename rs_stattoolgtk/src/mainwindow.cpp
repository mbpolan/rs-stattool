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
// mainwindow.cpp: implementation of MainWindow

// gtk includes
#include <gtkmm/aboutdialog.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/stock.h>

// project includes
#include "mainwindow.h"
#include "playerview.h"
#include "rsparser.h"

// constructor
MainWindow::MainWindow(): 
		Gtk::Window(Gtk::WINDOW_TOPLEVEL) {
	// window title
	set_title("RS Stat Tool");
	
	// build the ui
	construct();
};

// destructor
MainWindow::~MainWindow() {
	// free memory
	delete m_Parser;
	delete m_AboutDialog;
};

// search button click handler
void MainWindow::on_search_button_clicked() {
	// connect online and get the html data
	m_Parser->get_player_data(m_PlayerEntry->get_text());
};

// transfer begin handler
void MainWindow::on_transfer_start() {
	// disable search button
	m_SearchButton->set_sensitive(false);
	
	// alert the user
	m_StatusBar->push("Downloading stats...");
};

// data progress handler
void MainWindow::on_data_progress(double t, double d) {
};

// player data arrival handler
void MainWindow::on_data_ready(int code, char *data) {
	// error check
	if (code!=0) {
		// tell the user there was an error
		m_StatusBar->push("Unable to get player stats");
		return;
	}
	
	// parse the html data
	bool ok;
	PlayerData player=m_Parser->parse_html(data, &ok);
	
	// see if the player exists
	if (!ok) {
		// error message
		m_StatusBar->push("** Player is not in the high scores **");
	}
	
	else {
		// and add a player tab
		m_NB->add_player_tab(player.name, player);
		
		// success message
		m_StatusBar->push("Stats transfer complete");
	}
	
	// enable search button
	m_SearchButton->set_sensitive(true);
};

// about signal handler
void MainWindow::on_about() {
	// bring up the about dialog
	m_AboutDialog->run();
};

// quit signal handler
void MainWindow::on_quit() {
	// hide the main window; ends gtk event loop
	hide();
};

// build the ui
void MainWindow::construct() {
	///////////////////////////////////////////////
	// object allocation
	
	// allocate parser
	m_Parser=new RSParser;
	
	// connect parser signals
	m_Parser->signal_data_ready.connect(
			sigc::mem_fun(*this, &MainWindow::on_data_ready));
	m_Parser->signal_transfer_start.connect(
			sigc::mem_fun(*this, &MainWindow::on_transfer_start));
	m_Parser->signal_data_progress.connect(
			sigc::mem_fun(*this, &MainWindow::on_data_progress));
	
	///////////////////////////////////////////////
	// dialog allocation
	m_AboutDialog=new Gtk::AboutDialog;
	
	// author
	std::vector<Glib::ustring> author;
	author.push_back("Mike Polan (kanadakid@gmail.com)");
	m_AboutDialog->set_authors(author);
	
	// name
	m_AboutDialog->set_name("RuneScape Stat Tool");
	
	// version
	m_AboutDialog->set_version("0.1");
	
	// comments
	m_AboutDialog->set_comments("This is a simple tool for fetching "
			 	    "player stats. To search a user, type in "
				    "his or her name in the above box, and click "
				    "the Go button.");
	
	///////////////////////////////////////////////
	// widget allocation
	
	// layout management
	// main vbox
	Gtk::VBox *mvb=manage(new Gtk::VBox);
	
	// body and status bar hbox
	Gtk::HBox *shb=manage(new Gtk::HBox);
	Gtk::HBox *phb=manage(new Gtk::HBox);
	
	// menubar hbox
	Gtk::HBox *menuhb=manage(new Gtk::HBox);
	
	/////////////////////////////////////////
	// setup boxes
	shb->set_spacing(3);
	phb->set_spacing(3);
	mvb->set_spacing(5);
	
	// create menus
	m_Actions=Gtk::ActionGroup::create();
	
	// add actions
	m_Actions->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT, "_Quit", "Quit the program"),
		       sigc::mem_fun(*this, &MainWindow::on_quit));
	m_Actions->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT, "_About", "About this program"),
		       sigc::mem_fun(*this, &MainWindow::on_about));
	
	m_Actions->add(Gtk::Action::create("FileMenu", "File"));
	m_Actions->add(Gtk::Action::create("HelpMenu", "Help"));
	
	// create ui manager
	m_UI=Gtk::UIManager::create();
	m_UI->insert_action_group(m_Actions);
	
	// build the ui from string
	Glib::ustring ui=
	"<ui>"
	"	<menubar name='MenuBar'>"
	"		<menu action='FileMenu'>"
	"			<menuitem action='FileQuit'/>"
	"		</menu>"
	"		<menu action='HelpMenu'>"
	"			<menuitem action='HelpAbout'/>"
	"		</menu>"
	"</menubar>"
	"</ui>";
		
	// catch exceptions
	try {
		// try to add the string
		m_UI->add_ui_from_string(ui);
	}
	
	// catch exceptions
	catch(const Glib::Error &ex) {
		Gtk::MessageDialog md(*this, "Unable to create UI from string.",
				    Gtk::MESSAGE_ERROR);
		md.run();
	}
	
	// grab the menubar widget
	Gtk::Widget *menuBar=m_UI->get_widget("/MenuBar");
	if (menuBar)
		menuhb->pack_start(*menuBar);
	
	// add accel keys
	add_accel_group(m_UI->get_accel_group());
	
	// allocate buttons
	m_SearchButton=manage(new Gtk::Button("Go"));
	
	// allocate entries
	m_PlayerEntry=manage(new Gtk::Entry);
	
	// allocate labels
	m_TitleLabel=manage(new Gtk::Label);
	m_TitleLabel->set_markup("<b>RuneScape Stat Tool</b>");
	
	m_SearchLabel=manage(new Gtk::Label("Search"));
	
	// allocate notebook
	m_NB=manage(new PlayerView);
	
	// build the intro page
	Gtk::Box *ib=manage(new Gtk::HBox(false, 4));
	Gtk::Label *introLabel=manage(new Gtk::Label);
	introLabel->set_markup("<span font_desc='16'>RuneScape Stat Tool</span>\n\n"
			       "You can search a player by typing his or her\n"
			       "name above and clicking 'Go.' If that player\n"
			       "is listed in the RuneScape high scores, then\n"
			       "his or her stats will be displayed.");
	
	// add label to hbox
	ib->add(*introLabel);
	ib->set_border_width(8);
	
	// add the page
	m_NB->append_page(*ib, "RS Stat Tool");
	
	// allocate progress bar
	m_StatusBar=manage(new Gtk::Statusbar);
	
	// connect signals
	m_SearchButton->signal_clicked().connect(
			sigc::mem_fun(*this, &MainWindow::on_search_button_clicked));
	
	// pack the widgets
	phb->pack_start(*m_StatusBar);
	
	shb->pack_start(*m_SearchLabel, Gtk::PACK_SHRINK);
	shb->pack_start(*m_PlayerEntry);
	shb->pack_start(*m_SearchButton, Gtk::PACK_SHRINK);
	
	mvb->pack_start(*menuhb, Gtk::PACK_SHRINK);
	mvb->pack_start(*shb, Gtk::PACK_SHRINK);
	mvb->pack_start(*m_NB);
	mvb->pack_start(*phb, Gtk::PACK_SHRINK);
	add(*mvb);
	
	// show child widgets
	show_all_children();
};
