#if !defined(FO_MYSHAPE_H__B4463970_B8B9_4E56_8F32_BFB9BFB3C932__INCLUDED_)
#define FO_MYSHAPE_H__B4463970_B8B9_4E56_8F32_BFB9BFB3C932__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//------------------------------------------------------
// VIsualADS Graphic User Interface developed by luo
// Author: luo.
//------------------------------------------------------

class CVisualADSView;
class ADObject;
#include <fo.H>
#include <GdiplusH.h>

class CMyShape : public CFOCompositeShape  
{
	//DECLARE_SERIAL(CMyShape);
public:
	BOOL m_bIsAtomic;
	ADObject *m_aDObject;
	CVisualADSView *m_pView;

public:
	CMyShape();
	//void Serialize(CArchive &ar);

	// constructor
	CMyShape(ADObject *aDObject, CVisualADSView *pView, BOOL bIsAtomic);

	// Copy constructor.
	CMyShape(ADObject *aDObject, CVisualADSView *pView, const CMyShape& src);

	// Destructor.
	virtual ~CMyShape();

	// Creates the button shape from a CRect object.
	virtual void Create(CRect &rcPos,CString strCaption = _T(""));

	virtual void Create(UINT nID, const CRect &rcPos, CString strCaption = _T(""));

public:

	// Assignment operator.
	CMyShape& operator=(const CMyShape& src);

	// Creates a copy of this shape.
	virtual CFODrawShape* Copy() const;

	//Generate Shape Area
	virtual void GeometryUpdated(CFOArea* pRgn);

public:
	void displayProperties();

public:
	
	// WM_LBUTTONDOWN message.
	virtual void OnLButtonDown(UINT nFlags, CPoint point); 

	// WM_LBUTTONUP message.
	virtual void OnLButtonUp(UINT nFlags, CPoint point);

	virtual void OnMouseMove(UINT nFlags, CPoint point);

public:
	
	// Save document.
	virtual BOOL SaveDocument(LPCTSTR lpszPathName);

	// Open document.
	virtual BOOL OpenDocument(LPCTSTR lpszPathName);
	
	// Get file.
	CFile *GetFile(LPCTSTR lpszFileName, UINT nOpenFlags,CFileException* pError);
	
	// Release file.
	virtual void ReleaseFile(CFile* pFile, BOOL bAbort);

public:

	//Draw flat status.

	// Draws custom tracker.
	virtual void OnDrawTrackCustom(CDC *pDC);

	// Creates GDI objects and sets up the device context for drawing. 
	virtual void PrepareDC(CDC* pDC);

	// Draws the flat status of the shape.
	virtual void OnDrawFlat(CDC *pDC);

	// Draws shadow of shape.
	virtual void OnDrawShadow(CDC *pDC);

	// Draws the 3D status of the shape.
	virtual void OnDraw3d(CDC *pDC);

	// Frees GDI objects and restores the state of the device context.
	virtual void ClearDC(CDC* pDC);
	// Implementation

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


#endif // !defined(FO_MYSHAPE_H__B4463970_B8B9_4E56_8F32_BFB9BFB3C932__INCLUDED_)
