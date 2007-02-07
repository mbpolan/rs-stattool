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
#include <gtkmm/comboboxtext.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/stock.h>

// project includes
#include "comparedialog.h"
#include "dialogs.h"
#include "io.h"
#include "mainwindow.h"
#include "playerview.h"
#include "rsparser.h"
#include "utilities.h"

// constructor
MainWindow::MainWindow(): 
		Gtk::Window(Gtk::WINDOW_TOPLEVEL) {
	// window title
	set_title("RS Stat Tool");
	
	// set the minimum width
	set_size_request(400, 320);
	set_resizable(false);
	
	// build the ui
	construct();
}

// destructor
MainWindow::~MainWindow() {
	// free memory
	delete m_Parser;
	delete m_AboutDialog;
	delete m_CompareDialog;
}

// search button click handler
void MainWindow::on_search_button_clicked() {
	// connect online and get the html data
	m_Parser->get_player_data(m_PlayerEntry->get_text());
}

// refresh player stats button
void MainWindow::on_refresh_player(const Glib::ustring &name) {
	// connect and get the updated data
	m_Parser->get_player_data(m_PlayerEntry->get_text());
}

// transfer begin handler
void MainWindow::on_transfer_start() {
	// disable search button
	m_SearchButton->set_sensitive(false);
	
	// alert the user
	m_StatusBar->push("Downloading stats...");
}

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
		m_NB->set_current_page(-1);
		
		// success message
		m_StatusBar->push("Stats transfer complete");
	}
	
	// enable search button
	m_SearchButton->set_sensitive(true);
}

// save player stats handler
void MainWindow::on_save_player_stats(PlayerData &pd) {
	// run save options dialog
	SaveDialog sdiag;
	if (sdiag.run()) {
		// get save struct
		struct SaveDialog::SaveOps sp=sdiag.get_save_ops();
		
		// check for extension
		Glib::ustring ext=sp.path.substr(sp.path.size()-4, sp.path.size()-1);
		ext.lowercase();
		if (ext!=".rsp")
			sp.path+=".rsp";
		
		// ask the io handler to save this player's stats
		if (!IOHandler::save_player_stats(sp.path, pd)) {
			Gtk::MessageDialog md(*this, Utils::translate_io_error(IOHandler::Error));
			md.run();
		}
		
	}
	sdiag.hide();
}

// compare slot handler
void MainWindow::on_compare_players() {
	// initialize compare selection dialog
	CompareSelectDialog cd;
	CompareDialog cdialog;
	
	// build list of players
	std::vector<Glib::ustring> players;
	PlayerView::PlayerMap map=m_NB->get_map();
	for (PlayerView::PlayerMap::iterator it=map.begin(); it!=map.end(); ++it)
		players.push_back((*it).first);
	
	// use it to fill in combo boxes
	cd.set_players(players);
	
	// run the dialog
	if (cd.run()==1) {
		// get a compare struct
		struct CompareSelectDialog::CompareData data=cd.get_compare_struct();
		
		// player data structs
		PlayerData p1, p2;
		
		// compare using only 2 files
		if (data.useFile1 && data.useFile2) {
			// load both files
			if (!IOHandler::load_player_stats(data.player1File, p1) || 
			    (!IOHandler::load_player_stats(data.player2File, p2))) {
				Gtk::MessageDialog md(*this, Utils::translate_io_error(IOHandler::Error));
				md.run();
				return;
			}
			
			// set the compare data
			cdialog.set_players(p1, p2);
		}
		
		// compare player 1 with file
		else if (!data.useFile1 && data.useFile2) {
			// get data for player 1
			p1=m_NB->get_player_data(data.player1);
			
			// load the other file
			if (!IOHandler::load_player_stats(data.player2File, p2)) {
				Gtk::MessageDialog md(*this, Utils::translate_io_error(IOHandler::Error));
				md.run();
				return;
			}
			
			// set the compare data
			cdialog.set_players(p1, p2);
		}
		
		// compare file with player 2
		else if (data.useFile1 && !data.useFile2) {
			// get data for player 2
			p2=m_NB->get_player_data(data.player2);
			
			// load the other file
			if (!IOHandler::load_player_stats(data.player1File, p1)) {
				Gtk::MessageDialog md(*this, Utils::translate_io_error(IOHandler::Error));
				md.run();
				return;
			}
			
			// set the compare data
			cdialog.set_players(p1, p2);
		}
		
		// compare two players
		else {
			p1=m_NB->get_player_data(data.player1);
			p2=m_NB->get_player_data(data.player2);
			
			cdialog.set_players(p1, p2);
		}
		
		// bring up the dialog
		cdialog.run();
	}
}

