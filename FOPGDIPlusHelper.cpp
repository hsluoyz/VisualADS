//////////////////////////////////////////////////////////////////////
/*****************************************************************/
// This file is a part of the E-XD++ MFC Extention Class.
// 2001-200? ucancode.net Software, All Rights Reserved.
// This is only define for UCanCode Software E-XD++ Library.
//
// UCanCode SOFTWARE GRANTS TO YOU (ONE SOFTWARE PROGRAMMER) THE
// LIMITED RIGHT TO USE THIS SOFTWARE ON A SINGLE COMPUTER.  
//  THESE SOURCE FILE ARE CONSIDERED CONFIDENTIONAL AND ARE 
// THE PROPERTY OF UCanCode SOFTWARE AND ARE NOT TO BE RE-DISTRIBUTED
// BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN CONSENT OF 
// UCanCode SOFTWARE.
//
// You can contact us.
// Support@ucancode.net
// http://www.ucancode.net
/********************************************************************/
// FOPGDIPlusHelper.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FOPGDIPlusHelper.h"
#include "FOGlobals.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//SmoothingMode HokGdiPlusG::m_smoothingMode = SmoothingModeNone;
SmoothingMode HokGdiPlusG::m_smoothingMode = SmoothingModeHighQuality;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//	Pen
CFOPHokPenCache* CFOPHokPenCache::m_psPenCache = NULL;

CFOPHokPenCache::CFOPHokPenCache()
{
	if (m_psPenCache != NULL)
		ASSERT(m_psPenCache);
	else
		m_psPenCache = this;
}

CFOPHokPenCache::~CFOPHokPenCache()
{
	m_psPenCache = NULL;
}

CFOPHokPenCache* CFOPHokPenCache::GetInstance()
{
	if (m_psPenCache == NULL)
		m_psPenCache = new CFOPHokPenCache;

	ASSERT(m_psPenCache);
	return m_psPenCache;
}

void CFOPHokPenCache::DestoryInstance()
{
	if (m_psPenCache != NULL)
	{
		FOPHokPen *pPenTemp = NULL;
		POSITION pos = m_psPenCache->GetHeadPosition();
		while (pos)
		{
			pPenTemp = (FOPHokPen*)m_psPenCache->GetNext(pos);
			_SafeDeletePtr(pPenTemp->m_pPen);
			_SafeDeletePtr(pPenTemp);
		}
		m_psPenCache->RemoveAll();
	}
	_SafeDeletePtr(m_psPenCache);
}

Pen* CFOPHokPenCache::FindPen(FOPHokPenStyle &hokPenStyle)
{
	FOPHokPen* pCurFOPHokPen = NULL;
	POSITION pos = GetHeadPosition();
	while (pos)
	{
		pCurFOPHokPen = GetNext(pos);
		if (hokPenStyle == pCurFOPHokPen->m_penStyle)
		{//	找到的话，就返回这个
			ASSERT(pCurFOPHokPen->m_pPen);
			return pCurFOPHokPen->m_pPen;
		}
	}
	//	如果没有找到，创建一个
	return CreatePen(hokPenStyle);
}

Pen* CFOPHokPenCache::CreatePen(FOPHokPenStyle &hokPenStyle)
{
	FOPHokPen* hokPenTemp = new FOPHokPen();
	ASSERT(hokPenTemp);

	hokPenTemp->m_penStyle = hokPenStyle;

	hokPenTemp->m_pPen = new Pen(hokPenStyle.GetPenColor(), hokPenStyle.GetPenWidth());
	ASSERT(hokPenTemp->m_pPen);
	hokPenTemp->m_pPen->SetLineJoin(LineJoinRound);
	hokPenTemp->m_pPen->SetDashStyle(hokPenStyle.GetDashStyle());

	AddTail(hokPenTemp);
	
	TRACE(_T("Create: Pen Count: (%d)\n"), GetCount());
	return hokPenTemp->m_pPen;
}

