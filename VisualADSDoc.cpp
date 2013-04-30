// VisualADSDoc.cpp : implementation of the CVisualADSDoc class
//

#include "stdafx.h"
#include "VisualADS.h"

#include "VisualADSDoc.h"
#include "MainFrm.h"
#include "ADDomain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVisualADSDoc

IMPLEMENT_DYNCREATE(CVisualADSDoc, CFODoc)

BEGIN_MESSAGE_MAP(CVisualADSDoc, CFODoc)
	//{{AFX_MSG_MAP(CVisualADSDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisualADSDoc construction/destruction

CVisualADSDoc::CVisualADSDoc()
{
	// TODO: add one-time construction code here

}

CVisualADSDoc::~CVisualADSDoc()
{
}

BOOL CVisualADSDoc::OnNewDocument()
{
	if (!CFODoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	SetTitle(_T("Windows域安全卫士"));
	return TRUE;
}

void CVisualADSDoc::clearFilePathName()
{
	m_strPathName = _T("");
}

void CVisualADSDoc::MyFileClose()
{
	OnFileClose();
}

/////////////////////////////////////////////////////////////////////////////
// CVisualADSDoc serialization

void CVisualADSDoc::Serialize(CArchive& ar)
{
	CMainFrame *pMainFrm = (CMainFrame *) AfxGetMainWnd();
	
	if (ar.IsStoring())
	{
		int i;
		ar << pMainFrm->globalDomains.size();

		for (i = 0; i < pMainFrm->globalDomains.size(); i ++)
		{
			pMainFrm->globalDomains[i]->Serialize(ar);
		}
	}
	else
	{
		pMainFrm->OnBtnClear();

		int i;
		int iSize;
		ar >> iSize;

		for (i = 0; i < iSize; i ++)
		{
			ADDomain *pDomain = new ADDomain();
			pDomain->Serialize(ar);
			pMainFrm->globalDomains.push_back(pDomain);
		}

		pMainFrm->OnBtnLoad();
	}
}

void CVisualADSDoc::MyOnClose()
{
	// TODO: 在此添加命令处理程序代码
	if (!SaveModified())
		return;
	
	BOOL bAutoDelete = m_bAutoDelete;
	m_bAutoDelete = FALSE; // don't destroy document while closing views
	while (!m_viewList.IsEmpty())
	{
		// get view attached to the document
		CView* pView = (CView*)m_viewList.GetHead();
		ASSERT_VALID(pView);
		pView->DestroyWindow();
	}
	m_bAutoDelete = bAutoDelete;
	
	// clean up contents of document before destroying the document itself
	DeleteContents();
	
	// delete the document if necessary
	if (m_bAutoDelete)
		delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CVisualADSDoc diagnostics

#ifdef _DEBUG
void CVisualADSDoc::AssertValid() const
{
	CFODoc::AssertValid();
}

void CVisualADSDoc::Dump(CDumpContext& dc) const
{
	CFODoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVisualADSDoc commands

void CVisualADSDoc::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CFODoc::DeleteContents();
}
