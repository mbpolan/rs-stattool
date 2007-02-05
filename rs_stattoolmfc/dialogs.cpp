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
// dialogs.cpp: implementation of dialog classes

#include "stdafx.h"
#include "rs_stattoolmfc.h"
#include "dialogs.h"

IMPLEMENT_DYNAMIC(SaveDialog, CDialog)

// msg map
BEGIN_MESSAGE_MAP(SaveDialog, CDialog)
	ON_EN_CHANGE(IDC_PATHEDIT, onEditChange)
	ON_BN_CLICKED(IDC_SHOWFSBUTTON, onFSButtonClicked)
END_MESSAGE_MAP()

// constructor
SaveDialog::SaveDialog(CWnd* pParent)
	: CDialog(SaveDialog::IDD, pParent) {
}

// destructor
SaveDialog::~SaveDialog() {
}

// get a save struct
struct SaveDialog::SaveOps SaveDialog::getSaveOps() {
	struct SaveOps sp;
	sp.path=m_FilePath;

	return sp;
}

// ddx/ddv support
void SaveDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOWFSBUTTON, m_ShowFSButton);
	DDX_Control(pDX, IDC_PATHEDIT, m_FilePathEdit);
}

// file path edit text change handler
void SaveDialog::onEditChange() {
	m_FilePathEdit.GetWindowText(m_FilePath);
}

// m_ShowFSButton click handler
void SaveDialog::onFSButtonClicked() {
	// filters for file dialogs
	static char BASED_CODE filter[] = { "Player Stat Files (*.rsp)|*.rsp|" };

	CFileDialog fd(false, "rsp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, this);
	if (fd.DoModal()==IDOK) {
		// save the filename
		m_FilePath=fd.GetPathName();
		m_FilePathEdit.SetWindowText(m_FilePath);
	}
}
