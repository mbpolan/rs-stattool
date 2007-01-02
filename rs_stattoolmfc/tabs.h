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

#pragma once
#include "afxcmn.h"
#include "common.h"

// the initial tab for PlayerNotebook
class InitialTabDialog: public CDialog{
	DECLARE_DYNAMIC(InitialTabDialog)

	public:
		// constructor
		InitialTabDialog(CWnd* pParent = NULL);
		virtual ~InitialTabDialog();
	
		// Dialog Data
		enum { IDD = IDD_TABINITIALDIALOG };
	
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		DECLARE_MESSAGE_MAP()
};

// tab used for player tabs
class PlayerTabDialog: public CDialog {
	DECLARE_DYNAMIC(PlayerTabDialog)

	public:
		PlayerTabDialog(CWnd* pParent = NULL);   // standard constructor
		virtual ~PlayerTabDialog();

		// fill in the skill table
		void setSkillData(PlayerData *pd);
	
		// Dialog Data
		enum { IDD = IDD_TABDIALOG };

	protected:
		// DDX/DDV support
		virtual void DoDataExchange(CDataExchange* pDX);

		DECLARE_MESSAGE_MAP()

		// widgets
		CListCtrl m_SkillList;
};
