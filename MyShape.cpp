// MyShape.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MyShape.h"
#include "VisualADSView.h"
#include "ADObject.h"

#include <fo.H>
#include <map>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CMyShape Class
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//IMPLEMENT_SERIAL(CMyShape, CFOCompositeShape, 0)

CMyShape::CMyShape()
{
	//	SetType(Your own ID); you must asign your own ID here.
	m_aDObject = NULL;
	m_pView = NULL;
	m_bIsAtomic = TRUE;
}

// void CMyShape::Serialize(CArchive& ar)
// {
// 	if (ar.IsStoring())
// 	{
// 		ar << m_bIsAtomic;
// 	}
// 	else
// 	{
// 		ar >> m_bIsAtomic;
// 		//Intialization needed: m_aDObject --> 构造CMyShape时设置
// 		//Intialization needed: m_pView --> 
// 	}
// 	
// 	CFOCompositeShape::Serialize(ar);
// }

CMyShape::CMyShape(ADObject *aDObject, CVisualADSView *pView, BOOL bIsAtomic)
{
	//	SetType(Your own ID); you must asign your own ID here.
	m_aDObject = aDObject;
	m_pView = pView;
	m_bIsAtomic = bIsAtomic;
}

CMyShape::CMyShape(ADObject *aDObject, CVisualADSView *pView, const CMyShape& src)
{
	m_aDObject = aDObject;
	m_pView = pView;
	*this = src;
	m_bIsAtomic = TRUE;
}

CMyShape::~CMyShape()
{

}

/////////////////////////////////////////////////////////////////////////////
// CMyShape diagnostics

#ifdef _DEBUG
void CMyShape::AssertValid() const
{
	CFOCompositeShape::AssertValid();
}

void CMyShape::Dump(CDumpContext& dc) const
{
	CFOCompositeShape::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CMyShape::Create(CRect &rcPos,CString strCaption)
{
	CFOCompositeShape::Create(rcPos,strCaption);

	// FODO: Add your own code here.

}

void CMyShape::Create(UINT nID, const CRect &rcPos, CString strCaption)
{
	CFOCompositeShape::Create(nID,rcPos,strCaption);
}

/////////////////////////////////////////////////////////////////////////////
// CPrintBaseJob serialization
CFile* CMyShape::GetFile(LPCTSTR lpszFileName, UINT nOpenFlags,
								 CFileException* pError)
{
	CMirrorFile* pFile = new CMirrorFile;
	ASSERT(pFile != NULL);
	if (!pFile->Open(lpszFileName, nOpenFlags, pError))
	{
		delete pFile;
		pFile = NULL;
	}
	return pFile;
}

void CMyShape::ReleaseFile(CFile* pFile, BOOL bAbort)
{
	ASSERT_KINDOF(CFile, pFile);
	if (bAbort)
		pFile->Abort(); // will not throw an exception
	else
		pFile->Close();
	delete pFile;
}

BOOL CMyShape::OpenDocument(LPCTSTR lpszPathName)
{
	
	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		return FALSE;
	}
	
	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			Serialize(loadArchive);     // load me
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		
		return FALSE;
	}
	END_CATCH_ALL
		
		
		return TRUE;
}

BOOL CMyShape::SaveDocument(LPCTSTR lpszPathName)
{
	
	CFileException fe;
	CFile* pFile = NULL;
	pFile = GetFile(lpszPathName, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive, &fe);
	
	if (pFile == NULL)
	{
		
		return FALSE;
	}
	
	CArchive saveArchive(pFile, CArchive::store | CArchive::bNoFlushOnDelete);
	saveArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		Serialize(saveArchive);     // save me
		saveArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		
		return FALSE;
	}
	END_CATCH_ALL
		
		
		return TRUE;        // success
}

CMyShape& CMyShape::operator=(const CMyShape& src)
{
	(*(CFOCompositeShape*)this) = src;
	return (*this);
}

CFODrawShape* CMyShape::Copy() const
{
	return (new CMyShape(*this));
}

void CMyShape::PrepareDC(CDC* pDC)
{
	CFOCompositeShape::PrepareDC(pDC);

	//FODO:Add your own code below.
	
}

void CMyShape::OnDrawTrackCustom(CDC *pDC)
{
	CFOCompositeShape::OnDrawTrackCustom(pDC);

	// Add your own code here.

}

void CMyShape::OnDrawShadow(CDC *pDC)
{
	CFOCompositeShape::OnDrawShadow(pDC);
	
	//FODO:Add your own code below.
	
}

void CMyShape::OnDrawFlat(CDC *pDC)
{
	CFOCompositeShape::OnDrawFlat(pDC);

	//FODO:Add your own code below.
	
}

void CMyShape::OnDraw3d(CDC *pDC)
{
	CFOCompositeShape::OnDraw3d(pDC);

	//FODO:Add your own code below.

}

void CMyShape::ClearDC(CDC* pDC)
{
	CFOCompositeShape::ClearDC(pDC);

	//FODO:Add your own code below.
}

void CMyShape::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bIsAtomic)
		m_pView->m_bEnableDragCanvas = 0;
	//if (m_aDObject)
	//	m_aDObject->onButttonDown();
	displayProperties();
	CFOCompositeShape::OnLButtonDown(nFlags,point);
}

void CMyShape::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bIsAtomic)
		m_pView->m_bEnableDragCanvas = 1;
	//if (m_aDObject)
	//	m_aDObject->onButtonUp();
	CFOCompositeShape::OnLButtonUp(nFlags,point);
}

void CMyShape::OnMouseMove(UINT nFlags, CPoint point)
{
}

void CMyShape::GeometryUpdated(CFOArea* pRgn)
{
	CFOCompositeShape::GeometryUpdated(pRgn);

	//FODO:Add your own code below.
}

void CMyShape::displayProperties()
{
	int i;
	if (!m_aDObject)
	{
		return;
	}
	map<CString, CString>::iterator iter;
	Property_ClearKeyValues();

	if (m_aDObject->infos.size() != 0)
	{
		i = 0;
		for (iter = m_aDObject->infos.begin(); iter != m_aDObject->infos.end(); iter ++)
		{
			Property_AddKeyValue_Basic(iter->first, iter->second);
			if (i >= 6)
			{
				break;
			}
			else
			{
				i ++;
			}
		}
		iter ++;
		for (; iter != m_aDObject->infos.end(); iter ++)
		{
			Property_AddKeyValue_Extend(iter->first, iter->second);
		}
	}
	
	for (iter = m_aDObject->fatherInfos.begin(); iter != m_aDObject->fatherInfos.end(); iter ++)
	{
		Property_AddKeyValue_Father(iter->first, iter->second);
	}
	for (iter = m_aDObject->childGroupInfos.begin(); iter != m_aDObject->childGroupInfos.end(); iter ++)
	{
		Property_AddKeyValue_ChildGroup(iter->first, iter->second);
	}
	for (iter = m_aDObject->childUserInfos.begin(); iter != m_aDObject->childUserInfos.end(); iter ++)
	{
		Property_AddKeyValue_ChildUser(iter->first, iter->second);
	}
	for (iter = m_aDObject->accessInfos.begin(); iter != m_aDObject->accessInfos.end(); iter ++)
	{
		Property_AddKeyValue_Access(iter->first, iter->second);
	}
	for (iter = m_aDObject->networkInfos.begin(); iter != m_aDObject->networkInfos.end(); iter ++)
	{
		Property_AddKeyValue_Network(iter->first, iter->second);
	}

	Property_UpdateKeyValues();
}