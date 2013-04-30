// PropertiesViewBar.cpp : implementation of the CPropertiesBar class
//

#include "stdafx.h"
#include "VisualADS.h"
#include "PropertiesViewBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertiesBar class

CPropertiesBar::CPropertiesBar()
{
	m_BorderColor = CBCGPColor::Orange;
	m_FillBrush.SetColors(CBCGPColor::LightSteelBlue, CBCGPColor::White, CBCGPBrush::BCGP_GRADIENT_RADIAL_TOP_LEFT, 0.75);
	m_TextBrush.SetColor(CBCGPColor::SteelBlue);
}

CPropertiesBar::~CPropertiesBar()
{
}

BEGIN_MESSAGE_MAP(CPropertiesBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CPropertiesBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_SORTINGPROP, OnSortingprop)
	ON_UPDATE_COMMAND_UI(ID_SORTINGPROP, OnUpdateSortingprop)
	ON_COMMAND(ID_EXPAND, OnExpand)
	ON_UPDATE_COMMAND_UI(ID_EXPAND, OnUpdateExpand)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_COMMAND_CLICKED, OnCommandClicked)
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_MENU_ITEM_SELECTED, OnMenuItemSelected)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesBar::AdjustLayout ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect (rectClient);

	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width (), rectClient.Height (), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	if (!m_wndPropList.Create (WS_VISIBLE | WS_CHILD, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properies Grid \n");
		return -1;      // fail to create
	}

	InitPropList ();
	AdjustLayout ();

	//m_pGroupBasic->AllowEdit(FALSE);
	//m_pGroupExtension->AllowEdit(FALSE);

	m_pBasicInfo->Expand(TRUE);
	m_pExtensiveInfo->Expand(TRUE);
	m_pFatherInfo->Expand(TRUE);
	m_pChildGroupInfo->Expand(TRUE);
	m_pChildUserInfo->Expand(TRUE);
	m_pAccessInfo->Expand(TRUE);
	m_pNetworkInfo->Expand(TRUE);
	m_wndPropList.ExpandAll();

	return 0;
}

void CPropertiesBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout ();
}

void CPropertiesBar::OnSortingprop() 
{
	m_wndPropList.SetAlphabeticMode ();
}

void CPropertiesBar::OnUpdateSortingprop(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndPropList.IsAlphabeticMode ());
}

void CPropertiesBar::OnExpand() 
{
	m_wndPropList.SetAlphabeticMode (FALSE);
}

void CPropertiesBar::OnUpdateExpand(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (!m_wndPropList.IsAlphabeticMode ());
}

