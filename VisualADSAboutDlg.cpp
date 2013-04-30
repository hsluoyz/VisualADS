// VisualADSAboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VisualADS.h"
#include "VisualADSAboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVisualADSAboutDlg dialog


CVisualADSAboutDlg::CVisualADSAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVisualADSAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVisualADSAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVisualADSAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisualADSAboutDlg)
	DDX_Control(pDX, IDC_ABOUT_EDIT, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVisualADSAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CVisualADSAboutDlg)
	ON_BN_CLICKED(IDC_COMPANY_URL, OnCompanyUrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisualADSAboutDlg message handlers

BOOL CVisualADSAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	SetWindowText(_T("关于VisualADS"));
	CString strText;
	strText += _T("VisualADS v1.1.5.15389采用Visual C++ 6.0编译器结合BCGControlBar Pro 18.0、E-XD++ 15.80、Graphviz 2.2.8、RacerPro 1.9.2开发完成。\r\n");
	strText += _T("支持Windows版本：桌面版本包括Windows XP/Vista/7，服务器本版包括Windows Server 2003/2008/2008 R2，Windows 2000/NT4/9x等机器会被自动忽略。\r\n");
	strText += _T("使用过程中如果出现360关于HOSTS修改的提示，请允许。\r\n");
	strText += _T("\r\n");
	strText += _T("使用方法：\r\n");
	strText += _T("①选择一台与域在同一局域网的计算机安装本软件（可以不必是域内计算机），并打开主程序；\r\n");
	strText += _T("②在“域管理员登录”区域填入域控制器名称、域管理员账户密码后，点击“登录”按钮，等待域信息以图形方式显示，画布可以用鼠标拖动；\r\n");
	strText += _T("③点击“启动引擎”按钮，启动描述逻辑推理引擎；\r\n");
	strText += _T("④点击“冲突检测”按钮，等待一段时间，“策略冲突检测结果”区域将会显示域中存在的具体冲突及其详细说明；\r\n");
	strText += _T("⑤如果图形发生遮挡，或者排列过于紧密，请启用“设计模式”区域的“编辑图形元素”功能，这时可鼠标调整图形的位置和大小；\r\n");
	strText += _T("⑥在非编辑模式下单击用户、组、计算机等元素时，右侧属性栏将会显示其基本信息和扩展信息；\r\n");
	strText += _T("⑦如果想要进行下一次登录和检测，一定要先点击“清空缓存”按钮，把上一次的信息全部清除。\r\n");
	strText += _T("\r\n");
	strText += _T("技术支持请联系：veotax@sae.buaa.edu.cn\r\n");
	strText += _T("                                                                                                                  2012/10/31");
	m_edit.SetWindowText(strText);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVisualADSAboutDlg::OnCompanyUrl() 
{
	ShellExecute(NULL, _T("open"), _T("http://www.buaa.edu.cn"), NULL, NULL, SW_MAXIMIZE);
}
