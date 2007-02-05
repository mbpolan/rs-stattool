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
// rs_stattoolmfc.cpp: implementation of Application 

#include "stdafx.h"
#include "rs_stattoolmfc.h"
#include "maindialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND ghDlg=0;
HACCEL ghAccelTable=0;

// msg map
BEGIN_MESSAGE_MAP(Application, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// Application constructor
Application::Application() {
}

// one and only instance
Application theApp;


// Application initialization
BOOL Application::InitInstance() {
	CWinApp::InitInstance();

	// initialize accelerator table
	ghAccelTable=LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));

	// set a registry key
	SetRegistryKey(_T("Local RSStatTool"));

	// run the core dialog
	MainDialog dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	// FIXME: these are probably not needed
	if (nResponse == IDOK) {
	}
	else if (nResponse == IDCANCEL) {
	}

	return FALSE;
}

// override for accelerator keys
BOOL Application::ProcessMessageFilter(int code, LPMSG lpMsg) {
	if (code<0)
		CWinApp::ProcessMessageFilter(code, lpMsg);
	if (ghDlg && ghAccelTable) {
		if (::TranslateAccelerator(ghDlg, ghAccelTable, lpMsg))
			return TRUE;
	}

	return CWinApp::ProcessMessageFilter(code, lpMsg);
}
