// VisualADS.h : main header file for the VISUALADS application
//

#if !defined(AFX_VISUALADS_H__9BB0B230_AD48_465C_893D_3B3699A39C22__INCLUDED_)
#define AFX_VISUALADS_H__9BB0B230_AD48_465C_893D_3B3699A39C22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CVisualADSApp:
// See VisualADS.cpp for the implementation of this class
//

class CVisualADSApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	CString m_strInputDir;


	CVisualADSApp();
	void showAboutDlg();

	BOOL FolderExist(CString strPath);
	void initInputDir();
	BOOL getLanguage();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisualADSApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CVisualADSApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CVisualADSApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISUALADS_H__9BB0B230_AD48_465C_893D_3B3699A39C22__INCLUDED_)
