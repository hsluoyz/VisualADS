// VisualADSDoc.h : interface of the CVisualADSDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VISUALADSDOC_H__6D1E4D67_D102_41D5_9B14_9549F6D42C8A__INCLUDED_)
#define AFX_VISUALADSDOC_H__6D1E4D67_D102_41D5_9B14_9549F6D42C8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fo.H>

class CVisualADSDoc : public CFODoc
{
protected: // create from serialization only
	CVisualADSDoc();
	DECLARE_DYNCREATE(CVisualADSDoc)

// Attributes
public:

// Operations
public:
	void MyOnClose();
	void clearFilePathName();
	void MyFileClose();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisualADSDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVisualADSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVisualADSDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISUALADSDOC_H__6D1E4D67_D102_41D5_9B14_9549F6D42C8A__INCLUDED_)
