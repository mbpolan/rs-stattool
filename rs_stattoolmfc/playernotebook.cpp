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
// playernotebook.cpp: implementation of PlayerNotebook class

#include "stdafx.h"
#include "rsparser.h"
#include "rs_stattoolmfc.h"
#include "playernotebook.h"
#include "tabs.h"
#include "resource.h"
#include "utilities.h"

IMPLEMENT_DYNAMIC(PlayerNotebook, CTabCtrl)

// msg map
BEGIN_MESSAGE_MAP(PlayerNotebook, CTabCtrl)
END_MESSAGE_MAP()

// constructor
PlayerNotebook::PlayerNotebook() {
	// initially 1 tab
	m_PageCount=1;
	m_CurTab=0;

	// create our inital page
	m_DialogID.push_back(IDD_TABINITIALDIALOG);
	m_Dialogs.push_back(new InitialTabDialog);
}

// destructor
PlayerNotebook::~PlayerNotebook() {
	// delete dialogs
	for (int i=0; i<m_PageCount; i++)
		delete m_Dialogs[i];

	// delete saved player data
	for (std::map<CString, PlayerData*>::iterator it=m_Players.begin(); 
		 it!=m_Players.end(); ++it) {
		if ((*it).second)
			delete (*it).second;
	}
}

// initialize the dialogs
void PlayerNotebook::initDialogs() {
	// create the dialog
	m_Dialogs[0]->Create(m_DialogID[0], GetParent());
}

// activate the tabs
void PlayerNotebook::activateTabs() {
	// get our selected dialog
	int sel=GetCurSel();
	if (m_Dialogs[sel]->m_hWnd)
		m_Dialogs[sel]->ShowWindow(SW_HIDE);

	// grab the client rect and window rect
	CRect rcl, rwnd;
	GetClientRect(rcl);
	AdjustRect(false, rcl);
	GetWindowRect(rwnd);
	GetParent()->ScreenToClient(rwnd);

	// offset the client rect to start at the top left corner
	// of the bounding wnd rect
	rcl.OffsetRect(rwnd.left, rwnd.top);
	
	// hide every dialog
	for (int i=0; i<m_PageCount; i++)
		m_Dialogs[i]->SetWindowPos(&wndTop, rcl.left, rcl.top, rcl.Width(), rcl.Height(), SWP_HIDEWINDOW);

	// but show our selected one
	m_Dialogs[sel]->SetWindowPos(&wndTop, rcl.left, rcl.top, rcl.Width(), rcl.Height(), SWP_SHOWWINDOW);
	m_Dialogs[sel]->ShowWindow(SW_SHOW);
}

// add a new player tab
void PlayerNotebook::addPlayerTab(PlayerData pd) {
	// copy the player data
	PlayerData *pdc=new PlayerData(pd);
	pdc->timestamp="-";

	// see if a player with this name already exists
	int i=1; // start at 1 since first tab is not mapped
	for (std::map<CString, PlayerData*>::iterator it=m_Players.begin(); it!=m_Players.end(); ++it) {
		// bring the names to lowercase
		CString s1=(*it).first;
		s1=s1.MakeLower();
		CString s2=pd.name;
		s2=s2.MakeLower();

		// and compare them
		if (s1==s2) {
			CDialog *diag=m_Dialogs[i];
			if (diag) {
				// cast the dialog appropriately and update skill data
				PlayerTabDialog *tdiag=static_cast<PlayerTabDialog*> (diag);
				if (tdiag) {
					tdiag->setSkillData(pd);

					// delete old data struct and replace it
					delete m_Players[pd.name];
					m_Players[pd.name]=pdc;
				}
			}

			return;
		}
		i++;
	}

	// calculate the total level and exp
	pdc->overallExp=Utils::intToCString(RSParser::calculateTotalExp(pd));
	pdc->overallLvl=Utils::intToCString(RSParser::calculateTotalLevel(pd));

	// first we create a new tab
	m_DialogID.push_back(IDD_TABDIALOG);
	m_Dialogs.push_back(new PlayerTabDialog);
	m_Dialogs[m_PageCount]->Create(m_DialogID[m_PageCount], GetParent());

	// insert a new tab with the name
	InsertItem(m_PageCount, CString(pd.name));
	m_Players[pd.name]=pdc;

	// now fill in the skill table
	static_cast<PlayerTabDialog*> (m_Dialogs[m_PageCount])->setSkillData(pd);

	// set this tab as our selection
	SetCurSel(m_PageCount);
	activateTabs();

	// incremene the amount of pages we have
	m_PageCount++;
}

// return a vector of all players
std::vector<PlayerData*> PlayerNotebook::getPlayers() {
	std::vector<PlayerData*> vec;
	for (std::map<CString, PlayerData*>::iterator it=m_Players.begin();
		 it!=m_Players.end(); ++it) {
			 if ((*it).second)
				 vec.push_back((*it).second);
	}
	return vec;
}

// get a player data struct for a player
PlayerData* PlayerNotebook::getPlayerData(CString name) {
	return m_Players[name];
}

// return the name of the currently open tab
CString PlayerNotebook::getCurrentTabName() {
	// get the currently selected tab
	int sel=GetCurSel();

	// tab item struct (we only want text)
	TCITEM tab;
	char buf[256]={0};
	tab.pszText=buf;
	tab.cchTextMax=256;
	tab.mask=TCIF_TEXT;
	GetItem(sel, &tab);

	// get the tab text and return it as a CString
	CString name=CString(buf);

	return name;
}

// close the current tab
void PlayerNotebook::closeCurrentTab() {
	int sel=GetCurSel();

	// make sure the first page cannot be closed
	if (sel==0)
		return;

	// first delete the player data struct
	CString name=getCurrentTabName();
	for (std::map<CString, PlayerData*>::iterator it=m_Players.begin(); it!=m_Players.end(); ++it) {
		if ((*it).first==name) {
			delete (*it).second;
			m_Players.erase(it);
			break;
		}
	}

	// delete the dialog
	int i=0;
	for (std::vector<CDialog*>::iterator it=m_Dialogs.begin(); it!=m_Dialogs.end(); ++it) {
		if (i==sel) {
			delete (*it);
			it=m_Dialogs.erase(it);
			break;
		}
		i++;
	}

	// decrement page count
	m_PageCount--;
	if (m_PageCount<0)
		m_PageCount=0;

	// set the selection to the previous tab
	SetCurSel(sel-1);
	activateTabs();

	// close the tab
	DeleteItem(sel);
}