Pen* CFOPHokPenCache::GetPen(CPen * pPen, const BYTE &nA)
{
	if (NULL == pPen) return NULL;

	LOGPEN logpen;
	if (pPen->GetLogPen(&logpen))
	{
		if (logpen.lopnStyle >= PS_NULL) return NULL;

		UINT style = logpen.lopnStyle&PS_STYLE_MASK;
		Color cr;
		cr = Color(nA, GetRValue(logpen.lopnColor),GetGValue(logpen.lopnColor),GetBValue(logpen.lopnColor));
		//	这个地方我们可以产生更加好的对象
		REAL fPenWidth = (logpen.lopnWidth.x == 0)? 1.0f:logpen.lopnWidth.x;
		FOPHokPenStyle penStyle(cr, fPenWidth, DashStyle(style));

		return FindPen(penStyle);
	}
	
	int size = ::GetObject(pPen->GetSafeHandle(), 0, NULL);
	if (size>0)
	{
		EXTLOGPEN *pPenEx = (EXTLOGPEN*) new char[size];
		if (GetObject(pPen->GetSafeHandle(), size, pPenEx) != 0)
		{
			Color cr; cr.SetFromCOLORREF(pPenEx->elpColor);
			//	这个地方我们可以产生更加好的对象
			REAL fPenWidth  = (pPenEx->elpWidth == 0)? 1.0f:pPenEx->elpWidth;

			REAL pDashValues[6] = {0,};
			for(int i = 0; i < (int)pPenEx->elpNumEntries; i++)
			{
				if (pPenEx->elpStyleEntry[i] == 1)
					pDashValues[i] = 1;
				else
					pDashValues[i] = pPenEx->elpStyleEntry[i]/fPenWidth;
			}

			DashStyle dashStyle = GetDashStyleByArr(pDashValues, pPenEx->elpNumEntries);

			FOPHokPenStyle penStyle(cr, fPenWidth, dashStyle);

			delete [](char*)pPenEx;
			return FindPen(penStyle);
		}
		delete [](char*)pPenEx;
	}
	return NULL;
}