void CPropertiesBar::InitPropList ()
{
	// Add commands:
	CStringList lstCommands;
	lstCommands.AddTail (_T("Command 1..."));
	lstCommands.AddTail (_T("Command 2..."));

	m_wndPropList.SetCommands (lstCommands);

	// Add custom menu items:
	CStringList lstCustomMenuItem;
	lstCustomMenuItem.AddTail (_T("Custom menu item 1"));
	lstCustomMenuItem.AddTail (_T("Custom menu item 2"));
	lstCustomMenuItem.AddTail (_T("Custom menu item 3"));

	m_wndPropList.SetCustomMenuItems(lstCustomMenuItem);

	// Setup general look:
	m_wndPropList.EnableToolBar();
	m_wndPropList.EnableSearchBox();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDesciptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	m_wndPropList.EnableContextMenu();

	// Add properties:
	/*
	CBCGPProp* pGroup1 = new CBCGPProp (_T("Appearance"));

	pGroup1->AddSubItem (new CBCGPProp (_T("3D Look"), (_variant_t) false,
		_T("Specifies the dialog's font will be non-bold and controls will have a 3D border")));

	CBCGPProp* pProp = new CBCGPProp (_T("Border"), _T("Dialog Frame"),
		_T("One of: None, Thin, Resizable, or Dialog Frame"));
	pProp->AddOption (_T("None"));
	pProp->AddOption (_T("Thin"));
	pProp->AddOption (_T("Resizable"));
	pProp->AddOption (_T("Dialog Frame"));
	pProp->AllowEdit (FALSE);

	pGroup1->AddSubItem (pProp);
	pGroup1->AddSubItem (new CBCGPProp (_T("Caption"), (_variant_t) _T("About BCGProTest"),
		_T("Specifies the text that will be displayed in the dialog's title bar")));

	m_wndPropList.AddProperty (pGroup1);

	CBCGPProp* pSize = new CBCGPProp (_T("Window Size"), 0, TRUE);

	pProp = new CBCGPProp (_T("Height"), (_variant_t) 250l,
							_T("Specifies the dialog's height"));
	pProp->EnableSpinControl (TRUE, 0, 1000);
	pSize->AddSubItem (pProp);

	pProp = new CBCGPProp (	_T("Width"), (_variant_t) 150l,
							_T("Specifies the dialog's width"));
	pProp->EnableSpinControl (TRUE, 0, 1000);
	pSize->AddSubItem (pProp);

	m_wndPropList.AddProperty (pSize);

	CBCGPProp* pGroup2 = new CBCGPProp (_T("Font"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle ((HFONT) GetStockObject (DEFAULT_GUI_FONT));
	font->GetLogFont (&lf);

	lstrcpy (lf.lfFaceName, _T("Arial"));

	pGroup2->AddSubItem (new CBCGPFontProp (_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the dialog")));
	pGroup2->AddSubItem (new CBCGPProp (_T("Use System Font"), (_variant_t) true, _T("Specifies that the dialog uses MS Shell Dlg font")));

	m_wndPropList.AddProperty (pGroup2);

	CBCGPProp* pGroup3 = new CBCGPProp (_T("Brushes"));

	CBCGPColorProp* pColorProp = new CBCGPColorProp (_T("Border Color"), m_BorderColor, NULL, _T("Specifies the default dialog border color"));
	pColorProp->EnableOtherButton (_T("Other..."));
	pColorProp->EnableAutomaticButton (_T("Default"), globalData.clrBarShadow);
	pGroup3->AddSubItem (pColorProp);

	pGroup3->AddSubItem (new CBCGPBrushProp (_T("Background"), m_FillBrush, NULL,
		_T("Specifies the default background")));

	pGroup3->AddSubItem (new CBCGPBrushProp (_T("Foreground"), m_TextBrush, NULL,
		_T("Specifies the default foreground")));

	m_wndPropList.AddProperty (pGroup3);

	CBCGPProp* pGroup4 = new CBCGPProp (_T("Misc"));
	pProp = new CBCGPProp (_T("(Name)"), _T("IDD_ABOUT_BOX (dialog)"));
	pProp->Enable (FALSE);
	pGroup4->AddSubItem (pProp);

	static TCHAR BASED_CODE szFilter[] = _T("Icon Files (*.ico)|*.ico|All Files (*.*)|*.*||");
	pGroup4->AddSubItem (new CBCGPFileProp (_T("Icon"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Specifies the dialog icon")));

	pGroup4->AddSubItem (new CBCGPFileProp (_T("Folder"), _T("c:\\")));

	COleDateTime date = COleDateTime::GetCurrentTime ();
	pGroup4->AddSubItem (new CBCGPDateTimeProp (_T("Date"), date,
		_T("Set a date"), 0, CBCGPDateTimeCtrl::DTM_DATE));

	pGroup4->AddSubItem (new CBCGPDateTimeProp (_T("Time"), date,
		_T("Set a time"), 0, CBCGPDateTimeCtrl::DTM_TIME));

	m_wndPropList.AddProperty (pGroup4);

	CBCGPProp* pGroup5 = new CBCGPProp (_T("Hierarchy"));

	CBCGPProp* pGroup51 = new CBCGPProp (_T("First sub-level"));
	pGroup5->AddSubItem (pGroup51);

	CBCGPProp* pGroup511 = new CBCGPProp (_T("Second sub-level"));
	pGroup51->AddSubItem (pGroup511);

	pGroup511->AddSubItem (new CBCGPProp (_T("Item 1"), (_variant_t) _T("Value 1"),
		_T("This is a description")));
	pGroup511->AddSubItem (new CBCGPProp (_T("Item 2"), (_variant_t) _T("Value 2"),
		_T("This is a description")));
	pGroup511->AddSubItem (new CBCGPProp (_T("Item 3"), (_variant_t) _T("Value 3"),
		_T("This is a description")));

	pGroup5->Expand (FALSE);
	m_wndPropList.AddProperty (pGroup5);
	*/

	m_pBasicInfo = new CBCGPProp(_T("Active Directory基本信息"));
	m_wndPropList.AddProperty(m_pBasicInfo);
	m_pExtensiveInfo = new CBCGPProp(_T("Active Directory扩展信息"));
	m_wndPropList.AddProperty(m_pExtensiveInfo);
	m_pFatherInfo = new CBCGPProp(_T("所加入组信息"));
	m_wndPropList.AddProperty(m_pFatherInfo);
	m_pChildGroupInfo = new CBCGPProp(_T("成员组信息"));
	m_wndPropList.AddProperty(m_pChildGroupInfo);
	m_pChildUserInfo = new CBCGPProp(_T("成员用户信息"));
	m_wndPropList.AddProperty(m_pChildUserInfo);
	m_pAccessInfo = new CBCGPProp(_T("访问控制信息"));
	m_wndPropList.AddProperty(m_pAccessInfo);
	m_pNetworkInfo = new CBCGPProp(_T("网络信息"));
	m_wndPropList.AddProperty(m_pNetworkInfo);
	m_wndPropList.ExpandAll();
}

