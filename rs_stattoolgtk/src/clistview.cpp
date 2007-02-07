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
// clistview.cpp: implementations of CListView class

// project includes
#include "clistview.h"

// constructor
CListView::CListView() {
	construct();
	
}

// clear out all data
void CListView::clear() {
	m_Model->clear();
}

// populate the list
void CListView::populate(const PlayerData &data) {
	// clear out rows
	m_Model->clear();
	
	// iterate over skills
	for (int i=0; i<SKILL_COUNT; i++) {
		SkillData s=data.skills[i];
		
		// make new row
		Gtk::TreeModel::Row row=*(m_Model->append());
		
		// add in values
		row[m_Columns.m_Skill]=s.name;
		row[m_Columns.m_Rank]=s.rank;
		row[m_Columns.m_Level]=s.level;
		row[m_Columns.m_Exp]=s.xp;
	}
}

// build the widget
void CListView::construct() {
	// create the model
	m_Model=Gtk::ListStore::create(m_Columns);
	
	// set the default model
	set_model(m_Model);
	
	// append columns
	append_column("Skill", m_Columns.m_Skill);
	append_column("Rank", m_Columns.m_Rank);
	append_column("Level", m_Columns.m_Level);
	append_column("Exp", m_Columns.m_Exp);
	
	// disable searching
	set_enable_search(false);
	
	// enable clicking headers
	set_headers_clickable(true);
}
