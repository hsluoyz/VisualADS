// VisualADSView.cpp : implementation of the CVisualADSView class
//

#include "stdafx.h"
#include "VisualADS.h"
#include "MainFrm.h"

#include <fo.H>

#include "VisualADSDoc.h"
#include "VisualADSView.h"
#include "ADObject.h"

ADObject *abstractADObject = new ADObject();

#include "DirUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CLOCK_0 3
#define CLOCK_1 5
#define CLOCK_2 6
#define CLOCK_3 2
#define CLOCK_4 7
#define CLOCK_5 8
#define CLOCK_6 0
#define CLOCK_7 9
#define CLOCK_8 10
#define CLOCK_9 1
#define CLOCK_10 11
#define CLOCK_11 4

/////////////////////////////////////////////////////////////////////////////
// CVisualADSView

IMPLEMENT_DYNCREATE(CVisualADSView, CFODrawView)

BEGIN_MESSAGE_MAP(CVisualADSView, CFODrawView)
	//{{AFX_MSG_MAP(CVisualADSView)
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFODrawView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFODrawView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisualADSView construction/destruction

CVisualADSView::CVisualADSView()
{
	m_bEnableDragCanvas = TRUE;
	m_frameShape = NULL;
	m_bCenter = FALSE;

	m_ptGroupSize.x = 120;
	m_ptGroupSize.y = 46;

    m_ptUserSize.x = 100;
	m_ptUserSize.y = 45;

	m_ptComputerSize.x = 120;
	m_ptComputerSize.y = 45;

	m_ptResourceSize.x = 100;
	m_ptResourceSize.y = 45;

	m_ptGroupFrameSize.x = 130;
	m_ptGroupFrameSize.y = 50;

	m_ptUserFrameSize.x = 110;
	m_ptUserFrameSize.y = 50;

	m_ptComputerFrameSize.x = 130;
	m_ptComputerFrameSize.y = 50;

	m_ptResourceFrameSize.x = 110;
	m_ptResourceFrameSize.y = 50;

	//EnableShowSelectionAlways(TRUE);
}

void CVisualADSView::My_Serialize()
{
	CVisualADSView();
	deleteAllShapes();
	deleteAllLinks();

	//Intialization needed: m_shapes --> addXX, 
	//Intialization needed: m_links
}

CVisualADSView::~CVisualADSView()
{
	//deleteAllShapes();
	//deleteAllLinks();
}

BOOL CVisualADSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFODrawView::PreCreateWindow(cs);
}

CSize CVisualADSView::GetCanvasOrginOffset() const
{
	return CSize(0,0);
}

void CVisualADSView::DoChangeModel(CFODataModel * pModel)
{
	CFODrawView::DoChangeModel(pModel);
	// Adding your data model init codes here..
	CSize szCanvas = CSize(3000, 8000);
	pModel->SetCanvasSize(szCanvas);
	pModel->SetBorderShow(FALSE);
	GetCurrentModel()->SetPageBkColor(RGB(255, 255, 255));
	pModel->ShowGrid(FALSE);
	pModel->SetDesignMode(FALSE);
	pModel->SetHlpLineVisible(FALSE);
	ResetToSelectMode();
	EnableMirrorFreeMode(FALSE);
	EnableShearFreeMode(FALSE);
	//EnableScrollBar(SB_BOTH,ESB_DISABLE_BOTH);
	CSize sizeTotal;
	sizeTotal.cx = 1;
	sizeTotal.cy = 1;
	SetScrollSizes(MM_TEXT, sizeTotal);
	SetScrollRange(0, 0, 0);
	FOSetCursor(IDC_FO_CURSOR_RADIO2);

	//GetCurrentModel()->set
}

