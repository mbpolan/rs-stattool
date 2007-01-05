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
// maindialog.cpp: implementation of MainDialog class

#include "stdafx.h"
#include "common.h"
#include "comparedialog.h"
#include "aboutdialog.h"
#include "rs_stattoolmfc.h"
#include "maindialog.h"
#include "playerinfodialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// msg map
BEGIN_MESSAGE_MAP(MainDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GOBUTTON, onGoButtonClicked)
	ON_COMMAND(ID_FILE_QUIT, onFileQuit)
	ON_COMMAND(ID_TOOLS_COMPARE, onToolsCompare)
	ON_COMMAND(ID_HELP_ABOUT, onHelpAbout)
	ON_COMMAND(ID_POPUP_VIEWINFO, onViewPlayerInfo)
	ON_COMMAND(ID_POPUP_CLOSETAB, onTabsClose)
	ON_MESSAGE(WM_RSTHREAD_STARTED, onThreadStarted)
	ON_MESSAGE(WM_RSTHREAD_FINISHED, onThreadFinished)
	ON_NOTIFY(TCN_SELCHANGE, IDC_PLAYERNOTEBOOK, onTabSelChange)
	ON_NOTIFY(NM_RCLICK, IDC_PLAYERNOTEBOOK, onTabRClick)
END_MESSAGE_MAP()

// constructor
MainDialog::MainDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MainDialog::IDD, pParent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// destructor
MainDialog::~MainDialog() {
	if (m_Menu)
		delete m_Menu;
}

// ddx/ddv support
void MainDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAYERNAMEEDIT, m_PlayerNameEdit);
	DDX_Control(pDX, IDC_GOBUTTON, m_GoButton);
	DDX_Control(pDX, IDC_PLAYERNOTEBOOK, m_NB);
}

// initialization
BOOL MainDialog::OnInitDialog() {
	CDialog::OnInitDialog();
	
	// set icons
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// initialize the menu bar
	m_Menu=new CMenu;
	m_Menu->LoadMenu(IDR_MENU1);
	SetMenu(m_Menu);

	// initialize popup menu
	m_PopupMenu.LoadMenu(IDR_MENU2);

	// initialize tab widget
	m_NB.initDialogs();
	m_NB.InsertItem(0, "RS Stat Tool"); // initial page
	m_NB.activateTabs();

	// status bar indicators
	UINT BASED_CODE sIndicators [] = { IDS_INDICATOR_STATUS };

	// create status bar
	m_StatBar.Create(this);
	m_StatBar.SetIndicators(sIndicators, 1);

	// draw it to the dialog
	CRect cr;
	GetClientRect(&cr);
	m_StatBar.SetPaneInfo(0, IDS_INDICATOR_STATUS, SBPS_NORMAL, cr.Width());
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, IDS_INDICATOR_STATUS);

	return TRUE;
}

// paint handler
void MainDialog::OnPaint() {
	if (IsIconic()) {
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
		CDialog::OnPaint();
}

// icon drag handler
HCURSOR MainDialog::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}

// file menu quit handler
void MainDialog::onFileQuit() {
	EndDialog(1);
}

// tools menu compare handler
void MainDialog::onToolsCompare() {
	// first, prepare the selection dialog
	CompareSelectDialog sdiag;
	if (!sdiag.setPlayers(m_NB.getPlayers()))
		return;

	// run it now
	if (sdiag.DoModal()==IDOK) {
		// get our selections
		CString p1, p2;
		sdiag.getSelectedPlayers(p1, p2);

		// get player data
		PlayerData *pd1=m_NB.getPlayerData(p1);
		PlayerData *pd2=m_NB.getPlayerData(p2);

		// make sure these are valid
		if (!pd1 || !pd2)
			return;

		// run the compare dialog itself now
		CompareDialog cdiag;
		cdiag.setCompareData(pd1, pd2);
		cdiag.DoModal();
	}
}

// help menu about handler
void MainDialog::onHelpAbout() {
	CAboutDlg diag;
	diag.DoModal();
}

// view player info handler for tab popup
void MainDialog::onViewPlayerInfo() {
	// first get the player data struct
	PlayerData *pd=m_NB.getPlayerData(m_NB.getCurrentTabName());
	if (pd) {
		// display the info dialog
		PlayerInfoDialog pid;
		pid.setPlayerInfo(_T(pd->name), _T(pd->overallLvl), _T(pd->overallExp));
		pid.DoModal();
	}
}

// close handler for tab popup
void MainDialog::onTabsClose() {
	m_NB.closeCurrentTab();
}

// selection change notify handler
void MainDialog::onTabSelChange(NMHDR *pNMHDR, LRESULT *pResult) {
	m_NB.activateTabs();
	*pResult=0;
}

// go button click handler
void MainDialog::onGoButtonClicked() {
	// get the player's name
	CString name;
	m_PlayerNameEdit.GetWindowText(name);

	// and get the data
	m_Parser.getPlayerData(name, this);

	// display message
	m_StatBar.SetPaneText(0, _T("Beginning transfer..."), true);
}

// right click on tab handler
void MainDialog::onTabRClick(NMHDR *pNMHDR, LRESULT *pResult) {
	// display the context menu at the cursor pos
	CPoint pos;
	GetCursorPos(&pos);
	CMenu *sub=m_PopupMenu.GetSubMenu(0);
	sub->TrackPopupMenu(0, pos.x, pos.y, this);

	*pResult = 0;
}

// rs thread started
LRESULT MainDialog::onThreadStarted(WPARAM wParam, LPARAM lParam) {
	// disable the go button
	m_GoButton.EnableWindow(false);

	// update the status bar
	m_StatBar.SetPaneText(0, _T("Downloading stats..."));
	return 0;
}

// rs thread finished
LRESULT MainDialog::onThreadFinished(WPARAM wParam, LPARAM lParam) {
	// reenable the go button
	m_GoButton.EnableWindow(true);

	// parse the html data returned
	bool ok=true;
	PlayerData pd=RSParser::parseHTML(m_Parser.m_Data.data, &ok);
	if (ok) {
		// update the status bar
		m_StatBar.SetPaneText(0, _T("Done"));

		// add a player tab
		m_NB.addPlayerTab(pd);
	}
	else
		m_StatBar.SetPaneText(0, _T("** Player is not in the high scores **"));

	// free the allocated data
	if (m_Parser.m_Data.data)
		delete m_Parser.m_Data.data;

	return 0;
}
