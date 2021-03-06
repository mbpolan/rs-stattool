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
// playernotebook.h: Custom class for Tab control

#pragma once

#include <map>
#include <vector>
#include "common.h"

// Derived class for custom tab control
class PlayerNotebook: public CTabCtrl {
	DECLARE_DYNAMIC(PlayerNotebook)

	public:
		// constructor
		PlayerNotebook();

		// destructor
		virtual ~PlayerNotebook();

		// initialize the dialogs
		void InitDialogs();

		// activate the tabs
		void ActivateTabs();

		// add a new player tab
		void AddPlayerTab(PlayerData pd);

		// return a vector of all players
		std::vector<PlayerData*> GetPlayers();
		
		// get a player data struct for a player
		PlayerData* GetPlayerData(CString name);

		// return the name of the currently open tab
		CString GetCurrentTabName();

		// close the current tab
		void CloseCurrentTab();
	
	protected:
		DECLARE_MESSAGE_MAP()

		// ids for dialogs/tabs
		std::vector<int> m_DialogID;

		// dialogs for tabs
		std::vector<CDialog*> m_Dialogs;

		// map of players to skill data
		std::map<CString, PlayerData*> m_Players;

		// amount of pages
		int m_PageCount;

		// current open tab
		int m_CurTab;
};