void CVisualADSView::OnInitialUpdate() 
{
	CVisualADSDoc* pDoc = GetDocument();
	SetCurrentModel(pDoc->m_pDataModel);
	//GetCurrentModel()->DisplayPreviewBmp()
	LockUpdate(TRUE);
	
	
	/*
	for (int i = 0; i < 20; i ++)
	{
		addGroup(_T("Group组"), 100 + 50 * i, 100);
		addUser(_T("chaoge"), 200, 200 + 50 * i);
	}
	addComputer(_T("VEOTAX"), 150, 500);
	addResource(_T("NRDC1001"), 400, 100);
	addInheritRelation(_T("PermitRWX"), m_shapes[0], m_shapes[1]);

	FOPRect rect;
	rect.left = 100;
	rect.top = 100;
	rect.right = 500;
	rect.bottom = 500;

	addDomain(_T("domaintest.net"), 500, 500, 700, 700);
	*/

	CFODrawView::OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CVisualADSView drawing

void CVisualADSView::OnDraw(CDC* pDC)
{
	CVisualADSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	Draw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CVisualADSView printing

void CVisualADSView::OnFilePrintPreview() 
{
	BCGPPrintPreview(this);
}

BOOL CVisualADSView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return CFODrawView::OnPreparePrinting(pInfo);
}

void CVisualADSView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
	CFODrawView::OnBeginPrinting(pDC, pInfo);
}

void CVisualADSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CVisualADSView diagnostics

#ifdef _DEBUG
void CVisualADSView::AssertValid() const
{
	CFODrawView::AssertValid();
}

void CVisualADSView::Dump(CDumpContext& dc) const
{
	CFODrawView::Dump(dc);
}

CVisualADSDoc* CVisualADSView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVisualADSDoc)));
	return (CVisualADSDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVisualADSView message handlers

void CVisualADSView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CVisualADSView::FOSetCursor(UINT nID)
{
	//LoadStandardCursor(MAKEINTRESOURCE(32649));
	//LoadStandardCursor(IDC_WAIT);
	//FOSetSystemCursor();
	//if (nID == IDC_FO_AUTO_PAN_START || nID == IDC_FO_AUTO_PANING || nID == IDC_FO_CURSOR_RADIO2)
	//CFODrawView::FOSetCursor(nID);
	return;
}

void CVisualADSView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bEnableDragCanvas)
		CFODrawView::OnMouseMove(nFlags, point);
}

void CVisualADSView::DoEventSingleClick(CFODrawShape *pShape)
{
	//点击每个shape的时候调用
	if(pShape != NULL)
	{
		if (pShape->GetObjectCaption() == "group" /*|| pShape->GetObjectCaption() == "user"*/) //判断shape的文字
		{
			CPoint pt = pShape->GetCenterPoint();
			addGroupFrame(pt.x, pt.y); //给shape加上合适的虚线框
		}
		else if(pShape->GetObjectCaption() == "user"){
            CPoint pt = pShape->GetCenterPoint();
			addUserFrame(pt.x, pt.y);
		}
		else if (pShape->GetObjectCaption() == "computer")
		{
			CPoint pt = pShape->GetCenterPoint();
			addComputerFrame(pt.x, pt.y);
		}
		else if (pShape->GetObjectCaption() == "resource")
		{
			CPoint pt = pShape->GetCenterPoint();
			addResourceFrame(pt.x, pt.y);
		}
		else
		{
			if (m_frameShape != NULL)
			{
				GetCurrentModel()->RemoveShape(m_frameShape);
				//delete m_frameShape;
				m_frameShape = NULL;
				FOPInvalidate();
			}
		}
	}
	else
	{
		if (m_frameShape != NULL)
		{
			GetCurrentModel()->RemoveShape(m_frameShape); //什么都没有选中则去掉虚线框
			//delete m_frameShape;
			m_frameShape = NULL;
			FOPInvalidate();
		}
	}
}

void CVisualADSView::removeFrameShape()
{
	//删除一个shape
	if (m_frameShape != NULL)
	{
		GetCurrentModel()->RemoveShape(m_frameShape);
		//delete m_frameShape;
		m_frameShape = NULL;
	}
}

CMyShape* CVisualADSView::addDomain(ADObject *aDObject, CString strName, int iX1, int iY1, int iX2, int iY2)
{
	//生成一个域的shape
	CRect rctPosition = CRect(iX1, iY1, iX2, iY2);
	CRect rctTitlePos = CRect(0, (iY2-iY1)-25, (iX2 - iX1), iY2-iY1);  //tech.adtest.net
	CMyShape *pNode = new CMyShape(aDObject, this, FALSE);
	pNode->AddRef();
	pNode->Create(IDR_COMPSRES_DOMAIN, rctPosition, _T("domain"));
	pNode->AddText(_T(""), strName, rctTitlePos);
	pNode->AllowLabelSelect(FALSE);
	m_shapes.push_back(pNode);
	
	CFODataModel *pModel = GetCurrentModel();
	pModel->InsertShape(pNode);
	pNode->UpdateComp();
	pNode->Release();
	return pNode;
}

