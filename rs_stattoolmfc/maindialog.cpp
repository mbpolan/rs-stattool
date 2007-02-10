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
#include "dialogs.h"
#include "io.h"
#include "rs_stattoolmfc.h"
#include "maindialog.h"
#include "playerinfodialog.h"
#include "utilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// msg map
BEGIN_MESSAGE_MAP(MainDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GOBUTTON, OnGoButtonClicked)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_QUIT, OnFileQuit)
	ON_COMMAND(ID_TOOLS_COMPARE, OnToolsCompare)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(ID_POPUP_VIEWINFO, OnViewPlayerInfo)
	ON_COMMAND(ID_POPUP_SAVESTATS, OnSaveStats)
	ON_COMMAND(ID_POPUP_REFRESH, OnRefresh)
	ON_COMMAND(ID_POPUP_CLOSETAB, OnTabsClose)
	ON_MESSAGE(WM_RSTHREAD_STARTED, OnThreadStarted)
	ON_MESSAGE(WM_RSTHREAD_FINISHED, OnThreadFinished)
	ON_NOTIFY(TCN_SELCHANGE, IDC_PLAYERNOTEBOOK, OnTabSelChange)
	ON_NOTIFY(NM_RCLICK, IDC_PLAYERNOTEBOOK, OnTabRClick)
	ON_WM_SYSCOMMAND()
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

	// limit player names to 12 characters
	m_PlayerNameEdit.SetLimitText(12);
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
	m_NB.InitDialogs();
	m_NB.InsertItem(0, "RS Stat Tool"); // initial page
	m_NB.ActivateTabs();

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

	ghDlg=m_hWnd;
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

// syscommand handler
void MainDialog::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0)==SC_CLOSE)
		EndDialog(IDOK);
	else
		CDialog::OnSysCommand(nID, lParam);
}

// file menu open handler
void MainDialog::OnFileOpen() {
	static char BASED_CODE filter[] = "Player Stat Files (*.rsp)|*.rsp|";

	// run file dialog first
	CFileDialog fd(true, "rsp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
				   filter, this);
	if (fd.DoModal()==IDOK) {
		// get the file path
		CString path=fd.GetPathName();

		// and load the data
		PlayerData pd;
		if (!IOHandler::LoadPlayerStats(path, pd)) {
			AfxMessageBox(Utils::TranslateIOError(IOHandler::Error), MB_OK);
			return;
		}

		// add a new tab
		m_NB.AddPlayerTab(pd);
	}
}

// file menu quit handler
void MainDialog::OnFileQuit() {
	EndDialog(1);
}

// tools menu compare handler
void MainDialog::OnToolsCompare() {
	// first, prepare the selection dialog
	CompareSelectDialog sdiag;
	if (!sdiag.SetPlayers(m_NB.GetPlayers()))
		return;

	// run it now
	if (sdiag.DoModal()==IDOK) {
		CompareSelectDialog::CompareData cd=sdiag.GetCompareStruct();

		// player data structs
		PlayerData p1, p2;

		// compare using both files
		if (cd.useFile1 && cd.useFile2) {
			if (!IOHandler::LoadPlayerStats(cd.player1File, p1) ||
				!IOHandler::LoadPlayerStats(cd.player2File, p2)) {
					AfxMessageBox(Utils::TranslateIOError(IOHandler::Error), MB_OK);
					return;
			}
		}

		// compare player 1 with file
		else if (!cd.useFile1 && cd.useFile2) {
			if (m_NB.GetPlayerData(cd.player1))
				p1=*m_NB.GetPlayerData(cd.player1);

			if (!IOHandler::LoadPlayerStats(cd.player2File, p2)) {
				AfxMessageBox(Utils::TranslateIOError(IOHandler::Error), MB_OK);
				return;
			}
		}

		// compare file with player 2
		else if (cd.useFile1 && !cd.useFile2) {
			if (m_NB.GetPlayerData(cd.player2))
				p2=*m_NB.GetPlayerData(cd.player2);

			if (!IOHandler::LoadPlayerStats(cd.player1File, p1)) {
				AfxMessageBox(Utils::TranslateIOError(IOHandler::Error), MB_OK);
				return;
			}
		}

		// compare two players
		else {
			if (m_NB.GetPlayerData(cd.player1))
				p1=*m_NB.GetPlayerData(cd.player1);
			if (m_NB.GetPlayerData(cd.player2))
				p2=*m_NB.GetPlayerData(cd.player2);
		}
		
		// allocate a compare dialog
		CompareDialog cdialog;

		// set the player data and run the dialog
		cdialog.SetCompareData(&p1, &p2);
		cdialog.DoModal();
	}
}

