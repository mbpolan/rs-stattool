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
// comparedialog.cpp: implementation of CompareDialog

#include "stdafx.h"
#include "rs_stattoolmfc.h"
#include "comparedialog.h"
#include "utilities.h"

IMPLEMENT_DYNAMIC(CompareSelectDialog, CDialog)

// msg map
BEGIN_MESSAGE_MAP(CompareSelectDialog, CDialog)
	ON_BN_CLICKED(IDC_FILE1CB, OnFileCB1Clicked)
	ON_BN_CLICKED(IDC_FILE2CB, OnFileCB2Clicked)
	ON_BN_CLICKED(IDC_SHOWFS1BUTTON, OnShowFS1ButtonClicked)
	ON_BN_CLICKED(IDC_SHOWFS2BUTTON, OnShowFS2ButtonClicked)
	ON_EN_CHANGE(IDC_PLAYER1FILEEDIT, OnFile1EditChanged)
	ON_EN_CHANGE(IDC_PLAYER2FILEEDIT, OnFile2EditChanged)
	ON_CBN_SELCHANGE(IDC_PLAYER1COMBO, OnPlayer1CBChange)
	ON_CBN_SELCHANGE(IDC_PLAYER2COMBO, OnPlayer2CBChange)
END_MESSAGE_MAP()

// constructor
CompareSelectDialog::CompareSelectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CompareSelectDialog::IDD, pParent) {
}

// destructor
CompareSelectDialog::~CompareSelectDialog() {
}

// set the players to use
bool CompareSelectDialog::SetPlayers(std::vector<PlayerData*> players) {
	// set the list
	m_Players=players;
	return true;
}

// OnInitDialog override
BOOL CompareSelectDialog::OnInitDialog() {
	CDialog::OnInitDialog();
	
	// set the player data to both combo boxes
	for (int i=0; i<m_Players.size(); i++) {
		m_Player1CB.AddString(m_Players[i]->name);
		m_Player2CB.AddString(m_Players[i]->name);
	}
	m_Data.player1="NULL";
	m_Data.player2="NULL";

	if (m_Players.size()>=2) {
		m_Player1CB.SelectString(0, m_Players[0]->name);
		m_Player2CB.SelectString(0, m_Players[1]->name);

		// set defaults
		OnPlayer1CBChange();
		OnPlayer2CBChange();

		// disable file comparison by default
		m_File1CB.EnableWindow(true);
		m_File2CB.EnableWindow(true);
		m_File1CB.SetCheck(false);
		m_File2CB.SetCheck(false);

		// toggle widgets
		OnFileCB1Clicked();
		OnFileCB2Clicked();
	}

	else if (m_Players.size()==1) {
		m_Player1CB.SelectString(0, m_Players[0]->name);
		
		// set defaults
		OnPlayer1CBChange();

		// disable player 1 file comparison
		m_File1CB.EnableWindow(false);
		m_File1CB.SetCheck(false);

		// force player 2 file comparison
		m_File2CB.EnableWindow(false);
		m_File2CB.SetCheck(true);

		// toggle widgets
		OnFileCB1Clicked();
		OnFileCB2Clicked();
	}

	else {
		// enable player 1 file comparison
		m_File1CB.EnableWindow(false);
		m_File1CB.SetCheck(true);

		// enable player 2 file comparison
		m_File2CB.EnableWindow(false);
		m_File2CB.SetCheck(true);

		// toggle widgets
		OnFileCB1Clicked();
		OnFileCB2Clicked();
	}

	return TRUE;
}

// get a struct containing compare data
struct CompareSelectDialog::CompareData CompareSelectDialog::GetCompareStruct() {
	// copy the internal data struct
	int size=m_Players.size();

	// here we modify the new compare options
	if (size>=2)
		return m_Data;

	else if (size==1) {
		m_Data.useFile1=false;
		m_Data.useFile2=true;
	}

	else {
		m_Data.useFile1=true;
		m_Data.useFile2=true;
	}

	return m_Data;
}