CMyShape* CVisualADSView::addOU(ADObject *aDObject, CString strName, int iX1, int iY1, int iX2, int iY2)
{
	//生成一个组织单位的shape
	CRect rctPosition = CRect(iX1, iY1, iX2, iY2);
	CRect rctTitlePos = CRect(0, (iY2-iY1)-25, iX2 - iX1, iY2-iY1); 
	CMyShape *pNode = new CMyShape(aDObject, this, FALSE);
	pNode->AddRef();
	pNode->Create(IDR_COMPSRES_OU, rctPosition, _T("ou"));
	pNode->AddText(_T(""), strName, rctTitlePos);
	pNode->AllowLabelSelect(FALSE);
	m_shapes.push_back(pNode);
	
	GetCurrentModel()->InsertShape(pNode);
	pNode->UpdateComp();
	pNode->Release();
	return pNode;
}

CMyShape* CVisualADSView::addBigComputer(ADObject *aDObject, CString strName, int iX1, int iY1, int iX2, int iY2)
{
	//生成一个带共享文件夹的计算机的shape
	CRect rctPosition = CRect(iX1, iY1, iX2, iY2);
	CRect rctTitlePos = CRect(0, (iY2-iY1)-25, iX2 - iX1, iY2-iY1);
	CMyShape *pNode = new CMyShape(aDObject, this, FALSE);
	pNode->AddRef();
	pNode->Create(IDR_COMPSRES_BIG_COMPUTER, rctPosition, _T("big_computer"));
	pNode->AddText(_T(""), strName, rctTitlePos);
	pNode->AllowLabelSelect(FALSE);
	m_shapes.push_back(pNode);
	
	GetCurrentModel()->InsertShape(pNode);
	pNode->UpdateComp();
	pNode->Release();
	return pNode;
}


CMyShape* CVisualADSView::addGroup(ADObject *aDObject, CString strName, int iX, int iY)
{
	//生成一个组的shape
	CRect rctPosition = CRect(iX - m_ptGroupSize.x/2, iY - m_ptGroupSize.y/2, iX + m_ptGroupSize.x/2, iY + m_ptGroupSize.y/2);
	CRect rctTitlePos = CRect(0, 0, m_ptGroupSize.x, m_ptGroupSize.y);
	CMyShape *pNode = new CMyShape(aDObject, this, TRUE);
	pNode->AddRef();
	pNode->Create(IDR_COMPSRES_GROUP, rctPosition, _T("group"));
	pNode->AddText(_T(""), strName, rctTitlePos);
	for (int i = 0; i < 12; i ++)
	{
		pNode->GetPortAt(i)->SetPortType(FO_PORT_NOTSHOW);
	}
	//pNode->CreateDefaultPort(0, 0.5)->SetPortType(FO_PORT_NOTSHOW);
	//pNode->CreateDefaultPort(1, 0.5)->SetPortType(FO_PORT_NOTSHOW);
	//pNode->CreateDefaultPort(0.5, 0)->SetPortType(FO_PORT_NOTSHOW);
	//pNode->CreateDefaultPort(0.5, 1)->SetPortType(FO_PORT_NOTSHOW);
	pNode->AllowLabelSelect(FALSE);
	m_shapes.push_back(pNode);

	GetCurrentModel()->InsertShape(pNode);
	pNode->UpdateComp();
	pNode->Release();
	return pNode;
}

CMyShape* CVisualADSView::addUser(ADObject *aDObject, CString strName, int iX, int iY)
{
	//生成一个用户的shape
	CRect rctPosition = CRect(iX - m_ptUserSize.x/2, iY - m_ptUserSize.y/2, iX + m_ptUserSize.x/2, iY + m_ptUserSize.y/2);
	CRect rctTitlePos = CRect(0, 0, m_ptUserSize.x, m_ptUserSize.y);
	CMyShape *pNode = new CMyShape(aDObject, this, TRUE);
	pNode->AddRef();
	pNode->Create(IDR_COMPSRES_USER, rctPosition, _T("user"));
	pNode->AddText(_T(""), strName, rctTitlePos);
	for (int i = 0; i < 12; i ++)
	{
		pNode->GetPortAt(i)->SetPortType(FO_PORT_NOTSHOW);
	}
	pNode->AllowLabelSelect(FALSE);
	m_shapes.push_back(pNode);
	
	GetCurrentModel()->InsertShape(pNode);
	pNode->UpdateComp();
	pNode->Release();
	return pNode;
}