// help menu about handler
void MainDialog::OnHelpAbout() {
	CAboutDlg diag;
	diag.DoModal();
}

// view player info handler for tab popup
void MainDialog::OnViewPlayerInfo() {
	// first get the player data struct
	PlayerData *pd=m_NB.GetPlayerData(m_NB.GetCurrentTabName());
	if (pd) {
		// display the info dialog
		PlayerInfoDialog pid;
		pid.SetPlayerInfo(_T(pd->name), _T(pd->overallLvl), _T(pd->overallExp));
		pid.DoModal();
	}
}

// save player stats to file
void MainDialog::OnSaveStats() {
	// get the player to save
	PlayerData *pd=m_NB.GetPlayerData(m_NB.GetCurrentTabName());
	if (!pd)
		return;

	// run the save dialog
	SaveDialog sdiag;
	if (sdiag.DoModal()==IDOK) {
		// get the save data struct
		struct SaveDialog::SaveOps sp=sdiag.GetSaveOps();

		// and ask the iohandler to save this file
		if (!IOHandler::SavePlayerStats(sp.path, *pd))
			AfxMessageBox(_T("Unable to save file."), MB_OK);
	}
}

// close handler for tab popup
void MainDialog::OnTabsClose() {
	m_NB.CloseCurrentTab();
}

// selection change notify handler
void MainDialog::OnTabSelChange(NMHDR *pNMHDR, LRESULT *pResult) {
	m_NB.ActivateTabs();
	*pResult=0;
}

// right click on tab handler
void MainDialog::OnTabRClick(NMHDR *pNMHDR, LRESULT *pResult) {
	// display the context menu at the cursor pos
	CPoint pos;
	GetCursorPos(&pos);
	CMenu *sub=m_PopupMenu.GetSubMenu(0);
	sub->TrackPopupMenu(0, pos.x, pos.y, this);

	*pResult = 0;
}

// go button click handler
void MainDialog::OnGoButtonClicked() {
	// get the player's name
	CString name;
	m_PlayerNameEdit.GetWindowText(name);

	// and get the data
	m_Parser.GetPlayerData(name, this);

	// display message
	m_StatBar.SetPaneText(0, _T("Beginning transfer..."), true);
}

// refresh popup handler
void MainDialog::OnRefresh() {
	// get the current tab's name
	CString name=m_NB.GetCurrentTabName();

	// make sure this is a player tab
	if (name=="RS Stat Tool")
		return;

	// get the new data
	m_Parser.GetPlayerData(name, this);
}

// rs thread started
LRESULT MainDialog::OnThreadStarted(WPARAM wParam, LPARAM lParam) {
	// disable the go button
	m_GoButton.EnableWindow(false);

	// update the status bar
	m_StatBar.SetPaneText(0, _T("Downloading stats..."));
	return 0;
}

// rs thread finished
LRESULT MainDialog::OnThreadFinished(WPARAM wParam, LPARAM lParam) {
	// reenable the go button
	m_GoButton.EnableWindow(true);

	// parse the html data returned
	bool ok=true;
	PlayerData pd=RSParser::ParseHTML(m_Parser.m_Data.data, &ok);
	if (ok) {
		// update the status bar
		m_StatBar.SetPaneText(0, _T("Done"));

		// add a player tab
		m_NB.AddPlayerTab(pd);
	}
	else
		m_StatBar.SetPaneText(0, _T("** Player is not in the high scores **"));

	// free the allocated data
	if (m_Parser.m_Data.data)
		delete m_Parser.m_Data.data;

	return 0;
}
