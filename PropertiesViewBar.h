// PropertiesViewBar.h : interface of the CPropertiesBar class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTIESVIEWBAR_H__27FC5151_A9AD_47B9_BA14_955EA72C312B__INCLUDED_)
#define AFX_PROPERTIESVIEWBAR_H__27FC5151_A9AD_47B9_BA14_955EA72C312B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

class CPropertiesBar : public CBCGPDockingControlBar  
{
// Construction
public:
	CPropertiesBar();

	void AdjustLayout ();

// Attributes
public:
	BOOL bChineseOrEnglish;
	CBCGPProp* m_pBasicInfo;
	CBCGPProp* m_pExtensiveInfo;
	CBCGPProp* m_pFatherInfo;
	CBCGPProp* m_pChildGroupInfo;
	CBCGPProp* m_pChildUserInfo;
	CBCGPProp* m_pAccessInfo;
	CBCGPProp* m_pNetworkInfo;

	vector<CBCGPProp*> m_npBasicInfoItems;
	vector<CBCGPProp*> m_npExtensiveInfoItems;
	vector<CBCGPProp*> m_npFatherInfoItems;
	vector<CBCGPProp*> m_npChildGroupInfoItems;
	vector<CBCGPProp*> m_npChildUserInfoItems;
	vector<CBCGPProp*> m_npAccessInfoItems;
	vector<CBCGPProp*> m_npNetworkInfoItems;

public:
	void addBasicProperty(CString strKey, CString strValue);
	void addExtensiveProperty(CString strKey, CString strValue);
	void addFatherProperty(CString strKey, CString strValue);
	void addChildGroupProperty(CString strKey, CString strValue);
	void addChildUserProperty(CString strKey, CString strValue);
	void addAccessProperty(CString strKey, CString strValue);
	void addNetworkProperty(CString strKey, CString strValue);
	void clearProperties();
	void updateProperties();

public:
	void SetVSDotNetLook (BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook (bSet);
		m_wndPropList.SetGroupNameFullWidth (bSet);
	}

protected:
	CBCGPPropList		m_wndPropList;

	CBCGPColor			m_BorderColor;
	CBCGPBrush			m_FillBrush;
	CBCGPBrush			m_TextBrush;

// Attributes
protected:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertiesBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropertiesBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropertiesBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSortingprop();
	afx_msg void OnUpdateSortingprop(CCmdUI* pCmdUI);
	afx_msg void OnExpand();
	afx_msg void OnUpdateExpand(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg LRESULT OnCommandClicked(WPARAM, LPARAM);
	afx_msg LRESULT OnMenuItemSelected(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	void InitPropList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIESVIEWBAR_H__27FC5151_A9AD_47B9_BA14_955EA72C312B__INCLUDED_)