CMyShape* CVisualADSView::addComputer(ADObject *aDObject, CString strName, int iX, int iY)
{
	//生成一个不带共享文件夹的计算机的shape
	CRect rctPosition = CRect(iX - m_ptComputerSize.x/2, iY - m_ptComputerSize.y/2, iX + m_ptComputerSize.x/2, iY + m_ptComputerSize.y/2);
	CRect rctTitlePos = CRect(0, 0, m_ptComputerSize.x, m_ptComputerSize.y);
	CMyShape *pNode = new CMyShape(aDObject, this, TRUE);
	pNode->AddRef();
	pNode->Create(IDR_COMPSRES_COMPUTER, rctPosition, _T("computer"));
	pNode->AddText(_T(""), strName, rctTitlePos);
	for (int i = 0; i < 12; i ++)
	{
		pNode->GetPortAt(i)->SetPortType(FO_PORT_NOTSHOW);
	}
	pNode->AllowLabelSelect(FALSE);
	m_shapes.push_back(pNode);
	
	GetCurrentModel()->InsertShape(pNode);
	pNode->UpdateComp();
	pNode->Release();
	return pNode;
}

CMyShape* CVisualADSView::addResource(ADObject *aDObject, CString strName, int iX, int iY)
{
	//生成一个共享文件夹的shape
	CRect rctPosition = CRect(iX - m_ptResourceSize.x/2, iY - m_ptResourceSize.y/2, iX + m_ptResourceSize.x/2, iY + m_ptResourceSize.y/2);
	CRect rctTitlePos = CRect(0, 0, m_ptResourceSize.x, m_ptResourceSize.y);
	CMyShape *pNode = new CMyShape(aDObject, this, TRUE);
	pNode->AddRef();
	pNode->Create(IDR_COMPSRES_RESOURCE, rctPosition, _T("resource"));
	for (int i = 0; i < 12; i ++)
	{
		pNode->GetPortAt(i)->SetPortType(FO_PORT_NOTSHOW);
	}
	pNode->AddText(_T(""), strName, rctTitlePos);
	pNode->AllowLabelSelect(FALSE);
	m_shapes.push_back(pNode);
	
	GetCurrentModel()->InsertShape(pNode);
	pNode->UpdateComp();
	pNode->Release();
	return pNode;
}

CFOLinkShape* CVisualADSView::addInheritRelation(CString strName, CFODrawPortsShape *pShape1, CFODrawPortsShape *pShape2)
{
	//生成组成员关系的linkshape
	CFOPortShape *pStart;
	CFOPortShape *pEnd;
	if (pShape1->GetTopLeft().x < pShape2->GetTopLeft().x)
	{
		pStart = pShape1->GetPortAt(CLOCK_3);
		pEnd = pShape2->GetPortAt(CLOCK_9);
	}
	else if (pShape1->GetTopLeft().x > pShape2->GetTopLeft().x)
	{
		pStart = pShape1->GetPortAt(CLOCK_9);
		pEnd = pShape2->GetPortAt(CLOCK_3);
	}
	else if (pShape1->GetTopLeft().y > pShape2->GetTopLeft().y)
	{
		pStart = pShape1->GetPortAt(CLOCK_0);
		pEnd = pShape2->GetPortAt(CLOCK_6);
	}
	else if (pShape1->GetTopLeft().y < pShape2->GetTopLeft().y)
	{
		pStart = pShape1->GetPortAt(CLOCK_6);
		pEnd = pShape2->GetPortAt(CLOCK_0);
	}
	else
	{
		MyMessageBox_Error(_T("addInheritRelation Error."), _T("Error"));
		return NULL;
	}

	CFOLinkShape *pLinkComp = new CFOLinkShape();
	pLinkComp->AddRef();
	CPoint ptPoints[2];
	ptPoints[0] = pStart->GetLinkPoint();
	ptPoints[1] = pEnd->GetLinkPoint();
	pLinkComp->Create(ptPoints, 2);
	pLinkComp->SetObjectCaption(strName);
	
	pLinkComp->SetFromPort(pStart);
	pLinkComp->SetToPort(pEnd);
	
	pLinkComp->SetLineWidth(2);
	pLinkComp->SetShadow(FALSE);
	pLinkComp->SetLineColor(RGB(0, 0, 51));
	pLinkComp->SetModel(GetCurrentModel());
	
	pStart->AddLinkShape(pLinkComp);
	pEnd->AddLinkShape(pLinkComp);
	
	// Set arrow type.
	int nType = 5;
	pLinkComp->SetEndArrowType(nType);
	pLinkComp->DoEndPropChange();
	GetCurrentModel()->AddChildAtTail(pLinkComp);
	pLinkComp->UpdateComp();
	pLinkComp->RelayoutPoints();
	//pLinkComp->Release();
	m_links.push_back(pLinkComp);
	return pLinkComp;
}

