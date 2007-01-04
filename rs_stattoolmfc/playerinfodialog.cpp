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
// playerinfodialog.cpp: implementation of PlayerInfoDialog class

#include "stdafx.h"
#include "rs_stattoolmfc.h"
#include "playerinfodialog.h"

IMPLEMENT_DYNAMIC(PlayerInfoDialog, CDialog)

// msg map
BEGIN_MESSAGE_MAP(PlayerInfoDialog, CDialog)
END_MESSAGE_MAP()

// constructor
PlayerInfoDialog::PlayerInfoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(PlayerInfoDialog::IDD, pParent) {
}

// destructor
PlayerInfoDialog::~PlayerInfoDialog() {
}

// set the player info
void PlayerInfoDialog::setPlayerInfo(const CString &name, const CString &level, const CString &exp) {
	// set the labels for dialog initialize later on
	m_Name=name;
	m_Level=level;
	m_Exp=exp;
}

// ddx/ddv support
void PlayerInfoDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PPLAYERNAMESTATIC, m_PlayerNameEdit);
	DDX_Control(pDX, IDC_PTOTALLEVELSTATIC, m_TotalLevelEdit);
	DDX_Control(pDX, IDC_PTOTALEXPSTATIC, m_TotalExpEdit);
}

// override for OnInitDialog
BOOL PlayerInfoDialog::OnInitDialog() {
	CDialog::OnInitDialog();

	// update the labels
	m_PlayerNameEdit.SetWindowText(m_Name);
	m_TotalLevelEdit.SetWindowText(m_Level);
	m_TotalExpEdit.SetWindowText(m_Exp);

	return TRUE;
}
