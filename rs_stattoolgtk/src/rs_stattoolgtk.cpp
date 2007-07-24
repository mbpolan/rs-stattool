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
// rs_stattoolgtk.cpp: main file where everything is initiated

// necessary gtk includes
#include <gtkmm/imagemenuitem.h>
#include <gtkmm/main.h>
#include <gtkmm/statusicon.h>
#include <gtkmm/stock.h>

// project includes
#include "mainwindow.h"

static void on_app_quit(Gtk::Main *app);

// signal handler for signal_activate
static void on_icon_activate(GtkWidget *widget, gpointer data) {
	static bool visible=true;
	
	// show the window if it's already visible, otherwise hide it
	MainWindow *mw=(MainWindow*) data;
	if (mw) {
		if (mw->is_visible())
			mw->hide();
		else
			mw->show();
	}
}

// signal handler for signal_popup_menu
static void on_icon_popup_menu(GtkWidget *widget, guint button, guint atime, gpointer data) {
	// cast the menu
	Gtk::Menu *menu=(Gtk::Menu*) data;
	
	// popup the menu
	menu->popup(button, atime);
}

// signal handler for main window hide signals
static void on_mainwindow_hide(MainWindow *mw, Glib::RefPtr<Gtk::StatusIcon> icon, Gtk::Main *app) {
	// close the application if the status icon is not embedded
	if (!icon->is_embedded())
		on_app_quit(app);
}

// signal handler for context menu and mainwindow quit commands
static void on_app_quit(Gtk::Main *app) {
	app->quit();
}

// main
int main(int argc, char *argv[]) {
	// gtk event loop init
	Gtk::Main app(argc, argv);
	
	// initialize glib threads
	Glib::thread_init();
	
	// try to create the statusicon
	Glib::RefPtr<Gtk::StatusIcon> sIcon=Gtk::StatusIcon::create_from_file("status.png");
	if (!sIcon || !sIcon->get_pixbuf()) {
		// try again, this time with a stock icon
		sIcon=Gtk::StatusIcon::create(Gtk::Stock::MISSING_IMAGE);
	}
	
	// main window allocation
	MainWindow mw;
	mw.show();
	
	// connect signals
	mw.signal_hide().connect(sigc::bind(sigc::ptr_fun(on_mainwindow_hide), &mw, sIcon, &app));
	mw.signal_quit().connect(sigc::bind(sigc::ptr_fun(on_app_quit), &app));
	
	// if the icon is allocated, use it to control the application
	if (sIcon) {
		// set a tooltip
		sIcon->set_tooltip("Show or hide RuneScape Stat Tool");
		
		// create a popup menu for the icon
		Gtk::Menu iconMenu;
		
		// menu actions
		Gtk::Image showImage(Gtk::Stock::FULLSCREEN, Gtk::ICON_SIZE_MENU);
		Gtk::Image quitImage(Gtk::Stock::QUIT, Gtk::ICON_SIZE_MENU);
		
		Gtk::ImageMenuItem showItem(showImage, "Show/Hide");
		Gtk::ImageMenuItem quitItem(quitImage, "Quit");
		
		showImage.show();
		quitImage.show();
		
		// attach actions
		iconMenu.attach(showItem, 0, 1, 0, 1);
		iconMenu.attach(quitItem, 0, 1, 1, 2);
		
		showItem.show();
		quitItem.show();
		
		// NOTE: apparently, gtkmm 2.10 doesn't implement these signals, so default
		// to the C API for now
		GtkStatusIcon *gsIcon=sIcon->gobj();
		g_signal_connect(G_OBJECT(gsIcon), "activate", G_CALLBACK(on_icon_activate), &mw);
		g_signal_connect(G_OBJECT(gsIcon), "popup-menu", G_CALLBACK(on_icon_popup_menu), &iconMenu);
		
		// connect menu signals
		showItem.signal_activate().connect(sigc::bind(sigc::ptr_fun(on_icon_activate), (GtkWidget*) gsIcon, (gpointer) &mw));
		quitItem.signal_activate().connect(sigc::bind(sigc::ptr_fun(on_app_quit), &app));
		
		app.run();
	}
	
	// this should never happen...
	else
		app.run(mw);
	
	return 0;
}