void CPropertiesBar::addBasicProperty(CString strKey, CString strValue)
{
	CBCGPProp* pProp = new CBCGPProp(strKey, _variant_t(strValue), _T(""));
	pProp->AllowEdit(FALSE);
	m_pBasicInfo->AddSubItem(pProp);
	m_npBasicInfoItems.push_back(pProp);
}

void CPropertiesBar::addExtensiveProperty(CString strKey, CString strValue)
{
	CBCGPProp* pProp = new CBCGPProp(strKey, _variant_t(strValue), _T(""));
	pProp->AllowEdit(FALSE);
	m_pExtensiveInfo->AddSubItem(pProp);
	m_npExtensiveInfoItems.push_back(pProp);
}

void CPropertiesBar::addFatherProperty(CString strKey, CString strValue)
{
	CBCGPProp* pProp = new CBCGPProp(strKey, _variant_t(strValue), _T(""));
	pProp->AllowEdit(FALSE);
	m_pFatherInfo->AddSubItem(pProp);
	m_npFatherInfoItems.push_back(pProp);
}

void CPropertiesBar::addChildGroupProperty(CString strKey, CString strValue)
{
	CBCGPProp* pProp = new CBCGPProp(strKey, _variant_t(strValue), _T(""));
	pProp->AllowEdit(FALSE);
	m_pChildGroupInfo->AddSubItem(pProp);
	m_npChildGroupInfoItems.push_back(pProp);
}

void CPropertiesBar::addChildUserProperty(CString strKey, CString strValue)
{
	CBCGPProp* pProp = new CBCGPProp(strKey, _variant_t(strValue), _T(""));
	pProp->AllowEdit(FALSE);
	m_pChildUserInfo->AddSubItem(pProp);
	m_npChildUserInfoItems.push_back(pProp);
}

void CPropertiesBar::addAccessProperty(CString strKey, CString strValue)
{
	CBCGPProp* pProp = new CBCGPProp(strKey, _variant_t(strValue), _T(""));
	pProp->AllowEdit(FALSE);
	m_pAccessInfo->AddSubItem(pProp);
	m_npAccessInfoItems.push_back(pProp);
}

void CPropertiesBar::addNetworkProperty(CString strKey, CString strValue)
{
	CBCGPProp* pProp = new CBCGPProp(strKey, _variant_t(strValue), _T(""));
	pProp->AllowEdit(FALSE);
	m_pNetworkInfo->AddSubItem(pProp);
	m_npNetworkInfoItems.push_back(pProp);
}