DashStyle CFOPHokPenCache::GetDashStyleByArr(REAL *pArr, int nSize)
{
	if (2 == nSize)
	{
		if (pArr[0] == 1 && pArr[1] == 3)
		{
			return DashStyleDot;
		}

		if (pArr[0] == 2 && pArr[1] == 4)
		{
			return DashStyleDash;
		}
	}

	if (4 == nSize) 
	{
		if (pArr[0] == 2 && pArr[1] == 4&& pArr[2] == 1&& pArr[3] == 4)
		{
			return DashStyleDashDot;
		}
	}

	if (6 == nSize) 
	{
		if (pArr[0] == 2 && pArr[1] == 3&& pArr[2] == 1
			&& pArr[3] == 3&& pArr[4] == 1&& pArr[5] == 3)
		{
			return DashStyleDashDotDot;
		}
	}
	return DashStyleSolid;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//	Brush
CFOPHokBrushCache* CFOPHokBrushCache::m_psBrushCache = NULL;

CFOPHokBrushCache::CFOPHokBrushCache()
{
	if (m_psBrushCache != NULL)
		ASSERT(m_psBrushCache);
	else
		m_psBrushCache = this;
}

CFOPHokBrushCache::~CFOPHokBrushCache()
{
	ASSERT(m_psBrushCache != NULL);
}

CFOPHokBrushCache* CFOPHokBrushCache::GetInstance()
{
	if (m_psBrushCache == NULL)
		m_psBrushCache = new CFOPHokBrushCache;

	ASSERT(m_psBrushCache);
	return m_psBrushCache;
}

void CFOPHokBrushCache::DestoryInstance()
{
	if (m_psBrushCache != NULL)
	{
		FOPHokBrush *pBrushTemp = NULL;
		POSITION pos = m_psBrushCache->GetHeadPosition();
		while (pos)
		{
			pBrushTemp = (FOPHokBrush*)m_psBrushCache->GetNext(pos);
			_SafeDeletePtr(pBrushTemp->m_pBrush);
			_SafeDeletePtr(pBrushTemp);
		}
		m_psBrushCache->RemoveAll();
	}
	_SafeDeletePtr(m_psBrushCache);
}

Brush* CFOPHokBrushCache::FindBrush(FOPHokBrushStyle &hokBrushStyle)
{
	FOPHokBrush* pCurFOPHokBrush = NULL;
	POSITION pos = GetHeadPosition();
	while (pos)
	{
		pCurFOPHokBrush = GetNext(pos);
		if (hokBrushStyle == pCurFOPHokBrush->m_brushStyle)
		{//	找到的话，就返回这个
			ASSERT(pCurFOPHokBrush->m_pBrush);
			return pCurFOPHokBrush->m_pBrush;
		}
	}
	//	如果没有找到，创建一个
	return CreateBrush(hokBrushStyle);
}

Brush* CFOPHokBrushCache::CreateBrush(FOPHokBrushStyle &hokBrushStyle)
{
	Brush* brush = NULL;
	int nFillType = hokBrushStyle.GetBrushType();

	if (1 == nFillType)
	{
		brush = new SolidBrush(hokBrushStyle.GetFirColor());
	}
	else if (2 == nFillType)
	{
		brush = new SolidBrush(hokBrushStyle.GetSecColor());
	}
	else if (2 < nFillType && nFillType< 42)// 在这个地方GDI+ 有更加多的hatch Pattern
	{// 3-41 -- fill with hatch pattern.
		brush = new HatchBrush((HatchStyle)(HatchStyleMin + nFillType - 3), 
					hokBrushStyle.GetSecColor(), hokBrushStyle.GetFirColor());
	}
	else if (62 <= nFillType && nFillType < 92)
	{// 62 - 91 Fill with pattern bitmap.
		HBITMAP m_hbm = HBITMAP(gfxData.bitmap[nFillType-62].GetSafeHandle());
		brush = new TextureBrush(Bitmap::FromHBITMAP(m_hbm, NULL), WrapModeTile);
	}
	else
	{
		return NULL;
	}

	ASSERT(brush);
	FOPHokBrush* hokBrushTemp = new FOPHokBrush(brush);
	ASSERT(hokBrushTemp);
	hokBrushTemp->m_brushStyle = hokBrushStyle;

	AddTail(hokBrushTemp);

	TRACE(_T("Create a new brush in cache. Count = [%d].\n"), GetCount());
	return brush;
}

FOPHokBrushStyle CFOPHokBrushCache::GetStyle(Brush *pBrush)
{
	FOPHokBrush* pCurFOPHokBrush = NULL;
	POSITION pos = GetHeadPosition();
	while (pos)
	{
		pCurFOPHokBrush = GetNext(pos);
		if (pBrush == pCurFOPHokBrush->m_pBrush)
		{//	找到的话，就返回这个
			return pCurFOPHokBrush->m_brushStyle;
		}
	}
	//	如果没有找到，创建一个
	ASSERT(FALSE);
	FOPHokBrushStyle hokBrushStyle;
	return hokBrushStyle;
}

Brush* CFOPHokBrushCache::GetBrush(UINT nFillType, COLORREF crStart, COLORREF crEnd, const BYTE &nAStart, const BYTE &nAEnd)
{
	FOPHokBrushStyle hokBrushStyle(nFillType);

	Color crFir;
	crFir = Color(nAStart, GetRValue(crStart),GetGValue(crStart),GetBValue(crStart));
	hokBrushStyle.SetFirColor(crFir);

	Color crSec;
	crSec = Color(nAEnd, GetRValue(crEnd),GetGValue(crEnd),GetBValue(crEnd));

	hokBrushStyle.SetSecColor(crSec);

	return FindBrush(hokBrushStyle);
}

Brush* CFOPHokBrushCache::CreateGradientBrush(UINT nBrTypeIn, COLORREF crStartIn, COLORREF crEndIn, CRect rcIn, 
											  const BYTE &nAStart, const BYTE &nAEnd)
{
	Brush *pBrush = NULL;
	LinearGradientBrush *pLinearBrush = NULL;
	PathGradientBrush *pPathBrush = NULL;

	UINT nBrType = nBrTypeIn-42;

	Color crStart, crEnd;
	crStart = Color(nAStart, GetRValue(crStartIn),GetGValue(crStartIn),GetBValue(crStartIn));
	crEnd = Color(nAEnd, GetRValue(crEndIn),GetGValue(crEndIn),GetBValue(crEndIn));
	
	Rect rc(rcIn.left, rcIn.top, rcIn.Width(), rcIn.Height());

	switch(nBrType)
	{
	case GRADIENT_HORZ_TOP_BOTTOM:
		pBrush = new LinearGradientBrush(rc, crStart, crEnd, LinearGradientModeVertical);
		break;
		
	case GRADIENT_HORZ_BOTTOM_TOP:
		pBrush = new LinearGradientBrush(rc, crEnd, crStart, LinearGradientModeVertical);
		break;
		
	case GRADIENT_VERT_LEFT_RIGHT:
		pBrush = new LinearGradientBrush(rc, crEnd, crStart, LinearGradientModeHorizontal);
		break;

	case GRADIENT_VERT_RIGHT_LEFT:
		pBrush = new LinearGradientBrush(rc, crStart, crEnd, LinearGradientModeHorizontal);
		break;
		
	case GRADIENT_HORZ_CENTER_OUT:
		{
			REAL factors[3] = {0.0f, 1.0f, 0.0f};
			REAL positions[3] = {0.0f, 0.5f, 1.0f};
			pLinearBrush = new LinearGradientBrush(rc, crStart, crEnd, LinearGradientModeVertical);	
			pLinearBrush->SetBlend(factors, positions, 3);

			pBrush = pLinearBrush;
		}
		break;

	case GRADIENT_HORZ_CENTER_IN:
		{
			REAL factors[3] = {0.0f, 1.0f, 0.0f};
			REAL positions[3] = {0.0f, 0.5f, 1.0f};
			pLinearBrush = new LinearGradientBrush(rc, crEnd, crStart, LinearGradientModeVertical);	
			pLinearBrush->SetBlend(factors, positions, 3);
			
			pBrush = pLinearBrush;
		}
		break;

	case GRADIENT_VERT_MIDDLE_OUT:
		{
			REAL factors[3] = {0.0f, 1.0f, 0.0f};
			REAL positions[3] = {0.0f, 0.5f, 1.0f};
			pLinearBrush = new LinearGradientBrush(rc, crEnd, crStart, LinearGradientModeHorizontal);	
			pLinearBrush->SetBlend(factors, positions, 3);

			pBrush = pLinearBrush;
		}
		break;

	case GRADIENT_VERT_MIDDLE_IN:
		{
			REAL factors[3] = {0.0f, 1.0f, 0.0f};
			REAL positions[3] = {0.0f, 0.5f, 1.0f};
			pLinearBrush = new LinearGradientBrush(rc, crStart, crEnd, LinearGradientModeHorizontal);	
			pLinearBrush->SetBlend(factors, positions, 3);

			pBrush = pLinearBrush;
		}
		break;

	case GRADIENT_CORNER_TOP_LEFT:
		{
			Point pts[] = {Point(rc.GetLeft(), rc.GetTop()), Point(rc.GetRight(), rc.GetTop()), 
					Point(rc.GetRight(), rc.GetBottom()), Point(rc.GetLeft(), rc.GetBottom())};

			pPathBrush = new PathGradientBrush(pts, 4);	

			int count = 4;
			Color colors[] = {crStart, crStart, crStart, crStart};
			pPathBrush->SetSurroundColors(colors, &count);
			pPathBrush->SetCenterColor(crEnd);
			pPathBrush->SetCenterPoint(pts[0]);

			pBrush = pPathBrush;
		}
		break;
		
	case GRADIENT_CORNER_RIGHT_TOP:
		{
			Point pts[] = {Point(rc.GetLeft(), rc.GetTop()), Point(rc.GetRight(), rc.GetTop()), 
					Point(rc.GetRight(), rc.GetBottom()), Point(rc.GetLeft(), rc.GetBottom())};

			pPathBrush = new PathGradientBrush(pts, 4);	

			int count = 4;
			Color colors[] = {crStart, crStart, crStart, crStart};
			pPathBrush->SetSurroundColors(colors, &count);
			pPathBrush->SetCenterColor(crEnd);
			pPathBrush->SetCenterPoint(pts[1]);

			pBrush = pPathBrush;
		}
		break;
		
	case GRADIENT_CORNER_RIGHT_BOTTOM:
		{
			Point pts[] = {Point(rc.GetLeft(), rc.GetTop()), Point(rc.GetRight(), rc.GetTop()), 
					Point(rc.GetRight(), rc.GetBottom()), Point(rc.GetLeft(), rc.GetBottom())};

			pPathBrush = new PathGradientBrush(pts, 4);	

			int count = 4;
			Color colors[] = {crStart, crStart, crStart, crStart};
			pPathBrush->SetSurroundColors(colors, &count);
			pPathBrush->SetCenterColor(crEnd);
			pPathBrush->SetCenterPoint(pts[2]);
			
			pBrush = pPathBrush;
		}
		break;
		
	case GRADIENT_CORNER_BOTTOM_LEFT:
		{
			Point pts[] = {Point(rc.GetLeft(), rc.GetTop()), Point(rc.GetRight(), rc.GetTop()), 
					Point(rc.GetRight(), rc.GetBottom()), Point(rc.GetLeft(), rc.GetBottom())};

			pPathBrush = new PathGradientBrush(pts, 4);	

			int count = 4;
			Color colors[] = {crStart, crStart, crStart, crStart};
			pPathBrush->SetSurroundColors(colors, &count);
			pPathBrush->SetCenterColor(crEnd);
			pPathBrush->SetCenterPoint(pts[3]);

			pBrush = pPathBrush;
		}
		break;

	case GRADIENT_TITLE_OUT:
		{
			Point pts[] = {Point(rc.GetLeft(), rc.GetTop()), Point(rc.GetRight(), rc.GetTop()), 
					Point(rc.GetRight(), rc.GetBottom()), Point(rc.GetLeft(), rc.GetBottom())};

			pPathBrush = new PathGradientBrush(pts, 4);	

			int count = 4;
			Color colors[] = {crStart, crStart, crStart, crStart};
			pPathBrush->SetSurroundColors(colors, &count);
			pPathBrush->SetCenterColor(crEnd);
			pPathBrush->SetCenterPoint(Point(rc.X+INT(rc.Width/2), rc.Y+INT(rc.Height/2)));

			pBrush = pPathBrush;
		}
		break;
		
	case GRADIENT_TITLE_IN:
		{
			Point pts[] = {Point(rc.GetLeft(), rc.GetTop()), Point(rc.GetRight(), rc.GetTop()), 
					Point(rc.GetRight(), rc.GetBottom()), Point(rc.GetLeft(), rc.GetBottom())};

			pPathBrush = new PathGradientBrush(pts, 4);	

			int count = 4;
			Color colors[] = {crEnd, crEnd, crEnd, crEnd};
			pPathBrush->SetSurroundColors(colors, &count);
			pPathBrush->SetCenterColor(crStart);
			pPathBrush->SetCenterPoint(Point(rc.X+INT(rc.Width/2), rc.Y+INT(rc.Height/2)));

			pBrush = pPathBrush;
		}
		break;

	case GRADIENT_ELLIPSE_OUT:
		{
			GraphicsPath path;
			path.AddEllipse(rc);

			pPathBrush = new PathGradientBrush(&path);
			pPathBrush->SetGammaCorrection(TRUE);
			INT num = 1;
			pPathBrush->SetSurroundColors(&crStart, &num);
			pPathBrush->SetCenterColor(crEnd);
			
			pBrush = pPathBrush;
		}
		break;
		
	case GRADIENT_ELLIPSE_IN:
		{
			GraphicsPath path;
			path.AddEllipse(rc);

			pPathBrush = new PathGradientBrush(&path);
			pPathBrush->SetGammaCorrection(TRUE);
			INT num = 1;
			pPathBrush->SetSurroundColors(&crEnd, &num);
			pPathBrush->SetCenterColor(crStart);

			pBrush = pPathBrush;
		}
		break;

	case GRADIENT_ANGLE_45:
		pBrush = new LinearGradientBrush(rc, crEnd, crStart, LinearGradientModeForwardDiagonal);
		break;

	case GRADIENT_ANGLE_135:
		pBrush = new LinearGradientBrush(rc, crStart, crEnd, LinearGradientModeBackwardDiagonal);
		break;

	case GRADIENT_ANGLE_225:
		pBrush = new LinearGradientBrush(rc, crStart, crEnd, LinearGradientModeForwardDiagonal);
		break;

	case GRADIENT_ANGLE_315:
		pBrush = new LinearGradientBrush(rc, crEnd, crStart, LinearGradientModeBackwardDiagonal);
		break;
	}
	return pBrush;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Font
CFOPHokFontCache* CFOPHokFontCache::m_psFontCache = NULL;

CFOPHokFontCache::CFOPHokFontCache()
{
	if (m_psFontCache != NULL)
		ASSERT(m_psFontCache);
	else
		m_psFontCache = this;
}

CFOPHokFontCache::~CFOPHokFontCache()
{
	m_psFontCache = NULL;
}

CFOPHokFontCache* CFOPHokFontCache::GetInstance()
{
	if (m_psFontCache == NULL)
		m_psFontCache = new CFOPHokFontCache;

	ASSERT(m_psFontCache);
	return m_psFontCache;
}

void CFOPHokFontCache::DestoryInstance()
{
	if (m_psFontCache != NULL)
	{
		FOPHokFont *pFontTemp = NULL;
		POSITION pos = m_psFontCache->GetHeadPosition();
		while (pos)
		{
			pFontTemp = (FOPHokFont*)m_psFontCache->GetNext(pos);
			_SafeDeletePtr(pFontTemp->m_pFont);
			_SafeDeletePtr(pFontTemp);
		}
		m_psFontCache->RemoveAll();
	}
	_SafeDeletePtr(m_psFontCache);
}

Gdiplus::Font* CFOPHokFontCache::FindFont(FOPHokFontStyle &hokFontStyle)
{
	POSITION pos = GetHeadPosition();
	while (pos)
	{
		FOPHokFont* pCurFont = GetNext(pos);
		if (pCurFont->m_fontStyle == hokFontStyle)
		{
			ASSERT(pCurFont->m_pFont);
			return pCurFont->m_pFont;
		}
	}
	return CreateFont(hokFontStyle);
}

Gdiplus::Font* CFOPHokFontCache::CreateFont(FOPHokFontStyle &hokFontStyle)
{
	FOPHokFont* hokFontTemp = new FOPHokFont();
	ASSERT(hokFontTemp);

	hokFontTemp->m_fontStyle = hokFontStyle;

#ifdef _UNICODE
	CString lpwstrText = hokFontStyle.GetFaceName();
#else if
	USES_CONVERSION;
	LPWSTR lpwstrText = A2W(hokFontStyle.GetFaceName());
#endif

	FontFamily fontfamily(lpwstrText);
	hokFontTemp->m_pFont = new Gdiplus::Font(&fontfamily, hokFontStyle.GetHeight(),
							hokFontStyle.GetFontStyle(), UnitPoint);

	ASSERT(hokFontTemp->m_pFont);
	AddTail(hokFontTemp);

	TRACE(_T("Create: Font Count: (%d)\n"), GetCount());
	return hokFontTemp->m_pFont;
}

FOPHokFontStyle CFOPHokFontCache::GetStyle(Gdiplus::Font* pFont)
{
	POSITION pos = GetHeadPosition();
	while (pos)
	{
		FOPHokFont* pCurFont = GetNext(pos);
		if (pCurFont->m_pFont == pFont)
		{
			return pCurFont->m_fontStyle;
		}
	}
	FOPHokFontStyle hokFontStyle;
	return hokFontStyle;
}

Gdiplus::Font* CFOPHokFontCache::GetFont(CFont *pFont)
{
	if (NULL == pFont) return NULL;

	LOGFONT lplf;
	pFont->GetLogFont(&lplf);
	
	FOPHokFontStyle hokFontStyle;
	hokFontStyle.SetHeight(REAL(lplf.lfHeight));
	if (lplf.lfItalic == TRUE) {
		hokFontStyle.SetFontStyle((FontStyle)(hokFontStyle.GetFontStyle()|FontStyleItalic));
	}
	if (lplf.lfUnderline == TRUE) {
		hokFontStyle.SetFontStyle((FontStyle)(hokFontStyle.GetFontStyle()|FontStyleUnderline));
	}
	if (lplf.lfWeight > 700) {
		hokFontStyle.SetFontStyle((FontStyle)(hokFontStyle.GetFontStyle()|FontStyleBold));
	}
	hokFontStyle.SetHeight(REAL(lplf.lfHeight));
	
	hokFontStyle.SetFaceName(CString(lplf.lfFaceName));

	return FindFont(hokFontStyle);	
}