// ddv/ddx support
void CompareSelectDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAYER1COMBO, m_Player1CB);
	DDX_Control(pDX, IDC_PLAYER2COMBO, m_Player2CB);
	DDX_Control(pDX, IDC_PLAYER1STATIC, m_Player1Label);
	DDX_Control(pDX, IDC_PLAYER2STATIC, m_Player2Label);
	DDX_Control(pDX, IDC_FILE1STATIC, m_File1Label);
	DDX_Control(pDX, IDC_FILE2STATIC, m_File2Label);
	DDX_Control(pDX, IDC_PLAYER1FILEEDIT, m_File1Edit);
	DDX_Control(pDX, IDC_PLAYER2FILEEDIT, m_File2Edit);
	DDX_Control(pDX, IDC_SHOWFS1BUTTON, m_ShowFS1Button);
	DDX_Control(pDX, IDC_SHOWFS2BUTTON, m_ShowFS2Button);
	DDX_Control(pDX, IDC_FILE1CB, m_File1CB);
	DDX_Control(pDX, IDC_FILE2CB, m_File2CB);
}

// m_File1CB click handler
void CompareSelectDialog::OnFileCB1Clicked() {
	bool t=m_File1CB.GetCheck();
	m_Data.useFile1=(m_File1CB.IsWindowEnabled() && t);

	m_Player1Label.EnableWindow(!t);
	m_Player1CB.EnableWindow(!t);

	m_File1Label.EnableWindow(t);
	m_File1Edit.EnableWindow(t);
	m_ShowFS1Button.EnableWindow(t);
}

// m_File2CB click handler
void CompareSelectDialog::OnFileCB2Clicked() {
	bool t=m_File2CB.GetCheck();
	m_Data.useFile2=(m_File2CB.IsWindowEnabled() && t);

	m_Player2Label.EnableWindow(!t);
	m_Player2CB.EnableWindow(!t);

	m_File2Label.EnableWindow(t);
	m_File2Edit.EnableWindow(t);
	m_ShowFS2Button.EnableWindow(t);
}

// m_ShowFS1Button click handler
void CompareSelectDialog::OnShowFS1ButtonClicked() {
	static char BASED_CODE filter[] = "Player Stat Files (*.rsp)|*.rsp|";

	// run the file chooser
	CFileDialog fd(true, "rsp", NULL, OFN_HIDEREADONLY, filter, this);
	if (fd.DoModal()==IDOK) {
		m_File1Edit.SetWindowTextA(fd.GetPathName());
		m_Data.player1File=fd.GetPathName();
	}
}

// m_ShowFS2Button click handler
void CompareSelectDialog::OnShowFS2ButtonClicked() {
	static char BASED_CODE filter[] = "Player Stat Files (*.rsp)|*.rsp|";

	// run the file chooser
	CFileDialog fd(true, "rsp", NULL, OFN_HIDEREADONLY, filter, this);
	if (fd.DoModal()==IDOK) {
		m_File2Edit.SetWindowTextA(fd.GetPathName());
		m_Data.player2File=fd.GetPathName();
	}
}

// m_File1Edit change handler
void CompareSelectDialog::OnFile1EditChanged() {
	m_File1Edit.GetWindowText(m_Data.player1File);
}

// m_File2Edit change handler
void CompareSelectDialog::OnFile2EditChanged() {
	m_File2Edit.GetWindowText(m_Data.player2File);
}

// m_Player1CB selchange handler
void CompareSelectDialog::OnPlayer1CBChange() {
	m_Player1CB.GetLBText(m_Player1CB.GetCurSel(), m_Data.player1);
}

// m_Player2CB selchange handler
void CompareSelectDialog::OnPlayer2CBChange() {
	m_Player2CB.GetLBText(m_Player2CB.GetCurSel(), m_Data.player2);
}

/*************************************************************************************/

IMPLEMENT_DYNAMIC(CompareDialog, CDialog)

// msg map
BEGIN_MESSAGE_MAP(CompareDialog, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COMPARELIST, OnCustomDrawList)
END_MESSAGE_MAP()

// constructor
CompareDialog::CompareDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CompareDialog::IDD, pParent) {
}

// destructor
CompareDialog::~CompareDialog() {
}

// set the data for two players
void CompareDialog::SetCompareData(PlayerData *pd1, PlayerData *pd2) {
	m_Players[0]=pd1;
	m_Players[1]=pd2;
}