void CPropertiesBar::clearProperties()
{
	for (int i = 0; i < m_npBasicInfoItems.size(); i ++)
	{
		m_pBasicInfo->RemoveSubItem(m_npBasicInfoItems[i]);
	}
	m_npBasicInfoItems.clear();

	for (i = 0; i < m_npExtensiveInfoItems.size(); i ++)
	{
		m_pExtensiveInfo->RemoveSubItem(m_npExtensiveInfoItems[i]);
	}
	m_npExtensiveInfoItems.clear();

	for (i = 0; i < m_npFatherInfoItems.size(); i ++)
	{
		m_pFatherInfo->RemoveSubItem(m_npFatherInfoItems[i]);
	}
	m_npFatherInfoItems.clear();

	for (i = 0; i < m_npChildGroupInfoItems.size(); i ++)
	{
		m_pChildGroupInfo->RemoveSubItem(m_npChildGroupInfoItems[i]);
	}
	m_npChildGroupInfoItems.clear();

	for (i = 0; i < m_npChildUserInfoItems.size(); i ++)
	{
		m_pChildUserInfo->RemoveSubItem(m_npChildUserInfoItems[i]);
	}
	m_npChildUserInfoItems.clear();

	for (i = 0; i < m_npAccessInfoItems.size(); i ++)
	{
		m_pAccessInfo->RemoveSubItem(m_npAccessInfoItems[i]);
	}
	m_npAccessInfoItems.clear();

	for (i = 0; i < m_npNetworkInfoItems.size(); i ++)
	{
		m_pNetworkInfo->RemoveSubItem(m_npNetworkInfoItems[i]);
	}
	m_npNetworkInfoItems.clear();

	/*
	m_wndPropList.RemoveAll();

	m_pGroupBasic = new CBCGPProp(_T("基本信息"));
	m_pGroupBasic->Expand();
	m_wndPropList.AddProperty(m_pGroupBasic);

	m_pGroupExtension = new CBCGPProp(_T("扩展信息"));
	m_pGroupExtension->Expand();
	m_wndPropList.AddProperty(m_pGroupExtension);
	*/
}

CString aa;

void CPropertiesBar::updateProperties()
{
	for (int i = 0; i < m_npBasicInfoItems.size(); i ++)
	{
		//m_npGroupBasicChilds[i]->Show(FALSE);
	}
	for (i = 0; i < m_npBasicInfoItems.size(); i ++)
	{
		//m_npGroupBasicChilds[i]->Show(TRUE);
	}
	m_wndPropList.AdjustLayout();
	return;
	/*
	aa += "a";
	m_npBasicInfoItems[0]->SetValue(aa.GetBuffer(0));
	m_npBasicInfoItems[0]->CommitModifiedValue();
	m_npBasicInfoItems[0]->Redraw();
	m_npBasicInfoItems[0]->DoCopy();
	m_npBasicInfoItems[1]->DoPaste();
	m_npBasicInfoItems[0]->Enable(FALSE);
	m_npBasicInfoItems[0]->Enable();
	m_pBasicInfo->SerializeFromBuffer(aa, TRUE);
	m_pBasicInfo->CommitModifiedValue();
	m_pBasicInfo->Redraw();
	m_pBasicInfo->RedrawButton();
	m_pBasicInfo->RedrawMenuButton();
	
	for (i = 0; i < m_npExtensiveInfoItems.size(); i ++)
	{
		m_npExtensiveInfoItems[i]->Redraw();
	}
	return;
	//m_pGroupBasic->Redraw();
	m_pBasicInfo->Expand(FALSE);
	m_pBasicInfo->Expand(TRUE);

	//m_pGroupExtension->Redraw();
	m_pBasicInfo->Expand(TRUE);
	m_pExtensiveInfo->Expand(TRUE);
	*/
}

void CPropertiesBar::OnSetFocus(CWnd* pOldWnd) 
{
	CBCGPDockingControlBar::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus ();
}

LRESULT CPropertiesBar::OnCommandClicked(WPARAM, LPARAM /*lp*/)
{
//	int nCommandIndex = (int)lp;

	// TODO: Add your command handler code here
	
	return 0;
}

LRESULT CPropertiesBar::OnMenuItemSelected(WPARAM /*wp*/, LPARAM /*lp*/)
{
//	int nMenuIndex = (int)wp;

//	CBCGPProp* pProp = (CBCGPProp*)lp;
//	ASSERT_VALID(pProp);

	// TODO: Add your menu item processing code here

	return 0;
}
