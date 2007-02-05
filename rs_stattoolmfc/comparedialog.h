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
// comparedialog.h: the CompareDialog and related classs

#ifndef COMPAREDIALOG_H
#define COMPAREDIALOG_H

#include "afxwin.h"
#include "afxcmn.h"
#include "common.h"
#include "resource.h"
#include <vector>

// dialog for selecting players for comparison
class CompareSelectDialog : public CDialog {
	DECLARE_DYNAMIC(CompareSelectDialog)
	
	public:
		// data for comparison
		struct CompareData {
			// player names
			CString player1;
			CString player2;

			// file paths
			CString player1File;
			CString player2File;

			// comparison types
			bool useFile1;
			bool useFile2;
		};

		// constructor
		CompareSelectDialog(CWnd* pParent = NULL);

		// destructor
		virtual ~CompareSelectDialog();

		// set the players to use
		bool SetPlayers(std::vector<PlayerData*> players);

		// get a struct containing compare data
		struct CompareData GetCompareStruct();

		// Dialog Data
		enum { IDD = IDD_COMPARESELECTDIALOG };

	protected:
		// OnInitDialog override
		virtual BOOL OnInitDialog();

		// ddv/ddx support
		virtual void DoDataExchange(CDataExchange* pDX);
	
		DECLARE_MESSAGE_MAP()

		// msg map handlers
		afx_msg void OnFileCB1Clicked();
		afx_msg void OnFileCB2Clicked();
		afx_msg void OnShowFS1ButtonClicked();
		afx_msg void OnShowFS2ButtonClicked();
		afx_msg void OnFile1EditChanged();
		afx_msg void OnFile2EditChanged();
		afx_msg void OnPlayer1CBChange();
		afx_msg void OnPlayer2CBChange();

		// combo boxes
		CComboBox m_Player1CB;
		CComboBox m_Player2CB;
		
		// labels
		CStatic m_Player1Label;
		CStatic m_Player2Label;
		CStatic m_File1Label;
		CStatic m_File2Label;
		
		// edits
		CEdit m_File1Edit;
		CEdit m_File2Edit;
		
		// buttons
		CButton m_File1CB;
		CButton m_File2CB;
		CButton m_ShowFS1Button;
		CButton m_ShowFS2Button;

		// player data
		std::vector<PlayerData*> m_Players;

		// stored compare data
		CompareData m_Data;
};

/***********************************************************************************/

// dialog for comparing two players
class CompareDialog: public CDialog {
	DECLARE_DYNAMIC(CompareDialog)

	public:
		// constructor
		CompareDialog(CWnd* pParent = NULL);

		// destructor
		virtual ~CompareDialog();

		// set the data for two players
		void SetCompareData(PlayerData *pd1, PlayerData *pd2);

		// Dialog Data
		enum { IDD = IDD_COMPAREDIALOG };

	protected:
		// OnInitDialog override
		virtual BOOL OnInitDialog();

		// ddv/ddx support
		virtual void DoDataExchange(CDataExchange* pDX);
	
		DECLARE_MESSAGE_MAP()

		// msg map handlers
		afx_msg void OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult);
	
		// widgets
		CStatic m_Player1Static;
		CStatic m_Player2Static;
		CStatic m_Player1DateStatic;
		CStatic m_Player2DateStatic;
		CListCtrl m_CompareList;

		// player data
		PlayerData *m_Players[2];
		int m_HigherPlayer[SKILL_COUNT];
};

#endif