// about signal handler
void MainWindow::on_about() {
	// bring up the about dialog
	m_AboutDialog->run();
}

// open player stats handler
void MainWindow::on_open() {
	// run file chooser dialog
	Gtk::FileChooserDialog fcd(*this, "Open Saved Player Stats", Gtk::FILE_CHOOSER_ACTION_OPEN);
	fcd.add_button("Open", 1);
	fcd.add_button("Cancel", 0);
	
	// add a filter
	Gtk::FileFilter filter;
	filter.set_name("Player Stat Files (*.rsp)");
	filter.add_pattern("*.rsp");
	fcd.set_filter(filter);
	
	if (fcd.run()) {
		// get the path
		Glib::ustring path=fcd.get_filename();
		
		// and try to open it
		PlayerData pd;
		if (!IOHandler::load_player_stats(path, pd)) {
			Gtk::MessageDialog md(*this, Utils::translate_io_error(IOHandler::Error));
			md.run();
		}
		
		// add a new tab for this player
		m_NB->add_player_tab(pd.name, pd);
	}
}

// quit signal handler
void MainWindow::on_quit() {
	// hide the main window; ends gtk event loop
	hide();
}

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
	m_AboutDialog->set_version("0.5");
	
	// comments
	m_AboutDialog->set_comments("This is a simple tool for fetching "
			 	    "player stats. To search a user, type in "
				    "his or her name in the above box, and click "
				    "the Go button.");
	
	// compare dialog
	m_CompareDialog=new CompareDialog;
	
	///////////////////////////////////////////////
	// widget allocation
	
	// layout management
	// main vbox
	Gtk::VBox *mvb=manage(new Gtk::VBox);
	Gtk::VBox *msvb=manage(new Gtk::VBox);
	
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
	msvb->set_border_width(10);
	msvb->set_spacing(10);
	
	// create menus
	m_Actions=Gtk::ActionGroup::create();
	
	// add actions
	m_Actions->add(Gtk::Action::create("FileOpen", Gtk::Stock::OPEN, "_Open", "Open saved player stats"),
		       sigc::mem_fun(*this, &MainWindow::on_open));
	m_Actions->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT, "_Quit", "Quit the program"),
		       sigc::mem_fun(*this, &MainWindow::on_quit));
	
	m_Actions->add(Gtk::Action::create("ToolsCompare", "_Compare", "Compare two players"),
		       sigc::mem_fun(*this, &MainWindow::on_compare_players));
	
	m_Actions->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT, "_About", "About this program"),
		       sigc::mem_fun(*this, &MainWindow::on_about));
	
	m_Actions->add(Gtk::Action::create("FileMenu", "File"));
	m_Actions->add(Gtk::Action::create("ToolsMenu", "Tools"));
	m_Actions->add(Gtk::Action::create("HelpMenu", "Help"));
	
	// create ui manager
	m_UI=Gtk::UIManager::create();
	m_UI->insert_action_group(m_Actions);
	
	// build the ui from string
	Glib::ustring ui=
	"<ui>"
	"	<menubar name='MenuBar'>"
	"		<menu action='FileMenu'>"
	"			<menuitem action='FileOpen'/>"
	"			<separator/>"
	"			<menuitem action='FileQuit'/>"
	"		</menu>"
	"		<menu action='ToolsMenu'>"
	"			<menuitem action='ToolsCompare'/>"
	"		</menu>"
	"		<menu action='HelpMenu'>"
	"			<menuitem action='HelpAbout'/>"
	"		</menu>"
	"</menubar>"
	"</ui>";
	
	// try to add the string
	m_UI->add_ui_from_string(ui);
	
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
	m_PlayerEntry->set_max_length(RS_MAX_PLAYER_NAME_LENGTH);
	
	// allocate labels
	m_TitleLabel=manage(new Gtk::Label);
	m_TitleLabel->set_markup("<b>RuneScape Stat Tool</b>");
	
	m_SearchLabel=manage(new Gtk::Label("Search"));
	
	// allocate notebook
	m_NB=manage(new PlayerView);
	
	// connect signals
	m_NB->signal_save_stats_request.connect(sigc::mem_fun(*this, &MainWindow::on_save_player_stats));
	m_NB->signal_refresh_player.connect(sigc::mem_fun(*this, &MainWindow::on_refresh_player));
	
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
	msvb->pack_start(*shb, Gtk::PACK_SHRINK);
	msvb->pack_start(*m_NB);
	
	mvb->pack_start(*msvb);
	mvb->pack_start(*phb, Gtk::PACK_SHRINK);
	
	add(*mvb);
	
	// show child widgets
	show_all_children();
}
