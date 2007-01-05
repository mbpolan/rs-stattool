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
		// constructor
		CompareSelectDialog(CWnd* pParent = NULL);

		// destructor
		virtual ~CompareSelectDialog();

		// set the players to use
		bool setPlayers(std::vector<PlayerData*> players);

		// return the selected players
		void getSelectedPlayers(CString &p1, CString &p2);

		// Dialog Data
		enum { IDD = IDD_COMPARESELECTDIALOG };

	protected:
		// OnInitDialog override
		virtual BOOL OnInitDialog();

		// ddv/ddx support
		virtual void DoDataExchange(CDataExchange* pDX);
	
		DECLARE_MESSAGE_MAP()
		
		// msg map handlers
		afx_msg void onCombo1SelChange();
		afx_msg void onCombo2SelChange();
	
		// widgets
		CComboBox m_ComboBox1;
		CComboBox m_ComboBox2;

		// selected players
		CString m_Selected1;
		CString m_Selected2;

		// player data
		std::vector<PlayerData*> m_Players;
};


// dialog for comparing two players
class CompareDialog: public CDialog {
	DECLARE_DYNAMIC(CompareDialog)

	public:
		// constructor
		CompareDialog(CWnd* pParent = NULL);

		// destructor
		virtual ~CompareDialog();

		// set the data for two players
		void setCompareData(PlayerData *pd1, PlayerData *pd2);

		// Dialog Data
		enum { IDD = IDD_COMPAREDIALOG };

	protected:
		// OnInitDialog override
		virtual BOOL OnInitDialog();

		// ddv/ddx support
		virtual void DoDataExchange(CDataExchange* pDX);
	
		DECLARE_MESSAGE_MAP()

		// msg map handlers
		afx_msg void onCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult);
	
		// widgets
		CStatic m_Player1Static;
		CStatic m_Player2Static;
		CListCtrl m_CompareList;

		// player data
		PlayerData *m_Players[2];
		int m_HigherPlayer[SKILL_COUNT];
};

#endif