CFOLinkShape* CVisualADSView::addNTFSRelation(CString strName, CFODrawPortsShape *pShape1, CFODrawPortsShape *pShape2, int iPermission,
											  BOOL bBothRelation)
{
	//生成ntfs权限关系的linkshape
	CFOPortShape *pStart;
	CFOPortShape *pEnd;
	if (bBothRelation == TRUE)
	{
		if (pShape1->GetTopLeft().x < pShape2->GetTopLeft().x)
		{
			pStart = pShape1->GetPortAt(CLOCK_2);
			pEnd = pShape2->GetPortAt(CLOCK_9/*CLOCK_10*/);
		}
		else if (pShape1->GetTopLeft().x > pShape2->GetTopLeft().x)
		{
			pStart = pShape1->GetPortAt(CLOCK_9/*CLOCK_10*/);
			pEnd = pShape2->GetPortAt(CLOCK_2);
		}
		else if (pShape1->GetTopLeft().y > pShape2->GetTopLeft().y)
		{
			pStart = pShape1->GetPortAt(CLOCK_9/*CLOCK_11*/);
			pEnd = pShape2->GetPortAt(CLOCK_9/*CLOCK_7*/);
		}
		else if (pShape1->GetTopLeft().y < pShape2->GetTopLeft().y)
		{
			pStart = pShape1->GetPortAt(CLOCK_9/*CLOCK_7*/);
			pEnd = pShape2->GetPortAt(CLOCK_9/*CLOCK_11*/);
		}
		else
		{
			MyMessageBox_Error(_T("addNTFSRelation Error."), _T("Error"));
			return NULL;
		}
	}
	else
	{
		if (pShape1->GetTopLeft().x < pShape2->GetTopLeft().x)
		{
			pStart = pShape1->GetPortAt(CLOCK_3);
			pEnd = pShape2->GetPortAt(CLOCK_9);
		}
		else if (pShape1->GetTopLeft().x > pShape2->GetTopLeft().x)
		{
			pStart = pShape1->GetPortAt(CLOCK_9);
			pEnd = pShape2->GetPortAt(CLOCK_3);
		}
		else if (pShape1->GetTopLeft().y > pShape2->GetTopLeft().y)
		{
			pStart = pShape1->GetPortAt(CLOCK_0);
			pEnd = pShape2->GetPortAt(CLOCK_6);
		}
		else if (pShape1->GetTopLeft().y < pShape2->GetTopLeft().y)
		{
			pStart = pShape1->GetPortAt(CLOCK_6);
			pEnd = pShape2->GetPortAt(CLOCK_0);
		}
		else
		{
			MyMessageBox_Error(_T("addNTFSRelation Error."), _T("Error"));
			return NULL;
		}
	}
	
	CFOLinkShape *pLinkComp = new CFOLinkShape();
	pLinkComp->AddRef();
	CPoint ptPoints[2];
	ptPoints[0] = pStart->GetLinkPoint();
	ptPoints[1] = pEnd->GetLinkPoint();
	pLinkComp->Create(ptPoints, 2);
	pLinkComp->SetObjectCaption(strName);
	
	pLinkComp->SetFromPort(pStart);
	pLinkComp->SetToPort(pEnd);
	
	pLinkComp->SetLineWidth(2);
	pLinkComp->SetShadow(FALSE);
	if (iPermission == 0)
	{
		pLinkComp->SetLineColor(RGB(0, 0, 255));
	}
	else if (iPermission == 1)
	{
		pLinkComp->SetLineColor(RGB(255, 0, 0));
	}
	else if (iPermission == 2)   //?
	{
		pLinkComp->SetLineColor(RGB(255, 0, 255));
	}
	else
	{
		MyMessageBox_Error(_T("addNTFSRelation Error."), _T("Error"));
		return NULL;
	}
	pLinkComp->SetModel(GetCurrentModel());
	pStart->AddLinkShape(pLinkComp);
	pEnd->AddLinkShape(pLinkComp);
	
	// Set arrow type.
	int nType = 5;
	pLinkComp->SetEndArrowType(nType);
	pLinkComp->DoEndPropChange();
	GetCurrentModel()->AddChildAtTail(pLinkComp);
	pLinkComp->UpdateComp();
	pLinkComp->RelayoutPoints();
	//pLinkComp->Release();
	m_links.push_back(pLinkComp);
	return pLinkComp;
}

