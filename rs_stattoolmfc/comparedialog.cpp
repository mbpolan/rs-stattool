// comparedialog.cpp : implementation file
//

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

IMPLEMENT_DYNAMIC(CompareSelectDialog, CDialog)

// msg map
BEGIN_MESSAGE_MAP(CompareSelectDialog, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CompareSelectDialog::onCombo1SelChange)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CompareSelectDialog::onCombo2SelChange)
END_MESSAGE_MAP()

// constructor
CompareSelectDialog::CompareSelectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CompareSelectDialog::IDD, pParent) {
}

// destructor
CompareSelectDialog::~CompareSelectDialog() {
}

// set the players to use
bool CompareSelectDialog::setPlayers(std::vector<PlayerData*> players) {
	// validate the list
	if (players.size()!=2) {
		AfxMessageBox(_T("You must have at least two players' stats downloaded."), MB_OK);
		return false;
	}

	// set the list
	m_Players=players;
	return true;
}

// OnInitDialog override
BOOL CompareSelectDialog::OnInitDialog() {
	CDialog::OnInitDialog();

	// make sure we have at least 2 players
	if (m_Players.size()!=2)
		return FALSE;

	// set the player data to both combo boxes
	for (int i=0; i<m_Players.size(); i++) {
		m_ComboBox1.AddString(m_Players[i]->name);
		m_ComboBox2.AddString(m_Players[i]->name);
	}

	// set default selections
	m_ComboBox1.SelectString(0, m_Players[0]->name);
	m_ComboBox2.SelectString(0, m_Players[1]->name);
	m_Selected1=m_Players[0]->name;
	m_Selected2=m_Players[1]->name;

	return TRUE;
}

// return the selected players
void CompareSelectDialog::getSelectedPlayers(CString &p1, CString &p2) {
	p1=m_Selected1;
	p2=m_Selected2;
}

// ddv/ddx support
void CompareSelectDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox1);
	DDX_Control(pDX, IDC_COMBO2, m_ComboBox2);
}

// combo box 1 selection change handler
void CompareSelectDialog::onCombo1SelChange() {
	// get the current selection and validate it
	int sel=m_ComboBox1.GetCurSel();
	if (sel<0)
		return;

	// update selection
	m_ComboBox1.GetLBText(sel, m_Selected1);
}

// combo box 2 selection change handler
void CompareSelectDialog::onCombo2SelChange() {
	// get the current selection and validate it
	int sel=m_ComboBox2.GetCurSel();
	if (sel<0)
		return;

	// update selection
	m_ComboBox2.GetLBText(sel, m_Selected2);
}

/*************************************************************************************/

IMPLEMENT_DYNAMIC(CompareDialog, CDialog)

// msg map
BEGIN_MESSAGE_MAP(CompareDialog, CDialog)
END_MESSAGE_MAP()

// constructor
CompareDialog::CompareDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CompareDialog::IDD, pParent) {
}

// destructor
CompareDialog::~CompareDialog() {
}

// set the data for two players
void CompareDialog::setCompareData(PlayerData *pd1, PlayerData *pd2) {
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
	}

	return TRUE;
}

// ddv/ddx support
void CompareDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAYER0COMPARESTATIC, m_Player1Static);
	DDX_Control(pDX, IDC_PLAYER2COMPARESTATIC, m_Player2Static);
	DDX_Control(pDX, IDC_COMPARELIST, m_CompareList);

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
