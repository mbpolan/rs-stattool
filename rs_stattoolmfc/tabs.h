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
// tabs.h: various dialogs for tab widget

#include "afxcmn.h"
#include "common.h"
#include "afxwin.h"

// the initial tab for PlayerNotebook
class InitialTabDialog: public CDialog {
	DECLARE_DYNAMIC(InitialTabDialog)

	public:
		// constructor
		InitialTabDialog(CWnd* pParent = NULL);

		// destructor
		virtual ~InitialTabDialog();
	
		// Dialog Data
		enum { IDD = IDD_TABINITIALDIALOG };
	
	protected:
		// DDX/DDV support
		virtual void DoDataExchange(CDataExchange* pDX);

		// overrides
		virtual BOOL OnInitDialog();
		virtual void OnOK() { };
		virtual void OnCancel() { };

		DECLARE_MESSAGE_MAP()
		
		// widgets
		CStatic m_TitleStatic;
};

// tab used for player tabs
class PlayerTabDialog: public CDialog {
	DECLARE_DYNAMIC(PlayerTabDialog)

	public:
		// constructor
		PlayerTabDialog(CWnd* pParent = NULL);

		// destructor
		virtual ~PlayerTabDialog();

		// fill in the skill table
		void SetSkillData(PlayerData pd);
	
		// Dialog Data
		enum { IDD = IDD_TABDIALOG };

	protected:
		// DDX/DDV support
		virtual void DoDataExchange(CDataExchange* pDX);

		// overrides
		virtual BOOL OnInitDialog();
		virtual void OnOK() { };
		virtual void OnCancel() { };

		DECLARE_MESSAGE_MAP()

		// widgets
		CListCtrl m_SkillList;
};