CFOLinkShape* CVisualADSView::addShareRelation(CString strName, CFODrawPortsShape *pShape1, CFODrawPortsShape *pShape2, int iPermission,
											  BOOL bBothRelation)
{
	//生成share权限的linkshape
	CFOPortShape *pStart;
	CFOPortShape *pEnd;
	if (bBothRelation == TRUE)
	{
		if (pShape1->GetTopLeft().x < pShape2->GetTopLeft().x)
		{
			pStart = pShape1->GetPortAt(CLOCK_4);
			pEnd = pShape2->GetPortAt(CLOCK_9/*CLOCK_8*/);
		}
		else if (pShape1->GetTopLeft().x > pShape2->GetTopLeft().x)
		{
			pStart = pShape1->GetPortAt(CLOCK_8);
			pEnd = pShape2->GetPortAt(CLOCK_4);
		}
		else if (pShape1->GetTopLeft().y > pShape2->GetTopLeft().y)
		{
			pStart = pShape1->GetPortAt(CLOCK_3/*CLOCK_1*/);
			pEnd = pShape2->GetPortAt(CLOCK_3/*CLOCK_5*/);
		}
		else if (pShape1->GetTopLeft().y < pShape2->GetTopLeft().y)
		{
			pStart = pShape1->GetPortAt(CLOCK_3/*CLOCK_5*/);
			pEnd = pShape2->GetPortAt(CLOCK_3/*CLOCK_1*/);
		}
		else
		{
			MyMessageBox_Error(_T("addShareRelation Error."), _T("Error"));
			return NULL;
		}
	}
	else
	{
		if (pShape1->GetTopLeft().x < pShape2->GetTopLeft().x)
		{
			pStart = pShape1->GetPortAt(CLOCK_3);
			pEnd = pShape2->GetPortAt(CLOCK_9);
		}
		else if (pShape1->GetTopLeft().x > pShape2->GetTopLeft().x)
		{
			pStart = pShape1->GetPortAt(CLOCK_9);
			pEnd = pShape2->GetPortAt(CLOCK_3);
		}
		else if (pShape1->GetTopLeft().y > pShape2->GetTopLeft().y)
		{
			pStart = pShape1->GetPortAt(CLOCK_3/*CLOCK_0*/);
			pEnd = pShape2->GetPortAt(CLOCK_3/*CLOCK_6*/);
		}
		else if (pShape1->GetTopLeft().y < pShape2->GetTopLeft().y)
		{
			pStart = pShape1->GetPortAt(CLOCK_3/*CLOCK_6*/);
			pEnd = pShape2->GetPortAt(CLOCK_3/*CLOCK_0*/);
		}
		else
		{
			MyMessageBox_Error(_T("addShareRelation Error."), _T("Error"));
			return NULL;
		}
	}
	
	CFOLinkShape *pLinkComp = new CFOLinkShape();
	pLinkComp->AddRef();
	CPoint ptPoints[2];
	ptPoints[0] = pStart->GetLinkPoint();
	ptPoints[1] = pEnd->GetLinkPoint();
	pLinkComp->Create(ptPoints, 2);
	pLinkComp->SetObjectCaption(strName);
	
	pLinkComp->SetFromPort(pStart);
	pLinkComp->SetToPort(pEnd);
	
	pLinkComp->SetLineWidth(2);
	pLinkComp->SetShadow(FALSE);
	if (iPermission == 0)
	{
		pLinkComp->SetLineColor(RGB(0, 0, 255));
	}
	else if (iPermission == 1)
	{
		pLinkComp->SetLineColor(RGB(255, 0, 0));
	}
	else if (iPermission == 2)
	{
		pLinkComp->SetLineColor(RGB(255, 0, 255));
	}
	else
	{
		MyMessageBox_Error(_T("addShareRelation Error."), _T("Error"));
		return NULL;
	}
	pLinkComp->SetModel(GetCurrentModel());
	
	pStart->AddLinkShape(pLinkComp);
	pEnd->AddLinkShape(pLinkComp);
	
	// Set arrow type.
	int nType = 5;
	pLinkComp->SetEndArrowType(nType);
	pLinkComp->DoEndPropChange();
	GetCurrentModel()->AddChildAtTail(pLinkComp);
	pLinkComp->UpdateComp();
	pLinkComp->RelayoutPoints();
	//pLinkComp->Release();
	m_links.push_back(pLinkComp);
	return pLinkComp;
}

