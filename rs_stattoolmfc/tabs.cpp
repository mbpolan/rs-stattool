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
// tabs.cpp: implementation of various Tab dialogs

#include "stdafx.h"
#include "rs_stattoolmfc.h"
#include "tabs.h"

IMPLEMENT_DYNAMIC(InitialTabDialog, CDialog)

// msg map
BEGIN_MESSAGE_MAP(InitialTabDialog, CDialog)
END_MESSAGE_MAP()

// constructor
InitialTabDialog::InitialTabDialog(CWnd* pParent /*=NULL*/)
	: CDialog(InitialTabDialog::IDD, pParent) {

}

InitialTabDialog::~InitialTabDialog() {
}

void InitialTabDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}

/*************************************************************************************/

IMPLEMENT_DYNAMIC(PlayerTabDialog, CDialog)

// msg map
BEGIN_MESSAGE_MAP(PlayerTabDialog, CDialog)
END_MESSAGE_MAP()

// constructor
PlayerTabDialog::PlayerTabDialog(CWnd* pParent /*=NULL*/)
	: CDialog(PlayerTabDialog::IDD, pParent) {
}

// destructor
PlayerTabDialog::~PlayerTabDialog() {
}

// ddx/ddv support
void PlayerTabDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SKILLTABLE, m_SkillList);

	// add columns
	m_SkillList.InsertColumn(0, _T("Skill"), LVCFMT_LEFT, 75);
	m_SkillList.InsertColumn(1, _T("Rank"), LVCFMT_LEFT, 75);
	m_SkillList.InsertColumn(1, _T("Level"), LVCFMT_LEFT, 75);
	m_SkillList.InsertColumn(1, _T("Exp"), LVCFMT_LEFT, 75);
}

// fill in the skill table
void PlayerTabDialog::setSkillData(PlayerData *pd) {
	// iterate over skills
	for (int i=0; i<SKILL_COUNT; i++) {
		SkillData s=pd->skills[i];

		// insert the initial item
		int index=m_SkillList.InsertItem(i, s.name);

		// and fill in the rest of the data
		m_SkillList.SetItemText(index, 1, s.xp);
		m_SkillList.SetItemText(index, 2, s.level);
		m_SkillList.SetItemText(index, 3, s.rank);
	}
}
