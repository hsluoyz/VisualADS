#if !defined(AFX_VISUALADSABOUTDLG_H__EA595137_F781_4AAA_BE2D_F89DFE3BB74E__INCLUDED_)
#define AFX_VISUALADSABOUTDLG_H__EA595137_F781_4AAA_BE2D_F89DFE3BB74E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VisualADSAboutDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVisualADSAboutDlg dialog

class CVisualADSAboutDlg : public CDialog
{
// Construction
public:
	CVisualADSAboutDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVisualADSAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CEdit	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisualADSAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVisualADSAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCompanyUrl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISUALADSABOUTDLG_H__EA595137_F781_4AAA_BE2D_F89DFE3BB74E__INCLUDED_)