CMyShape* CVisualADSView::addGroupFrame(int iX, int iY)
{
	//选中组时的虚线框
	if (m_frameShape != NULL)
	{
		CPoint ptOrigin = m_frameShape->GetCenterPoint();
		if (ptOrigin.x == iX && ptOrigin.y == iY)
		{
			return m_frameShape;
		}
		else
		{
			GetCurrentModel()->RemoveShape(m_frameShape);
			//delete m_frameShape;
			m_frameShape = NULL;
		}
	}

	CRect rctPosition = CRect(iX - m_ptGroupFrameSize.x / 2, iY - m_ptGroupFrameSize.y / 2 - 2, iX + m_ptGroupFrameSize.x / 2,
		iY + m_ptGroupFrameSize.y / 2 + 2);
	CMyShape *pNode = new CMyShape(abstractADObject, this, FALSE);
	pNode->AddRef();
	pNode->Create(IDR_COMPSRES_GROUP_FRAME, rctPosition, _T("group_frame"));
	pNode->SetSelectProtect(TRUE);
	m_frameShape = pNode;
	
	GetCurrentModel()->InsertShape(pNode);
	pNode->UpdateComp();
	pNode->Release();
	return pNode;
}

CMyShape* CVisualADSView::addUserFrame(int iX, int iY)
{
	//选中用户时的虚线框
	if (m_frameShape != NULL)
	{
		CPoint ptOrigin = m_frameShape->GetCenterPoint();
		if (ptOrigin.x == iX && ptOrigin.y == iY)
		{
			return m_frameShape;
		}
		else
		{
			GetCurrentModel()->RemoveShape(m_frameShape);
			//delete m_frameShape;
			m_frameShape = NULL;
		}
	}
	
	CRect rctPosition = CRect(iX - m_ptUserFrameSize.x / 2, iY - m_ptUserFrameSize.y / 2 - 1, iX + m_ptUserFrameSize.x / 2,
		iY + m_ptUserFrameSize.y / 2 + 1);
	CMyShape *pNode = new CMyShape(abstractADObject, this, FALSE);
	pNode->AddRef();
	pNode->Create(IDR_COMPSRES_GROUP_FRAME, rctPosition, _T("user_frame"));
	pNode->SetSelectProtect(TRUE);
	m_frameShape = pNode;
	
	GetCurrentModel()->InsertShape(pNode);
	pNode->UpdateComp();
	pNode->Release();
	return pNode;
}

CMyShape* CVisualADSView::addComputerFrame(int iX, int iY)
{
	//选中计算机时的虚线框
	if (m_frameShape != NULL)
	{
		CPoint ptOrigin = m_frameShape->GetCenterPoint();
		if (ptOrigin.x == iX && ptOrigin.y == iY)
		{
			return m_frameShape;
		}
		else
		{
			GetCurrentModel()->RemoveShape(m_frameShape);
			//delete m_frameShape;
			m_frameShape = NULL;
		}
	}

	CRect rctPosition = CRect(iX - m_ptComputerFrameSize.x / 2, iY - m_ptComputerFrameSize.y / 2, iX + m_ptComputerFrameSize.x / 2, iY + m_ptComputerFrameSize.y / 2);
	CMyShape *pNode = new CMyShape(abstractADObject, this, FALSE);
	pNode->AddRef();
	pNode->Create(IDR_COMPSRES_COMPUTER_FRAME, rctPosition, _T("computer_frame"));
	pNode->SetSelectProtect(TRUE);
	m_frameShape = pNode;
	
	GetCurrentModel()->InsertShape(pNode);
	pNode->UpdateComp();
	pNode->Release();
	return pNode;
}

