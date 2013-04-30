#include "StdAfx.h"
#include "MyRibbonEdit.h"

//IMPLEMENT_DYNCREATE(CMyRibbonEdit, CBCGPRibbonEdit)

CMyRibbonEdit::CMyRibbonEdit() : CBCGPRibbonEdit()
{
}

CMyRibbonEdit::CMyRibbonEdit(UINT nID, int nWidth, LPCTSTR lpszLabel, int nImage) : CBCGPRibbonEdit(nID, nWidth, lpszLabel, nImage)
{
}

CMyRibbonEdit::~CMyRibbonEdit()
{
}

CBCGPRibbonEditCtrl* CMyRibbonEdit::CreateEdit(CWnd* pWndParent, DWORD dwEditStyle)
{
	return CBCGPRibbonEdit::CreateEdit(pWndParent, dwEditStyle | ES_PASSWORD);
}