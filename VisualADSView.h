// VisualADSView.h : interface of the CVisualADSView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VISUALADSVIEW_H__C51F58B0_3B0D_432D_89F0_526A93C78AE0__INCLUDED_)
#define AFX_VISUALADSVIEW_H__C51F58B0_3B0D_432D_89F0_526A93C78AE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyShape.h"

#include <vector>
using namespace std;

class CVisualADSDoc;
class ADObject;

class CVisualADSView : public CFODrawView
{
protected: // create from serialization only
	CVisualADSView();
	DECLARE_DYNCREATE(CVisualADSView)

// Attributes
public:
	CVisualADSDoc* GetDocument();

// Operations
public:
	void My_Serialize();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisualADSView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

public:
	vector<CMyShape*> m_shapes;
	vector<CFOLinkShape*> m_links;
	CMyShape* m_frameShape;
	POINT m_ptGroupSize;
	POINT m_ptUserSize;
	POINT m_ptComputerSize;
	POINT m_ptResourceSize;
	POINT m_ptGroupFrameSize;
	POINT m_ptUserFrameSize;
	POINT m_ptComputerFrameSize;
	POINT m_ptResourceFrameSize;

	BOOL m_bEnableDragCanvas;

public:
	void removeFrameShape();

	CMyShape* addDomain(ADObject *aDObject, CString strName, int iX1, int iY1, int iX2, int iY2);
	CMyShape* addOU(ADObject *aDObject, CString strName, int iX1, int iY1, int iX2, int iY2);
	CMyShape* addBigComputer(ADObject *aDObject, CString strName, int iX1, int iY1, int iX2, int iY2);
	CMyShape* addGroup(ADObject *aDObject, CString strName, int iX, int iY);
	CMyShape* addUser(ADObject *aDObject, CString strName, int iX, int iY);
	CMyShape* addComputer(ADObject *aDObject, CString strName, int iX, int iY);
	CMyShape* addResource(ADObject *aDObject, CString strName, int iX, int iY);

	CFOLinkShape* addInheritRelation(CString strName, CFODrawPortsShape *pShape1, CFODrawPortsShape *pShape2);
	CFOLinkShape* addNTFSRelation(CString strName, CFODrawPortsShape *pShape1, CFODrawPortsShape *pShape2, int iPermission, BOOL bBothRelation);
	CFOLinkShape* addShareRelation(CString strName, CFODrawPortsShape *pShape1, CFODrawPortsShape *pShape2, int iPermission, BOOL bBothRelation);

	CMyShape* addGroupFrame(int iX, int iY);
    CMyShape* addUserFrame(int iX, int iY);
	CMyShape* addComputerFrame(int iX, int iY);
	CMyShape* addResourceFrame(int iX, int iY);

	void deleteAllShapes();
	void deleteAllLinks();


	void FOSetCursor(UINT nID);
	virtual CSize GetCanvasOrginOffset() const;
	virtual void DoChangeModel(CFODataModel *pModel);
	virtual void DoEventSingleClick(CFODrawShape *pShape);

	void DoChangeCanvasSize(CPoint ptGraphSize);
	void MyDoChangeModel(CPoint ptGraphSize);

// Implementation
public:
	virtual ~CVisualADSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVisualADSView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in VisualADSView.cpp
inline CVisualADSDoc* CVisualADSView::GetDocument()
   { return (CVisualADSDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISUALADSVIEW_H__C51F58B0_3B0D_432D_89F0_526A93C78AE0__INCLUDED_)
