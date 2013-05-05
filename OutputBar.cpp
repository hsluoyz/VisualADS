// OutputBar.cpp : implementation of the COutputBar class
//

#include "stdafx.h"
#include "VisualADS.h"
#include "OutputBar.h"
#include "MainFrm.h"
#include "MyMessageBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// COutputBar

BEGIN_MESSAGE_MAP(COutputBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(COutputBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputBar construction/destruction

COutputBar::COutputBar()
{
	// TODO: add one-time construction code here
	m_bChineseOrEnglish = Global_GetLanguage();
}

COutputBar::~COutputBar()
{
}

CString COutputBar::itos(int i)
{
	CString strTemp;
	strTemp.Format(_T("%d"), i);
	return strTemp;
}

CString COutputBar::getTime()
{
	CTime time = CTime::GetCurrentTime();
    CString strTime = time.Format("%H:%M:%S");
	return strTime;
}

void COutputBar::initConsoleList()
{
	LONG lStyle;
	lStyle = GetWindowLong(m_listConsole.m_hWnd, GWL_STYLE); //获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_listConsole.m_hWnd, GWL_STYLE, lStyle); //设置style
	
	m_listConsole.SetExtendedStyle(m_listConsole.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	
	if (m_bChineseOrEnglish)
	{
		m_listConsole.InsertColumn(0, _T("时间序列"));
		m_listConsole.InsertColumn(1, _T("消息"));
		m_listConsole.SetColumnWidth(0, 100);
	}
	else
	{
		m_listConsole.InsertColumn(0, _T("Timeline"));
		m_listConsole.InsertColumn(1, _T("Message"));
		m_listConsole.SetColumnWidth(0, 100);
	}
}

void COutputBar::insert2ConsoleList(CString strNo, CString strContent)
{
	int iLimit = m_listConsole.GetScrollLimit(SB_VERT);
	int iPos = m_listConsole.GetScrollPos(SB_VERT);

	CString strHead = _T("[") + getTime() + _T("]-") + itos(m_listConsole.GetItemCount());
	m_listConsole.InsertItem(m_listConsole.GetItemCount(), strHead);
	m_listConsole.SetItemText(m_listConsole.GetItemCount() - 1, 1, strContent);

	if (iLimit == iPos)
	{
		m_listConsole.EnsureVisible(m_listConsole.GetItemCount() - 1, FALSE);
	}
}



void COutputBar::clearConsoleList()
{
	
	m_listConsole.DeleteAllItems();
}

void COutputBar::initReportList()
{
	LONG lStyle;
	lStyle = GetWindowLong(m_listReport.m_hWnd, GWL_STYLE); //获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_listReport.m_hWnd, GWL_STYLE, lStyle); //设置style

	m_listReport.SetExtendedStyle(m_listReport.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	if (m_bChineseOrEnglish)
	{
		m_listReport.InsertColumn(0, _T("序号"));
		m_listReport.InsertColumn(1, _T("策略冲突"));
		m_listReport.SetColumnWidth(0, 70);
	}
	else
	{
		m_listReport.InsertColumn(0, _T("No."));
		m_listReport.InsertColumn(1, _T("Policy Conflict"));
		m_listReport.SetColumnWidth(0, 70);
	}
}

void COutputBar::insert2ReportList(CString strNo, CString strContent)
{
	int iLimit = m_listReport.GetScrollLimit(SB_VERT);
	int iPos = m_listReport.GetScrollPos(SB_VERT);

	m_listReport.InsertItem(m_listReport.GetItemCount(), strNo);
	m_listReport.SetItemText(m_listReport.GetItemCount() - 1, 1, strContent);

	if (iLimit == iPos)
	{
		m_listReport.EnsureVisible(m_listReport.GetItemCount() - 1, FALSE);
	}
}

void COutputBar::clearReportList()
{
	m_listReport.DeleteAllItems();
}

void COutputBar::initCacheList()
{
	LONG lStyle;
	lStyle = GetWindowLong(m_listRacerCache.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_listRacerCache.m_hWnd, GWL_STYLE, lStyle);//设置style

	m_listRacerCache.SetExtendedStyle(m_listRacerCache.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	
	if (m_bChineseOrEnglish)
	{
		m_listRacerCache.InsertColumn(0, _T("序号"));
		m_listRacerCache.InsertColumn(1, _T("RacerPro缓存内容"));
		m_listRacerCache.SetColumnWidth(0, 70);
	}
	else
	{
		m_listRacerCache.InsertColumn(0, _T("No."));
		m_listRacerCache.InsertColumn(1, _T("RacerPro Cache"));
		m_listRacerCache.SetColumnWidth(0, 70);
	}
}

void COutputBar::insert2CacheList(CString strNo, CString strContent)
{
	int iLimit = m_listRacerCache.GetScrollLimit(SB_VERT);
	int iPos = m_listRacerCache.GetScrollPos(SB_VERT);
	
	m_listRacerCache.InsertItem(m_listRacerCache.GetItemCount(), strNo);
	m_listRacerCache.SetItemText(m_listRacerCache.GetItemCount() - 1, 1, strContent);

	if (iLimit == iPos)
	{
		m_listRacerCache.EnsureVisible(m_listRacerCache.GetItemCount() - 1, FALSE);
	}
}

void COutputBar::clearCacheList()
{
	m_listRacerCache.DeleteAllItems();
}

void COutputBar::initLogonInfo()
{
	LONG lStyle;
	lStyle = GetWindowLong(m_listLogonInfo.m_hWnd, GWL_STYLE); //获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_listLogonInfo.m_hWnd, GWL_STYLE, lStyle); //设置style
	
	m_listLogonInfo.SetExtendedStyle(m_listLogonInfo.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	
	m_listLogonInfo.InsertColumn(0, _T("计算机名"));
	m_listLogonInfo.InsertColumn(1, _T("登陆时间"));
	m_listLogonInfo.InsertColumn(2, _T("注销时间"));
	m_listLogonInfo.InsertColumn(3, _T("用户名"));
	m_listLogonInfo.InsertColumn(4, _T("登陆类型"));
	m_listLogonInfo.SetColumnWidth(0, 200);
	m_listLogonInfo.SetColumnWidth(1, 200);
	m_listLogonInfo.SetColumnWidth(2, 200);
	m_listLogonInfo.SetColumnWidth(3, 200);
	m_listLogonInfo.SetColumnWidth(4, 200);
	m_Logwnd=m_listLogonInfo.m_hWnd;



	
}

void COutputBar::insert2LogonInfo(CString cn, CString LogonTime, CString LogofTime, CString un,CString LogStyle)
{
	int listnum;
	listnum=m_listLogonInfo.GetItemCount();
	m_listLogonInfo.InsertItem(listnum,cn);
	m_listLogonInfo.SetItemText(listnum,1,LogonTime);
	m_listLogonInfo.SetItemText(listnum,2,LogofTime);
	m_listLogonInfo.SetItemText(listnum,3,un);
	m_listLogonInfo.SetItemText(listnum,4,LogStyle);
}
void COutputBar::clearLogonInfo()
{
	m_listLogonInfo.DeleteAllItems();
}
/////////////////////////////////////////////////////////////////////////////
// COutputBar message handlers

int COutputBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tabs window:
	if (!m_wndTabs.Create (CBCGPTabWnd::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}

	// Create list windows.
	// TODO: create your own window here:
	const DWORD dwViewStyle =	
		LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	
	if (!m_listReport.Create (dwViewStyle, rectDummy, &m_wndTabs, 2) ||
		!m_listRacerCache.Create (dwViewStyle, rectDummy, &m_wndTabs, 3) ||
//		!m_wndList3.Create (dwViewStyle, rectDummy, &m_wndTabs, 4) ||
		!m_listConsole.Create (dwViewStyle, rectDummy, &m_wndTabs, 5)||
		!m_listLogonInfo.Create (dwViewStyle, rectDummy, &m_wndTabs, 6))
	{
		TRACE0("Failed to create output view\n");
		return -1;      // fail to create
	}

	// Attach list windows to tab:
	if (m_bChineseOrEnglish)
	{
		m_wndTabs.AddTab (&m_listConsole, _T("控制台输出"), -1);
		m_wndTabs.AddTab (&m_listRacerCache, _T("RacerPro通信缓存"), -1);
		m_wndTabs.AddTab (&m_listReport, _T("策略冲突检测结果"), -1);
		m_wndTabs.AddTab (&m_listLogonInfo, _T("登陆信息查询"), -1);
	}
	else
	{
		m_wndTabs.AddTab (&m_listConsole, _T("Console"), -1);
		m_wndTabs.AddTab (&m_listRacerCache, _T("RacerPro TCP Cache"), -1);
		m_wndTabs.AddTab (&m_listReport, _T("Conflict Detection Result"), -1);
		m_wndTabs.AddTab (&m_listLogonInfo, _T("Logon Information Query"), -1);
	}

	//m_wndTabs.AddTab (&m_wndList2, _T("Output 2"), -1);
	//m_wndTabs.AddTab (&m_wndList3, _T("Output 3"), -1);

	initConsoleList();
	initCacheList();
	initReportList();
	initLogonInfo();

	return 0;
}

void COutputBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy,
		SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	m_listConsole.SetColumnWidth(1, cx - 100 - 6);
	m_listReport.SetColumnWidth(1, cx - 70 - 6);
	m_listRacerCache.SetColumnWidth(1, cx - 70 - 6);
//	m_listLogonInfo.SetColumnWidth(1, cx - 70 - 6);
}





