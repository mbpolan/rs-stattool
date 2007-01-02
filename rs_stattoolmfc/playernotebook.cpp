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
#include "rs_stattoolmfc.h"
#include "playernotebook.h"
#include "tabs.h"
#include "resource.h"

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
	for (int i=0; i<m_PageCount; i++)
		delete m_Dialogs[i];
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
void PlayerNotebook::addPlayerTab(PlayerData *pd) {
	// first we create a new tab
	m_DialogID.push_back(IDD_TABDIALOG);
	m_Dialogs.push_back(new PlayerTabDialog);
	m_Dialogs[m_PageCount]->Create(m_DialogID[m_PageCount], GetParent());

	// see if we should fill in the tab's skill list
	if (!pd) {
		// insert a blank tab
		InsertItem(m_PageCount, "Player");
	}
	else {
		// insert a new tab with the name
		InsertItem(m_PageCount, CString(pd->name));

		// now fill in the skill table
		static_cast<PlayerTabDialog*> (m_Dialogs[m_PageCount])->setSkillData(pd);
	}

	// set this tab as our selection
	SetCurSel(m_PageCount);
	activateTabs();

	// incremene the amount of pages we have
	m_PageCount++;
}

// close the current tab
void PlayerNotebook::closeCurrentTab() {
	int sel=GetCurSel();

	// make sure the first page cannot be closed
	if (sel==0)
		return;

	// otherwise, close the tab
	DeleteItem(sel);

	// delete the dialog
	delete m_Dialogs[sel];

	// decrement page count
	m_PageCount--;
	if (m_PageCount<0)
		m_PageCount=0;

	// set the selection to 0
	SetCurSel(0);
	activateTabs();
}
