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
// playerinfodialog.h: the PlayerInfoDialog class

#ifndef PLAYERINFODIALOG_H
#define PLAYERINFODIALOG_H

#include "afxwin.h"
#include "resource.h"

// dialog for displaying a player's info
class PlayerInfoDialog : public CDialog{
	DECLARE_DYNAMIC(PlayerInfoDialog)

	public:
		// constructor
		PlayerInfoDialog(CWnd* pParent = NULL);

		// destructor
		virtual ~PlayerInfoDialog();

		// set the player info
		void setPlayerInfo(const CString &name, const CString &level, const CString &exp);

		// Dialog Data
		enum { IDD = IDD_PLAYERINFODIALOG };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		// override for OnInitDialog
		virtual BOOL OnInitDialog();

		DECLARE_MESSAGE_MAP()

		// widgets
		CStatic m_PlayerNameEdit;
		CStatic m_TotalLevelEdit;
		CStatic m_TotalExpEdit;

		// strings
		CString m_Name;
		CString m_Level;
		CString m_Exp;
};

#endif