CMyShape* CVisualADSView::addResourceFrame(int iX, int iY)
{
	//选中共享文件夹时的虚线框 resource
	if (m_frameShape != NULL)
	{
		CPoint ptOrigin = m_frameShape->GetCenterPoint();
		if (ptOrigin.x == iX && ptOrigin.y == iY)
		{
			return m_frameShape;
		}
		else
		{
			GetCurrentModel()->RemoveShape(m_frameShape);
			//delete m_frameShape;
			m_frameShape = NULL;
		}
	}

	CRect rctPosition = CRect(iX - m_ptResourceFrameSize.x / 2, iY - m_ptResourceFrameSize.y / 2 - 1,
		iX + m_ptResourceFrameSize.x / 2, iY + m_ptResourceFrameSize.y / 2 + 1);
	CMyShape *pNode = new CMyShape(abstractADObject, this, FALSE);
	pNode->AddRef();
	pNode->Create(IDR_COMPSRES_RESOURCE_FRAME, rctPosition, _T("resource_frame"));
	pNode->SetSelectProtect(TRUE);
	m_frameShape = pNode;
	
	GetCurrentModel()->InsertShape(pNode);
	pNode->UpdateComp();
	pNode->Release();
	return pNode;
}

void CVisualADSView::deleteAllShapes()
{
	GetCurrentModel()->ClearAll();
	return;
	//删除所有E-XD++中的节点
	for (int i = 0; i < m_shapes.size(); i ++)
	{
		delete m_shapes[m_shapes.size() - 1];
		m_shapes.pop_back();
	}
}

void CVisualADSView::deleteAllLinks()
{
	return;
	//删除所有E-XD++中的边
	for (int i = 0; i < m_links.size(); i ++)
	{
		delete m_links[m_links.size() - 1];
		m_links.pop_back();
	}
}

void CVisualADSView::DoChangeCanvasSize(CPoint ptGraphSize)
{
	CSize szCanvas = CSize(ptGraphSize.x, ptGraphSize.y);
	GetCurrentModel()->SetCanvasSize(szCanvas);
}

void CVisualADSView::MyDoChangeModel(CPoint ptGraphSize)
{
	CFODataModel *pModel = GetCurrentModel();
	CFODrawView::DoChangeModel(pModel);
	// Adding your data model init codes here..
	CSize szCanvas = CSize(ptGraphSize.x, ptGraphSize.y);
	pModel->SetCanvasSize(szCanvas);
	pModel->SetBorderShow(FALSE);
	GetCurrentModel()->SetPageBkColor(RGB(255, 255, 255));
	pModel->ShowGrid(FALSE);
	pModel->SetDesignMode(FALSE);
	pModel->SetHlpLineVisible(FALSE);

	ResetToSelectMode();
	EnableMirrorFreeMode(FALSE);
	EnableShearFreeMode(FALSE);
	//EnableScrollBar(SB_BOTH,ESB_DISABLE_BOTH);
	CSize sizeTotal;
	sizeTotal.cx = 1;
	sizeTotal.cy = 1;
	SetScrollSizes(MM_TEXT, sizeTotal);
	SetScrollRange(0, 0, 0);
	FOSetCursor(IDC_FO_CURSOR_RADIO2);
}


void CVisualADSView::OnSize(UINT nType, int cx, int cy) 
{
	CFODrawView::OnSize(nType, cx, cy);

	CMainFrame *pMainFrm = (CMainFrame *) AfxGetMainWnd();
	if (pMainFrm->m_hWndJava)
	{
		CView *pView = (CView *) this;
		RECT rect, rect2;
		pView->GetClientRect(&rect);
		pView->ClientToScreen (&rect);
		pMainFrm->GetWindowRect(&rect2);
		rect.left = rect.left - rect2.left - 8;
		rect.bottom = rect.bottom - rect2.top + 0;
		rect.right = rect.right - rect2.left - 8;
		rect.top = rect.top - rect2.top + 0;
		int nWidht = GetSystemMetrics(SM_CXVSCROLL);
		int nHeight = GetSystemMetrics(SM_CYVSCROLL);
		::MoveWindow(pMainFrm->m_hWndJava, rect.left, rect.top, rect.right - rect.left + nWidht, rect.bottom - rect.top + nHeight, TRUE);
		//::SetWindowPos(pMainFrm->m_hWndJava, m_hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW | SWP_NOACTIVATE);
	}
}
