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
// maindialog.h: the MainDialog class

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "playernotebook.h"
#include "rsparser.h"
#include "resource.h"

// core MFC application dialog
class MainDialog: public CDialog {
	public:
		// constructor
		MainDialog(CWnd* pParent = NULL);

		// destructor
		virtual ~MainDialog();
	
		// Dialog Data
		enum { IDD = IDD_RS_STATTOOLMFC_DIALOG };
	
	protected:
		// our icon
		HICON m_hIcon;
		
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
		
		// msg map functions
		virtual BOOL OnInitDialog();
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		afx_msg void OnOK() { };
		afx_msg void OnClose() { };
		afx_msg void OnFileOpen();
		afx_msg void OnFileQuit();
		afx_msg void OnToolsCompare();
		afx_msg void OnHelpAbout();
		afx_msg void OnViewPlayerInfo();
		afx_msg void OnSaveStats();
		afx_msg void OnTabsClose();
		afx_msg void OnGoButtonClicked();
		afx_msg void OnRefresh();
		afx_msg void OnTabSelChange(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnTabRClick(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg LRESULT OnThreadStarted(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnThreadFinished(WPARAM wParam, LPARAM lParam);
		
		DECLARE_MESSAGE_MAP()

		// menu
		CMenu *m_Menu;
		CMenu m_PopupMenu;

		// widgets
		CEdit m_PlayerNameEdit;
		CButton m_GoButton;
		PlayerNotebook m_NB;
		CStatusBar m_StatBar;

		// parser for html
		RSParser m_Parser;
};
