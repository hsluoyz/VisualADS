// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__A2E3FE84_7646_4501_82BB_20CFFA80F87C__INCLUDED_)
#define AFX_MAINFRM_H__A2E3FE84_7646_4501_82BB_20CFFA80F87C__INCLUDED_

#define SafeDeletePointer(block,size) {if((size)>=0){if((block)&&!(IsBadReadPtr((block),(size)))) {GlobalFree((block));(block)=0;}}}


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "WorkspaceBar.h"
#include "WorkspaceBar2.h"
#include "OutputBar.h"
#include "PropertiesViewBar.h"
#include "MyRibbonEdit.h"

#include <vector>
#include<fstream>
using namespace std;

//#define CFrameWnd CBCGPFrameWnd

class ADDomain;

class CMainFrame : public CBCGPFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
    int m_flagCNDP;
	CString m_initDCName;
	CString m_initUsername;
	CString m_initPassword;
	CString m_initFilePathName;
	CString m_initDCName2;
	CString m_initUsername2;
	CString m_initPassword2;
	CString m_initRacerIP;

	BOOL m_bSelfStartRacer;

	CBCGPRibbonPanel *m_pPanelLogOn;
	CBCGPRibbonEdit* m_pEditDCName;
	CBCGPRibbonEdit* m_pEditUsername;
	CMyRibbonEdit* m_pEditPassword;
	CBCGPRibbonEdit* m_pEditRacerIP;
	BOOL m_bDesignMode;

	CBCGPRibbonCheckBox *m_pCheckBoxChinese;
	CBCGPRibbonCheckBox *m_pCheckBoxEnglish;
	BOOL m_bChineseOrEnglish;

	int m_iDomains;
	vector<CBCGPRibbonEdit*> m_npEditDCNames;
	vector<CBCGPRibbonEdit*> m_npEditUsernames;
	vector<CBCGPRibbonEdit*> m_npEditPasswords;

	CBCGPRibbonEdit* m_pEditFilePathName;

	CBCGPRibbonProgressBar* m_pProgress;
	int m_iProgress;
	BOOL m_bProgressing;
	CBCGPRibbonStatusBarPane* m_pStatusLabel;

	vector<ADDomain*> globalDomains;

	BOOL g_bReboot;

public:  // control bar embedded members
	CBCGPRibbonBar m_wndRibbonBar;
	CBCGPRibbonCategory *m_pCategory1;
	CBCGPRibbonCategory *m_pCategory2;
	CBCGPRibbonMainButton m_MainButton;
	CBCGPToolBarImages m_PanelImages;
	CBCGPRibbonStatusBar m_wndStatusBar;
	CWorkspaceBar m_wndWorkSpace;
	CWorkspaceBar2 m_wndWorkSpace2;
	COutputBar m_wndOutput;
	CPropertiesBar m_wndPropGrid;

	CString m_strInputDir;
	CString m_strPolicyDirectory;
	CString m_strExePath;
	
public:
	//DWORD FindProcessByName(CString& strFileName);
	//HWND GetProcessMainWnd(DWORD dwProcessId);
	HWND GetGlobalHandleByTitle(CString strTitle);

	BOOL FolderExist(CString strPath);
	void initInputDir();
	void initPolicyDir();
	void initEXEDir();
	void initCNDPFilePathName();
	BOOL getLanguage();
	void setLanguage(BOOL bChineseOrEnglish);

// Generated message map functions
public:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);
	afx_msg LRESULT OnRibbonCustomize (WPARAM wp, LPARAM lp);
	afx_msg void OnToolsOptions();
	afx_msg void OnViewWorkspace();
	afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnViewWorkspace2();
	afx_msg void OnUpdateViewWorkspace2(CCmdUI* pCmdUI);
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnViewPropGrid();
	afx_msg void OnUpdateViewPropGrid(CCmdUI* pCmdUI);

	afx_msg void OnEditDCName(){};
	afx_msg void OnEditUsername(){};
	afx_msg void OnEditPassword();
	afx_msg void OnEditCNDPFilePathName(){};
	afx_msg void OnEditRacerIP(){};
	afx_msg void OnBtnLogOn();
	afx_msg void OnBtnCNDPGenerate();
	afx_msg void OnBtnStartCNDPCD();
	afx_msg void OnBtnLoad();
	afx_msg void OnBtnAddDomain();
	afx_msg void OnBtnRemoveDomain();
	afx_msg void OnBtnStartRacer();
	afx_msg void OnBtnDetect();
	afx_msg void OnBtnClear();
	afx_msg void On_SocketEvent();

	afx_msg void OnBtnLogInfoSearch();

	afx_msg void OnToggleDesignMode();
	afx_msg void OnUpdateToggleDesignMode(CCmdUI* pCmdUI);
	afx_msg void OnToggleChinese();
	afx_msg void OnUpdateToggleChinese(CCmdUI* pCmdUI);
	afx_msg void OnToggleEnglish();
	afx_msg void OnUpdateToggleEnglish(CCmdUI* pCmdUI);

	afx_msg LRESULT On_MyMessageBox_Error(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On_Output_ConsoleList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On_Output_RacerProCache(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On_Output_ReportList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On_Property_AddKeyValue_Basic(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On_Property_AddKeyValue_Extend(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On_Property_AddKeyValue_Father(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On_Property_AddKeyValue_ChildGroup(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On_Property_AddKeyValue_ChildUser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On_Property_AddKeyValue_Access(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On_Property_AddKeyValue_Network(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On_Property_ClearKeyValues(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On_Property_UpdateKeyValues(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On_ProgressBar_Start(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On_ProgressBar_Stop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On_Canvas_Invalidate(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	CString i18n(CString strText);
	BOOL CreateRibbonBar ();
	void ShowOptions (int nPage);

	UINT	m_nAppLook;

public:
	void ShowLogInfoOutputBar(CString DNSname,CString cn);
	void TraceUserLogonInfo(CString cn, CString NetBIOSName,CString DNSname, CString UserName, CString PassWord);
	BOOL logOnByUserPassword(CString strDomainName, CString strUserName, CString strPassword);
	BOOL logOff();
};
class UserLogonInfo
{
	
	
public:
	CString User_name;
	CString Computer_name;
	CString WriteTime;
	CString LogonTime;
	CString LogofTime;
	CString LogonId;
	DWORD D_LogonTime;
	TCHAR LogonStyle;
	DWORD EventId;
	DWORD dwCounter;
protected:
private:
};
DWORD WINAPI LogOnThread(LPVOID lpParameter);
DWORD WINAPI LoadThread(LPVOID lpParameter);
DWORD WINAPI DetectThread(LPVOID lpParameter);
DWORD WINAPI LogOnSearchThread(LPVOID lpParameter);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__A2E3FE84_7646_4501_82BB_20CFFA80F87C__INCLUDED_)
