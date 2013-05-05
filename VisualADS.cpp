// VisualADS.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "VisualADS.h"

#include "MainFrm.h"
#include "VisualADSDoc.h"
#include "VisualADSView.h"
#include "VisualADSAboutDlg.h"
#include "SplashWindow.h"

#include "FOHyperLink.h"
#include "FOPGDIPlusProxy.h"

#include <WinSock2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WSA_VERSION  MAKEWORD(2,0)

/////////////////////////////////////////////////////////////////////////////
// CVisualADSApp

BEGIN_MESSAGE_MAP(CVisualADSApp, CWinApp)
	//{{AFX_MSG_MAP(CVisualADSApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisualADSApp construction

CVisualADSApp::CVisualADSApp() :
	CBCGPWorkspace (TRUE /* m_bResourceSmartUpdate */)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVisualADSApp object

CVisualADSApp theApp;

BOOL CVisualADSApp::FolderExist(CString strPath)
{
	WIN32_FIND_DATA wfd;
	BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile(strPath, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = TRUE;
	}
	FindClose(hFind);
	return rValue;
}

void CVisualADSApp::initInputDir()
{
	CString cstrDir;
	TCHAR cBuffer[260];
	GetCurrentDirectory(MAX_PATH, cBuffer);
	CString cstrPath(cBuffer);
	if (FolderExist(cstrPath + _T("\\input")))
	{
		cstrDir = cstrPath;
	}
	else
	{
		int nCount = cstrPath.ReverseFind(_T('\\'));
		cstrPath.Delete(nCount, cstrPath.GetLength() - nCount);
		
		if (FolderExist(cstrPath + _T("\\input")))
		{
			cstrDir = cstrPath;
		}
		else
		{
			MyMessageBox_Error(_T("initInputDir Error"), _T("Error"));
			return;
		}
	}
	cstrDir += _T("\\input\\");
	m_strInputDir = cstrDir;
}

BOOL CVisualADSApp::getLanguage()
{
	CStdioFile pFile;
	if (!pFile.Open(m_strInputDir + _T("\\Language.ini"), CFile::modeRead | CFile::typeText))
	{
		return TRUE;
	}
	
	CString strText;
	BOOL bChineseOrEnglish;
	pFile.ReadString(strText);
	if (strText == _T("1"))
	{
		bChineseOrEnglish = TRUE;
	}
	else
	{
		bChineseOrEnglish = FALSE;
	}
	
	pFile.Close();
	return bChineseOrEnglish;
}

/////////////////////////////////////////////////////////////////////////////
// CVisualADSApp initialization

BOOL CVisualADSApp::InitInstance()
{
	initInputDir();
	WSADATA WSAData = {0};
	if (0 != WSAStartup(WSA_VERSION, &WSAData))
	{
		// Tell the user that we could not find a usable
		// WinSock DLL.
		if (LOBYTE(WSAData.wVersion) != LOBYTE(WSA_VERSION) || HIBYTE(WSAData.wVersion) != HIBYTE(WSA_VERSION))
		{
			::MessageBox(NULL, _T("Incorrect version of WS2_32.dll found"), _T("Error"), MB_OK);
		}
		
		WSACleanup();
		return FALSE;
	}

	CSplashWindow::ShowSplashScreen( NULL, "Starting VisualADS...", 3000);

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	FOInit(RUNTIME_CLASS(CFOPGDIPlusProxy));
	gfxData.InitGUIType(GUI_BLUE);
	FOEnableXPStyle(TRUE);
	FOEnableContinueDraw(TRUE);
	FOEnableAroundCenterLink(TRUE);

	globalData.SetDPIAware ();

	AfxEnableControlContainer();
	

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("BCGP AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	SetRegistryBase (_T("Settings"));

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	InitContextMenuManager();
	InitKeyboardManager();

	// TODO: Remove this if you don't want extended tooltips:
	InitTooltipManager();

	CBCGPToolTipParams params;
	params.m_bVislManagerTheme = TRUE;

	theApp.GetTooltipManager ()->SetTooltipParams (
		BCGP_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS (CBCGPToolTipCtrl),
		&params);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CVisualADSDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CVisualADSView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CVisualADSApp message handlers

int CVisualADSApp::ExitInstance() 
{
	BCGCBProCleanUp();
	WSACleanup();
	return CWinApp::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CBCGPURLLinkButton	m_btnURL;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_COMPANY_URL, m_btnURL);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CVisualADSApp::OnAppAbout()
{
	CVisualADSAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CVisualADSApp::PreLoadState ()
{

	GetContextMenuManager()->AddMenu (_T("My menu"), IDR_CONTEXT_MENU);

	// TODO: add another context menus here
}

void CVisualADSApp::showAboutDlg()
{
	CVisualADSAboutDlg aboutDlg;
	aboutDlg.DoModal();
	//CVisualADSAboutDlg *pAboutDlg = new CVisualADSAboutDlg();
	//pAboutDlg->Create(IDD_ABOUTBOX, NULL);
	//pAboutDlg->ShowWindow(SW_SHOW);
}
/////////////////////////////////////////////////////////////////////////////
// CVisualADSApp message handlers


BOOL CVisualADSApp::PreTranslateMessage(MSG* pMsg) 
{
	if (CSplashWindow::PreTranslateAppMessage(pMsg))
		return TRUE;
	
	return CWinApp::PreTranslateMessage(pMsg);
}

CDocument* CVisualADSApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	return CWinApp::OpenDocumentFile(lpszFileName);

	CVisualADSDoc *pDoc = (CVisualADSDoc *) ((CMainFrame *)m_pMainWnd)->GetActiveDocument();
	// prompt the user (with all document templates)
	CString newName;
	if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
		return pDoc; // open cancelled
	
	// Process newName string if necessary
	
	// Get pTemplate, a pointer to one of the app's document templates
	
	pDoc->OnOpenDocument(newName);
	return pDoc;
	pDoc->OnCloseDocument();
}
