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
// dialogs.h: various dialog classes

#pragma once
#include "afxwin.h"


// dialog used during stats save
class SaveDialog : public CDialog {
	DECLARE_DYNAMIC(SaveDialog)
	
	public:
		// save data
		struct SaveOps {
			CString path;
		};

		// constructor
		SaveDialog(CWnd* pParent = NULL);

		// destructor
		virtual ~SaveDialog();

		// get a save struct
		struct SaveOps GetSaveOps();
	
		// Dialog Data
		enum { IDD = IDD_SAVEDIALOG };
	
	protected:
		// ddx/ddv support
		virtual void DoDataExchange(CDataExchange* pDX);

		// msg map handlers
		afx_msg void OnEditChange();
		afx_msg void OnFSButtonClicked();
	
		DECLARE_MESSAGE_MAP()

		// file string
		CString m_FilePath;
	
		// widgets
		CButton m_ShowFSButton;
		CEdit m_FilePathEdit;
};