// OnInitDialog override
BOOL CompareDialog::OnInitDialog() {
	CDialog::OnInitDialog();

	// make sure the data is valid
	if (!m_Players[0] || !m_Players[1])
		return FALSE;

	// update static text labels
	m_Player1Static.SetWindowText(m_Players[0]->name);
	m_Player2Static.SetWindowText(m_Players[1]->name);

	// update timestamps
	m_Player1DateStatic.SetWindowText(m_Players[0]->timestamp);
	m_Player2DateStatic.SetWindowText(m_Players[1]->timestamp);

	SkillData *s1=m_Players[0]->skills;
	SkillData *s2=m_Players[1]->skills;

	// fill in the compare list
	for (int i=0; i<SKILL_COUNT; i++) {
		// insert skill name
		int index=m_CompareList.InsertItem(i, s1[i].name);

		// next fill in player 1's data
		m_CompareList.SetItemText(index, 1, s1[i].rank);
		m_CompareList.SetItemText(index, 2, s1[i].level);
		m_CompareList.SetItemText(index, 3, s1[i].xp);

		// and now fill in player 2's data
		m_CompareList.SetItemText(index, 5, s2[i].rank);
		m_CompareList.SetItemText(index, 6, s2[i].level);
		m_CompareList.SetItemText(index, 7, s2[i].xp);

		// compare the exp points of both players
		long exp1=Utils::CStringToLong(s1[i].xp);
		long exp2=Utils::CStringToLong(s2[i].xp);

		// if any of the two skills does not rank, assign -1
		if (s1[i].xp.Find("-")!=-1 || s2[i].xp.Find("-")!=-1)
			m_HigherPlayer[i]=-1;

		// if player 1 has higher exp, assign 0
		else if (exp1>exp2)
			m_HigherPlayer[i]=0;

		// if player 2 has more, assign 1
		else if (exp2>exp1)
			m_HigherPlayer[i]=1;

		// if they're both equal, assign 2
		else if (exp1==exp2)
			m_HigherPlayer[i]=2;

		// set this data with the item for use later
		// in custom drawing for m_CompareList
		m_CompareList.SetItemData(i, (DWORD_PTR) &m_HigherPlayer[i]);
	}

	return TRUE;
}

// ddv/ddx support
void CompareDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAYER0COMPARESTATIC, m_Player1Static);
	DDX_Control(pDX, IDC_PLAYER2COMPARESTATIC, m_Player2Static);
	DDX_Control(pDX, IDC_COMPARELIST, m_CompareList);
	DDX_Control(pDX, IDC_PLAYER1DATESTATIC, m_Player1DateStatic);
	DDX_Control(pDX, IDC_PLAYER2DATESTATIC, m_Player2DateStatic);

	// insert columns to list view
	m_CompareList.InsertColumn(0, _T("Skill"), LVCFMT_LEFT, 75);
	m_CompareList.InsertColumn(1, _T("P1 Rank"), LVCFMT_LEFT, 75);
	m_CompareList.InsertColumn(2, _T("P1 Level"), LVCFMT_LEFT, 75);
	m_CompareList.InsertColumn(3, _T("P1 Exp"), LVCFMT_LEFT, 75);
	m_CompareList.InsertColumn(4, _T(" "), LVCFMT_LEFT, 20);
	m_CompareList.InsertColumn(5, _T("P2 Rank"), LVCFMT_LEFT, 75);
	m_CompareList.InsertColumn(6, _T("P2 Level"), LVCFMT_LEFT, 75);
	m_CompareList.InsertColumn(7, _T("P2 Exp"), LVCFMT_LEFT, 75);
}

// custom draw override for list view to draw colored rows
void CompareDialog::OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult) {
	NMLVCUSTOMDRAW *pLV=reinterpret_cast<NMLVCUSTOMDRAW*> (pNMHDR);
	*pResult=CDRF_DODEFAULT;

	// request notifications for item drawing
	if (pLV->nmcd.dwDrawStage==CDDS_PREPAINT)
		*pResult=CDRF_NOTIFYITEMDRAW;

	// request notifications for subitem drawing
	else if (pLV->nmcd.dwDrawStage==CDDS_ITEMPREPAINT)
		*pResult=CDRF_NOTIFYSUBITEMDRAW;

	// this is where we do the drawing
	else if (pLV->nmcd.dwDrawStage==(CDDS_ITEMPREPAINT | CDDS_SUBITEM)) {
		COLORREF clr;

		// get the item data
		int higherPlayer=*(int*) pLV->nmcd.lItemlParam;

		// see what color to set
		switch(higherPlayer) {
			default:
			case -1: clr=RGB(192, 192, 192); break;
			case 0: clr=RGB(128, 255, 128); break;
			case 1: clr=RGB(255, 128, 128); break;
			case 2: clr=RGB(255, 255, 128); break;
		}

		// set the color
		pLV->clrTextBk=clr;

		*pResult=CDRF_DODEFAULT;
	}
}
