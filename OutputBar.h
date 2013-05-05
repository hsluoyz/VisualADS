// OutputBar.h : interface of the COutputBar class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTBAR_H__B6D5C453_618F_4AD3_8C40_FC96BB7CAF11__INCLUDED_)
#define AFX_OUTPUTBAR_H__B6D5C453_618F_4AD3_8C40_FC96BB7CAF11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutputBar : public CBCGPDockingControlBar
{
public:
	COutputBar();

// Attributes
protected:



	BOOL m_bChineseOrEnglish;

	CBCGPTabWnd	m_wndTabs;

	CListCtrl	m_listConsole;
	CListCtrl	m_listReport;
	CListCtrl	m_listRacerCache;
	CListCtrl	m_wndList3;
	CListCtrl   m_listLogonInfo;
// Operations
public:
	HWND m_Logwnd;
	CString itos(int i);
	CString getTime();
	void initConsoleList();
	void insert2ConsoleList(CString strNo, CString strContent);
	void clearConsoleList();
	void initReportList();
	void insert2ReportList(CString strNo, CString strContent);
	void clearReportList();
	void initCacheList();
	void insert2CacheList(CString strNo, CString strContent);
	void clearCacheList();
	void initLogonInfo();
	void insert2LogonInfo(CString cn, CString LogonTime, CString LogofTime, CString un, CString LogStyle);
	void clearLogonInfo();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputBar)
	//}}AFX_VIRTUAL

// Implementation
public:

	
	
	virtual ~COutputBar();

// Generated message map functions
protected:
	
	//{{AFX_MSG(COutputBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTBAR_H__B6D5C453_618F_4AD3_8C40_FC96BB7CAF11__INCLUDED_)
